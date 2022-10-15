#include "BluetoothBikeController.h"
#include <ArduinoBLE.h>

#include "Arduino.h"


//
// Static instance of bluetooth controller array
//
std::vector<BluetoothBikeController*> BluetoothBikeController::bluetoothBikeControllers;

BikeStateToBluetoothBikeRequest BluetoothBikeController::bikeStateToBluetoothBikeRequest;

//
// Static Handler functions are CB functions for BLE state changes
//
void BluetoothBikeController::connect_handler_cb(BLEDevice device) {
  for (std::vector<BluetoothBikeController *>::iterator it = std::begin(BluetoothBikeController::bluetoothBikeControllers); it != std::end(BluetoothBikeController::bluetoothBikeControllers); ++it) {
    if (device == (*it)->connectedDevice) {
      (*it)->updateConnectCB(device);
    }
  }
}

void BluetoothBikeController::disconnect_handler_cb(BLEDevice device) {
  for (std::vector<BluetoothBikeController *>::iterator it = std::begin(BluetoothBikeController::bluetoothBikeControllers); it != std::end(BluetoothBikeController::bluetoothBikeControllers); ++it) {
    if (device == (*it)->connectedDevice) {
      (*it)->updateDisconnectCB(device);
    }
  }
}

void BluetoothBikeController::update_ebs_handler_cb(BLEDevice device, BLECharacteristic characteristic) {
  for (std::vector<BluetoothBikeController *>::iterator it = std::begin(BluetoothBikeController::bluetoothBikeControllers); it != std::end(BluetoothBikeController::bluetoothBikeControllers); ++it) {
    // Check the addresses match to identify the controller is connected to this device
    if ((*it)->getConnected() && device == (*it)->getConnectedDevice()) {
      (*it)->updateEbsCharacteristicCB(device, characteristic);
    }
  }
}

void BluetoothBikeController::update_csc_handler_cb(BLEDevice device, BLECharacteristic characteristic) {
  for (std::vector<BluetoothBikeController *>::iterator it = std::begin(BluetoothBikeController::bluetoothBikeControllers); it != std::end(BluetoothBikeController::bluetoothBikeControllers); ++it) {
    // Check the addresses match to identify the controller is connected to this device
    if ((*it)->getConnected() && device == (*it)->getConnectedDevice()) {
      (*it)->updateCscCharacteristicCB(device, characteristic);
    }
  }
}

//
// Constructor
//

BluetoothBikeController::BluetoothBikeController() {
  if (!BluetoothBikeController::bluetoothBikeControllers.size()) {
    BLE.setEventHandler(BLEConnected, BluetoothBikeController::connect_handler_cb);
    BLE.setEventHandler(BLEDisconnected, BluetoothBikeController::disconnect_handler_cb);
  }
  BluetoothBikeController::bluetoothBikeControllers.push_back(this);
  this->listener_obj = NULL;
  this->scanning = false;
  this->scannedDeviceAvailable = false;
  this->connected = false;
  this->connectedBikeStateUpdated = false;
}

//
// Distructor
//

BluetoothBikeController::~BluetoothBikeController() {
  // On deleting this control ensure it's removed form the list of known controllers
  std::vector<BluetoothBikeController *>::iterator it = std::find(std::begin(BluetoothBikeController::bluetoothBikeControllers), std::end(BluetoothBikeController::bluetoothBikeControllers), this);
  if (it != BluetoothBikeController::bluetoothBikeControllers.end()) {
    BluetoothBikeController::bluetoothBikeControllers.erase(it);
  }
}

void BluetoothBikeController::init() {
  if (!BLE.begin()) {
    //Serial.println("starting BLE failed!");
    while (1);
  }
  this->scanning = false; 
  this->scannedDeviceAvailable = false; 
  this->connected = false;
}

bool BluetoothBikeController::startScan() {
  // Scans are not possible if already connected to the device
  if (this->connected) return false;
  if (this->scanning) return true;

  BLE.scan();
  this->scanning = true;
  this->scannedDeviceAvailable = false;
  return true;
}

bool BluetoothBikeController::continueScan() {
    // Scans are not possible if already connected to the device
    if (this->connected) return false;

    this->scannedDeviceAvailable = false;
    return true;
}

bool BluetoothBikeController::stopScan() {
  // Scans are not possible if already connected to the device
  if (this->connected) return false;
  if (!this->scanning) return true;

  BLE.stopScan();
  this->scanning = false;
  this->scannedDeviceAvailable = false;
  return true;
}

