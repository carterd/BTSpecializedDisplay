#include "BluetoothHeartRateMonitor.h"

#include <ArduinoBLE.h>
#include <Arduino.h>

BluetoothHeartRateMonitor* BluetoothHeartRateMonitor::singleton = NULL;

void BluetoothHeartRateMonitor::update_heartrate_handler_cb(BLEDevice device, BLECharacteristic characteristic) {
    if (BluetoothHeartRateMonitor::singleton && BluetoothHeartRateMonitor::singleton->isBleDevice(device)) {
      BluetoothHeartRateMonitor::singleton->updateHeartRateCharacteristicCB(device, characteristic);
    }
}

/**
 * @brief Construct a new Bluetooth Heart Rate Monitor
 * 
 * @param bleDevice the BLE device which is the bile
 */
BluetoothHeartRateMonitor::BluetoothHeartRateMonitor() {   
    this->listener_obj = NULL;
    BluetoothHeartRateMonitor::singleton = this;

}

void BluetoothHeartRateMonitor::setBleDevice(BLEDevice& bleDevice) {
	if (this->bleDevice && this->isConnected())
		this->disconnect();
	if (this->bleDevice != bleDevice) {

   	    // Reset all the bike stats monitoring and timings
	}
	this->bleDevice = bleDevice;
}

/**
 * @brief Connects to the bike and updates the characterisitcs for the bike
 * 
 * @return true The bluetooth bike object is connected to the external bike
 * @return false Connection has failed this object is not connected to the external bike
 */
bool BluetoothHeartRateMonitor::connect(BLEDevice bleDevice) {
  // Ensure any existing bluetoothBike is cleaned up
  this->setBleDevice(bleDevice);

  if (this->bleDevice.connect()) {
    if (this->bleDevice.discoverAttributes()) {
      if (this->bleDevice.hasService(UUID_HEART_RATE_SERVICE)) {
        this->heartRateMeasurementBleCha = this->bleDevice.characteristic(UUID_HEART_RATE_MEASUREMENT_CHARACTERISTIC);
        this->bodySensorLocationBleCha = this->bleDevice.characteristic(UUID_BODY_SENSOR_LOCATION_CHARACTERISTIC);
      }
      this->statusLastUpdateTime = 0;
      // Heart rate is connected
      if (this->subscribeNotifications()) {
        return true;
      }
    }
    this->bleDevice.disconnect();
  }
  // Unable to make connection
  BLEDevice emptyDevice;
  this->bleDevice = emptyDevice;
  return false;
}

bool BluetoothHeartRateMonitor::disconnect() {
  if (this->isConnected()) {
    return this->bleDevice.disconnect();
  }
  return true;
}

bool BluetoothHeartRateMonitor::subscribeNotifications() {
  //  notifications use static handlers on the controller as the controller can identify which BluetoothBike to forward to
  if (this->heartRateMeasurementBleCha.canSubscribe() && this->heartRateMeasurementBleCha.subscribe()) {
    this->heartRateMeasurementBleCha.setEventHandler(BLEUpdated, BluetoothHeartRateMonitor::update_heartrate_handler_cb);
    return true;
  }
  return false;
}

/**
 * @brief Main entry for generic checking the bluetooth for change scanning or connection
 * 
 */
void BluetoothHeartRateMonitor::checkForChange() {
    // Controller believes the bike is connected
    if (this->isConnected()) {
        // Attempt to poll the device and receive any notifications
        this->poll();

        uint32_t timeStamp = millis();
        static uint32_t prevFetchTimeTicks[3];
        static uint32_t avrgGapTimeTicks[3];

        // If there is a listener and we've got some udpate information for the gui then perform callback
        if (this->listener_obj && this->getConnectedStateUpdated()) {
            lv_event_send(this->listener_obj, LV_EVENT_REFRESH, this);
        }
        this->statusLastCheckTime = this->statusLastUpdateTime;
    }
    else {
      this->statusLastUpdateTime = 0;
    }
}

void BluetoothHeartRateMonitor::poll() {
  if (this->isConnected())
    this->bleDevice.poll();
}

bool BluetoothHeartRateMonitor::isConnected() {
  return this->bleDevice.connected();
}

//
// Processing of Reading Writing and Notifications
//

void BluetoothHeartRateMonitor::readBufferToHeartRateMeasurement()
{
  bool statusUpdate = false;
  uint32_t time = millis();

  int readBufferIndex = 1;

  if (this->readBuffer[0] & static_cast<int>(HeartRateMonitorFlags::SENSOR_CONTACT_SUPPORT)) {
      this->heartRateMonitorState.sensorContactSupported = true;
      this->heartRateMonitorState.sensorContactStatus = this->readBuffer[0] & static_cast<int>(HeartRateMonitorFlags::SENSOR_CONTACT_STATUS);
  } else {
      this->heartRateMonitorState.sensorContactSupported = false;
      this->heartRateMonitorState.sensorContactStatus = false;
  }
  if (this->readBuffer[0] & static_cast<int>(HeartRateMonitorFlags::MEASURE_SIZE)) {
      this->heartRateMonitorState.heartRateBpm = this->bufferToUint16(readBufferIndex);
      readBufferIndex += 2;
  } else {
      this->heartRateMonitorState.heartRateBpm = this->bufferToUint8(readBufferIndex);      
      readBufferIndex ++;
  }
  if (this->readBuffer[0] & static_cast<int>(HeartRateMonitorFlags::ENERGY_EXPENDED_STATUS_BIT)) {
      this->heartRateMonitorState.expendedEnergy = this->bufferToUint16(readBufferIndex);
      readBufferIndex += 2;
  }
  if (this->readBuffer[0] & static_cast<int>(HeartRateMonitorFlags::RR_INTERVAL_SUPPORT_BIT)) {
      this->heartRateMonitorState.rrIntervalCount = 0;
      while (this->bufferSize > readBufferIndex && this->heartRateMonitorState.rrIntervalCount < 10) {
        this->heartRateMonitorState.rrInterval[this->heartRateMonitorState.rrIntervalCount++] = this->bufferToUint16(readBufferIndex) / 1024.0;
        readBufferIndex += 2;
      }
  }
  this->heartRateMonitorState.lastFetchTimeTicks = time;
  this->statusLastUpdateTime = time;
}

//
// Class callback functions, called by controller
//

void BluetoothHeartRateMonitor::updateHeartRateCharacteristicCB(BLEDevice device, BLECharacteristic characteristic)  {
  if (device == this->bleDevice) {    
    this->bufferSize = characteristic.readValue(this->readBuffer, 20);
    if (this->bufferSize > 1) {
      this->readBufferToHeartRateMeasurement();
    }
  }
}