bool BluetoothBikeController::connect(BLEDevice* bleDevice) {
  if (this->connected) return true;
  this->stopScan();
  
  this->scannedDeviceAvailable = false;
  this->connectedDevice = *bleDevice;
  if (this->connectedDevice.connect()) {
    if (this->connectedDevice.discoverAttributes()) {
      this->connectedBikeType = BikeType::NONE;
      if (this->connectedDevice.hasService(UUID_SPECIALIZED_READ_SERVICE_GEN1)) {
        this->connectedBikeType = BikeType::GEN1;
        this->ebsReadKeyBleCha = this->connectedDevice.characteristic(UUID_SPECIALIZED_READ_KEY_CHARACTERISTIC_GEN1);
        this->ebsReadValueBleCha = this->connectedDevice.characteristic(UUID_SPECIALIZED_READ_VALUE_CHARACTERISTIC_GEN1);
        this->ebsWriteKeyValueBleCha = this->connectedDevice.characteristic(UUID_SPECIALIZED_WRITE_KEY_VALUE_CHARACTERISTIC_GEN1);
        this->ebsNotifyKeyValueBleCha = this->connectedDevice.characteristic(UUID_SPECIALIZED_NOTIFY_KEY_VALUE_CHARACTERISTIC_GEN1);
      }
      if (this->connectedDevice.hasService(UUID_SPECIALIZED_READ_SERVICE_GEN2)) {
        this->connectedBikeType = BikeType::GEN2;
        this->ebsReadKeyBleCha = this->connectedDevice.characteristic(UUID_SPECIALIZED_READ_KEY_CHARACTERISTIC_GEN2);
        this->ebsReadValueBleCha = this->connectedDevice.characteristic(UUID_SPECIALIZED_READ_VALUE_CHARACTERISTIC_GEN2);
        this->ebsWriteKeyValueBleCha = this->connectedDevice.characteristic(UUID_SPECIALIZED_WRITE_KEY_VALUE_CHARACTERISTIC_GEN2);
        this->ebsNotifyKeyValueBleCha = this->connectedDevice.characteristic(UUID_SPECIALIZED_NOTIFY_KEY_VALUE_CHARACTERISTIC_GEN2);
      }
      if (this->connectedDevice.hasService(UUID_CYCLE_SERVICE)) {
        this->cscMeasurementBleCha = this->connectedDevice.characteristic(UUID_CSC_MEASUREMENT_CHARACTERISTIC);
        this->cscFeatureBleCha = this->connectedDevice.characteristic(UUID_CSC_FEATURE_CHARACTERISTIC);
        this->scControlPointBleCha = this->connectedDevice.characteristic(UUID_SC_CONTROL_POINT_CHARACTERISTIC);
      }
      this->connected = true;
      this->connectedBikeStateUpdated = false;
      return true;
    }
    this->connectedDevice.disconnect();
  }
  // Unable to make connection
  this->connected = false;  
  return false;
}

bool BluetoothBikeController::disconnect() {
  if (this->connected) {
    this->connected = false;
    return this->connectedDevice.disconnect();
  }
  return true;
}

void BluetoothBikeController::checkForScanningChange() {
  if (this->scanning && !this->scannedDeviceAvailable) {
    this->scannedDevice = BLE.available();
    if (this->scannedDevice) {
      this->scannedDeviceAvailable = true;
      if (this->listener_obj) {
        lv_event_send(this->listener_obj, LV_EVENT_REFRESH, this);
      }
    }
  }
}

void BluetoothBikeController::checkForStaleBikeStateAttribute(MonitorAttributeType monitorAttributeType, uint32_t maximumTime) {
    // Only if we've done no reading of other attributes
    if (!this->connectedBikeStateUpdated) {
        BikeStateAttributeIndex bikeStateAttributeIndex = this->bikeState.getOldestStateAttribute(monitorAttributeType);
        if (bikeStateAttributeIndex != BikeStateAttributeIndex::BIKE_STATE_ATTRIBUTE_SIZE && this->bikeState.getStateAttribute(bikeStateAttributeIndex).lastFetchTimeTicks < maximumTime) {
            this->readBikeStateAttribute(bikeStateAttributeIndex, monitorAttributeType);
            this->connectedBikeStateUpdated = true;
        }
    }
}

void BluetoothBikeController::checkForConnectionChange() {
  if (this->connected) {
    // Attempt to poll the device
    this->connectedDevice.poll();
    // If we have lost connection then update the controller so this is the final update of state to allow reconnect functionality
    if (!this->connectedDevice.connected()) {
      this->connected = false;
      this->connectedBikeStateUpdated = true;
    }

    uint32_t timeStamp = millis();
    if (timeStamp > 60 * 1000) this->checkForStaleBikeStateAttribute(MonitorAttributeType::EVERY_MINUTE, timeStamp - 60 * 1000);
    if (timeStamp > 10 * 1000)  this->checkForStaleBikeStateAttribute(MonitorAttributeType::EVERY_TEN_SECONDS, timeStamp - 10 * 1000);
    if (timeStamp > 1000)  this->checkForStaleBikeStateAttribute(MonitorAttributeType::EVERY_SECOND, timeStamp - 1000);
    if (timeStamp > 20) this->checkForStaleBikeStateAttribute(MonitorAttributeType::CONSTANTLY, timeStamp - 20);


    // If there is a listener and we've got some udpate information for the gui then perform callback
    if (this->listener_obj && this->connectedBikeStateUpdated) {
      lv_event_send(this->listener_obj, LV_EVENT_REFRESH, this);
      this->connectedBikeStateUpdated = false;
    }
  }
}

void BluetoothBikeController::checkForChange() {
  this->checkForScanningChange();
  this->checkForConnectionChange();
}

bool BluetoothBikeController::readBikeStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, MonitorAttributeType monitorAttributeType)
{
    bool result;
    if (monitorAttributeType > this->bikeState.getStateAttribute(bikeStateAttributeIndex).monitorAttributeType) {
        this->bikeState.setMonitorAttributeType(bikeStateAttributeIndex, monitorAttributeType);
    }
    BluetoothBikeRequest bluetoothBikeRequest = this->bikeStateToBluetoothBikeRequest.getBluetoothBikeRequest(bikeStateAttributeIndex, this->getConnectedBikeType());
    result = this->readEbikeValue(bluetoothBikeRequest.request1.area, bluetoothBikeRequest.request1.attribute);
    if (result && bluetoothBikeRequest.request2.area != EbikeStatusArea::INVALID) {
        result = this->readEbikeValue(bluetoothBikeRequest.request2.area, bluetoothBikeRequest.request2.attribute);
    }
    
    return result;
}

void BluetoothBikeController::readBufferToCscMeasurement()
{
  bool bikeStatusUpdate = false;
  uint32_t time = millis();

  int readBufferIndex = 1;
  if (this->readBuffer[0] & static_cast<int>(CscMeasurementBit::CSC_WHEEL_REV_BIT)) {
    uint32_t wheelRotations = this->bufferToUint32(readBufferIndex);
    uint16_t wheelEventTime = this->bufferToUint16(readBufferIndex+4);
    readBufferIndex += 6;
    uint32_t lastWheelRotations = this->bikeState.getStateAttribute(BikeStateAttributeIndex::WHEEL_ROTATIONS).bikeStateAttributeValue.valueUint32;
    uint16_t lastWheelEventTime = this->bikeState.getStateAttribute(BikeStateAttributeIndex::LAST_WHEEL_EVENT_TIME).bikeStateAttributeValue.valueUint32;
    bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::WHEEL_ROTATIONS, wheelRotations, time);
    bikeStatusUpdate |= this->bikeState.setStateAttribute(BikeStateAttributeIndex::LAST_WHEEL_EVENT_TIME, lastWheelEventTime, time);
    // finally calculate the speed
    int32_t eventTimeDelta = lastWheelEventTime <= wheelEventTime ? 
      wheelEventTime - lastWheelEventTime : 0x10000 + wheelEventTime - lastWheelEventTime;
    int32_t wheelRotationsDelta = lastWheelRotations <= wheelRotations ?
      wheelRotation - lastWheelRotations : 0;
    float wheelRatationsPerSecond = 1000.0 * wheelRotationsDelta / eventTimeDelta;

  }
  if (this->readBuffer[0] & static_cast<int>(CscMeasurementBit::CSC_CRANK_REV_BIT)) {
    uint32_t wheelRotations = this->bufferToUint32(readBufferIndex);
    uint16_t wheelEventTime = this->bufferToUint16(readBufferIndex+4);
    readBufferIndex += 6;
  }
}


void BluetoothBikeController::readBufferToBikeState() {
  bool bikeStatusUpdate = false;
  uint32_t time = millis();
  EbikeStatusArea ebikeStatusArea = static_cast<EbikeStatusArea>(this->readBuffer[0]);
  //LV_LOG_USER("Read = [%x, %x] [%x, %x, %x, %x]", this->readBuffer[0],this->readBuffer[1], this->readBuffer[2],this->readBuffer[3], this->readBuffer[4],this->readBuffer[5]);
  switch (ebikeStatusArea) {
    case EbikeStatusArea::BATTERY: {
      EbikeStatusBattery ebikeStatusBattery = static_cast<EbikeStatusBattery>(this->readBuffer[1]);
      switch (ebikeStatusBattery) {
        case EbikeStatusBattery::CAPACITY:         bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::BATTERY_CAPACITY, this->bufferToUint16(), time); break;
        case EbikeStatusBattery::CAPACITY_REMAIN:  bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::BATTERY_CAPACITY_REMAINING, this->bufferToUint16(), time); break;
        case EbikeStatusBattery::HEALTH:           bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::BATTERY_HEALTH, this->bufferToUint8(), time); break;
        case EbikeStatusBattery::TEMP:             bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::BATTERY_TEMP, this->bufferToUint8(), time); break;
        case EbikeStatusBattery::CHARGE_CYCLES:    bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::BATTERY_CHARGE_CYCLES, this->bufferToUint16(), time); break;
        case EbikeStatusBattery::VOLTAGE:          bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::BATTERY_VOLTAGE, this->bufferToUint8() / 5.0f + 20, time); break;
        case EbikeStatusBattery::CURRENT:          bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::BATTERY_CURRENT, this->bufferToUint8() / 5.0f, time); break;
        case EbikeStatusBattery::HARDWARE_VERSION: {            
            // Possible code with GEN2
            HardwareVersion hardwareVersion { this->bufferToUint8(), 0 };
            bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::BATTERY_HARDWARE_VERSION, hardwareVersion, time);
          } break;
        case EbikeStatusBattery::FIRMWARE_VERSION: {
            FirmwareVersion firmwareVersion { this->bufferToUint8(BATTERY_FIRMWARE_VERSION_MAJOR_NUMBER_BUFFER_INDEX), 
                                              this->bufferToUint8(BATTERY_FIRMWARE_VERSION_MINOR_NUMBER_BUFFER_INDEX),
                                              this->bufferToUint8(BATTERY_FIRMWARE_VERSION_PATCH_NUMBER_BUFFER_INDEX) };
            bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::BATTERY_FIRMWARE_VERSION, firmwareVersion, time);
          } break;
        case EbikeStatusBattery::SERIAL_NO_GEN1:  bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::BATTERY_SERIAL_NUMBER, this->bufferToUint16(), time); break;
        case EbikeStatusBattery::CHARGE_PERCENT:  bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::BATTERY_CHARGE_PERCENT, this->bufferToUint8(), time); break;
        case EbikeStatusBattery::SERIAL_NO_GEN2_PART1:
        case EbikeStatusBattery::SERIAL_NO_GEN2_PART2:
        // GEN 2 TODO
          break;
      }
    }
    break;
    case EbikeStatusArea::MOTOR: {
      EbikeStatusMotor ebikeStatusMotor = static_cast<EbikeStatusMotor>(this->readBuffer[1]);
      switch (ebikeStatusMotor) {
        case EbikeStatusMotor::RIDER_POWER:   bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::RIDER_POWER, this->bufferToUint16(), time); break;
        case EbikeStatusMotor::CADENCE:       bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::MOTOR_CADENCE, this->bufferToUint16() / 10.0f, time); break;
        case EbikeStatusMotor::SPEED:         bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::MOTOR_SPEED, this->bufferToUint16() / 10.0f, time); break;
        case EbikeStatusMotor::ODOMETER:      bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::MOTOR_ODOMETER, this->bufferToUint32(), time); break;
        case EbikeStatusMotor::ASSIST_LEVEL:  bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::MOTOR_ASSIST_LEVEL, this->bufferToUint16(), time); break;
        case EbikeStatusMotor::TEMP:          bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::MOTOR_TEMP, this->bufferToUint8(), time); break;
        case EbikeStatusMotor::MOTOR_POWER:   bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::MOTOR_FIRMWARE_VERSION, this->bufferToUint16(), time); break;
        case EbikeStatusMotor::FIRMWARE_VERSION: {
          FirmwareVersion firmwareVersion { this->bufferToUint8(MOTOR_FIRMWARE_VERSION_MAJOR_NUMBER_BUFFER_INDEX), 
                                            this->bufferToUint8(MOTOR_FIRMWARE_VERSION_MINOR_NUMBER_BUFFER_INDEX), 
                                            this->bufferToUint8(MOTOR_FIRMWARE_VERSION_PATCH_NUMBER_BUFFER_INDEX) }; 
          bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::MOTOR_FIRMWARE_VERSION, firmwareVersion, time);
        } break;
        case EbikeStatusMotor::PEAK_ASSIST:
        {
            AssistLevels assistLevels = this->bikeState.getStateAttribute(BikeStateAttributeIndex::PEAK_ASSIST_LEVELS).bikeStateAttributeValue.assistLevels;
            switch (this->connectedBikeType) {
            case BikeType::GEN1: {                
                uint8_t peakTypeAndValue = this->bufferToUint8();
                uint8_t peakType = peakTypeAndValue % 10;
                uint8_t peakLevel = peakTypeAndValue - peakType;
                switch (peakType) {
                case 1: assistLevels.eco = peakLevel; break;
                case 2: assistLevels.trail = peakLevel; break;
                case 3: assistLevels.turbo = peakLevel; break;
                }
            } break;
            case BikeType::GEN2: {
                AssistLevels assistLevels = this->bikeState.getStateAttribute(BikeStateAttributeIndex::PEAK_ASSIST_LEVELS).bikeStateAttributeValue.assistLevels;
                assistLevels.eco = this->bufferToUint8(2);
                assistLevels.trail = this->bufferToUint8(3);
                assistLevels.turbo = this->bufferToUint8(4);
                bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::PEAK_ASSIST_LEVELS, assistLevels, time);
            } break;
            } break;
            bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::PEAK_ASSIST_LEVELS, assistLevels, time);
        } break;
        case EbikeStatusMotor::HARDWARE_NO_PART1: {
            char* motorHardwareNumber = this->bikeState.getStateAttribute(BikeStateAttributeIndex::MOTOR_HARDWARE_VERSION).bikeStateAttributeValue.numberString.value;
            bufferToCharArray(&(motorHardwareNumber[0]), 18);
            motorHardwareNumber[20] = 0x00;
            bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::MOTOR_HARDWARE_VERSION, time);
          } break;
        case EbikeStatusMotor::HARDWARE_NO_PART2: {
            char* motorHardwareNumber = this->bikeState.getStateAttribute(BikeStateAttributeIndex::MOTOR_HARDWARE_VERSION).bikeStateAttributeValue.numberString.value;
            bufferToCharArray(&(motorHardwareNumber[18]), 2);
            motorHardwareNumber[20] = 0x00;
            bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::MOTOR_HARDWARE_VERSION, time);
          } break;
        case EbikeStatusMotor::SERIAL_NO_PART1: {
            char* motorSerailNumber = this->bikeState.getStateAttribute(BikeStateAttributeIndex::MOTOR_SERIAL_NUMBER).bikeStateAttributeValue.numberString.value;
            bufferToCharArray(&(motorSerailNumber[0]), 18);
            motorSerailNumber[23] = 0x00;
            bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::MOTOR_SERIAL_NUMBER, time);
          } break;
        case EbikeStatusMotor::SERIAL_NO_PART2: {
            char* motorSerailNumber = this->bikeState.getStateAttribute(BikeStateAttributeIndex::MOTOR_SERIAL_NUMBER).bikeStateAttributeValue.numberString.value;
            bufferToCharArray(&(motorSerailNumber[18]), 5);
            motorSerailNumber[23] = 0x00;
            bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::MOTOR_SERIAL_NUMBER, time);
          } break;
      }
    } break;
    case EbikeStatusArea::OTHER: {
      EbikeStatusOther ebikeStatusOther = static_cast<EbikeStatusOther>(this->readBuffer[1]);
      switch (ebikeStatusOther) {
        case EbikeStatusOther::WHEEL_CIRCUMFERENCE: bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::WHEEL_CIRCUMFERENCE, this->bufferToUint16(), time); break;
        case EbikeStatusOther::ASSIST_ECO: {
          this->bikeState.getStateAttribute(BikeStateAttributeIndex::ASSIST_PERCENTAGE).bikeStateAttributeValue.assistLevels.eco = this->bufferToUint8();
          bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::ASSIST_PERCENTAGE, time);
        } 
        break;
        case EbikeStatusOther::ASSIST_TRAIL: {
            this->bikeState.getStateAttribute(BikeStateAttributeIndex::ASSIST_PERCENTAGE).bikeStateAttributeValue.assistLevels.trail = this->bufferToUint8();
            bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::ASSIST_PERCENTAGE, time);
        }
        break;
        case EbikeStatusOther::ASSIST_TURBO: {
            this->bikeState.getStateAttribute(BikeStateAttributeIndex::ASSIST_PERCENTAGE).bikeStateAttributeValue.assistLevels.turbo = this->bufferToUint8();
            bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::ASSIST_PERCENTAGE, time);
        }
        break;
        case EbikeStatusOther::BIKE_ON_OFF:         bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::BIKE_ON_OFF_STATE, this->bufferToUint8(), time); break;
        case EbikeStatusOther::BIKE_SERIAL_NO:
          {
            char* bikeSerailNumber = this->bikeState.getStateAttribute(BikeStateAttributeIndex::BIKE_SERIAL_NUMBER).bikeStateAttributeValue.numberString.value;
            bufferToCharArray(&(bikeSerailNumber[0]), 17);
            bikeSerailNumber[17] = 0x00;
            bikeStatusUpdate = this->bikeState.setStateAttribute(BikeStateAttributeIndex::BIKE_SERIAL_NUMBER, time);
            bikeStatusUpdate |= this->bikeState.setStateAttribute(BikeStateAttributeIndex::BATTERY_CONNECTED_STATE, (bikeSerailNumber[0] == 0xff && bikeSerailNumber[1] == 0xff && bikeSerailNumber[2] == 0xff && bikeSerailNumber[3]) ? false : true, time); break;
          }
          break;
      }
    } break;
  }
  this->connectedBikeStateUpdated |= bikeStatusUpdate;
}

bool BluetoothBikeController::readEbikeValue(int ebikeStatusArea, int ebikeStatusParameter) {
  // int32 requires the bytes in this order (due to endian conversion)
  uint16_t key = (ebikeStatusParameter << 8) + ebikeStatusArea;
  //LV_LOG_USER("ebsReadKeyBleChar.writeValue [%x, %x]", ebikeStatusArea, ebikeStatusParameter);
  if (this->ebsReadKeyBleCha.writeValue(key)) {
    // Ensure we've at least read the key back
    if (this->ebsReadValueBleCha.readValue(this->readBuffer, 20) > 1) {
      if ( (this->readBuffer[0] == ebikeStatusArea) && (this->readBuffer[1] == ebikeStatusParameter)) {
        // Now process the read buffer
        this->readBufferToBikeState();
        return true;
      } else {
        LV_LOG_ERROR("valueKey mismatch FAILURE");
      }
    } else {
      LV_LOG_ERROR("ebsReadValueBleCha.readValue FAILURE");
    }
  } else {
    LV_LOG_ERROR("ebsReadKeyBleChar.writeValue FAILURE");
  }
  return false;
}

bool BluetoothBikeController::subscribeEbikeNotifications() {
  if (this->ebsNotifyKeyValueBleCha.canSubscribe() && this->ebsNotifyKeyValueBleCha.subscribe()) {
    this->ebsNotifyKeyValueBleCha.setEventHandler(BLEUpdated, BluetoothBikeController::update_ebs_handler_cb);
    return true;
  }
  return false;
}

bool BluetoothBikeController::subscribeCscNotifications() {
  LV_LOG_USER("subscribeCscNotifications");
  if (this->cscMeasurementBleCha.canSubscribe() && this->cscMeasurementBleCha.subscribe()) {
    LV_LOG_USER("setEventHandler");
    this->cscMeasurementBleCha.setEventHandler(BLEUpdated, BluetoothBikeController::update_csc_handler_cb);
    return true;
  }
  return false;
}

void BluetoothBikeController::updateCscCharacteristicCB(BLEDevice device, BLECharacteristic characteristic) {
  LV_LOG_USER("cscMeasurementBleCha.readValue");
  if (characteristic.readValue(this->readBuffer, 20) > 1) {
    this->readBufferToCscMeasurement();
  }
}

//
// Class callback functions
//

void BluetoothBikeController::updateEbsCharacteristicCB(BLEDevice device, BLECharacteristic characteristic)  {
  // If the characteristic is for e bike service then read the value and process using readBufferToBikeState
  LV_LOG_USER("readBufferToBikeState");
  if (characteristic.readValue(this->readBuffer, 20) > 1) {
    this->readBufferToBikeState();
  }
}

void BluetoothBikeController::updateConnectCB(BLEDevice device)  {
  // No need we should identify if connected
  // this->connected = true;
}

void BluetoothBikeController::updateDisconnectCB(BLEDevice device)  {
  // No need we shoudl identify if disconnected from polling
  //this->connected = false;
}
