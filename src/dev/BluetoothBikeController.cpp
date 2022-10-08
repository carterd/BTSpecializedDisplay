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

void BluetoothBikeController::update_handler_cb(BLEDevice device, BLECharacteristic characteristic) {
  for (std::vector<BluetoothBikeController *>::iterator it = std::begin(BluetoothBikeController::bluetoothBikeControllers); it != std::end(BluetoothBikeController::bluetoothBikeControllers); ++it) {
    // Check the addresses match to identify the controller is connected to this device
    if ((*it)->getConnected() && device == (*it)->getConnectedDevice()) {
      (*it)->updateCharacteristicCB(device, characteristic);
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

void BluetoothBikeController::checkForConnectionChange() {
  if (this->connected) {
    // Attempt to poll the device
    this->connectedDevice.poll();
    // If we have lost connection then update the controller so this is the final update of state to allow reconnect functionality
    if (!this->connectedDevice.connected()) {
      this->connected = false;
      this->connectedBikeStateUpdated = true;
    }
    // Currently we can't tell if the poll has detected any changes soo simply keep REFRESHING every 20ms
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
    BluetoothBikeRequest bluetoothBikeRequest = this->bikeStateToBluetoothBikeRequest.getBluetoothBikeRequest(bikeStateAttributeIndex, this->getConnectedBikeType());
    result = this->readEbikeValue(bluetoothBikeRequest.request1.area, bluetoothBikeRequest.request1.attribute);
    if (result && bluetoothBikeRequest.request2.area != EbikeStatusArea::INVALID) {
        result = this->readEbikeValue(bluetoothBikeRequest.request2.area, bluetoothBikeRequest.request2.attribute);
    }
    
    return result;
}

bool BluetoothBikeController::readBattery(MonitorAttributeType monitorAttributeType) {
  if (readBatteryCapacity(monitorAttributeType) && readBatteryCapacityRemaining(monitorAttributeType) && readBatteryChargeCycles(monitorAttributeType) && readBatteryChargePercent(monitorAttributeType) && readBatteryHealth(monitorAttributeType) &&
	    readBatteryTemp(monitorAttributeType) && readBatteryVoltage(monitorAttributeType) && readBatteryCurrent(monitorAttributeType) && readBatteryFirmwareVersion(monitorAttributeType) && readBatteryHardwareVersion(monitorAttributeType) && readBatterySerialNumber(monitorAttributeType))
  {
    return true;
  }
  return false;
}

bool BluetoothBikeController::readMotor(MonitorAttributeType monitorAttributeType) {
  if (readRiderPower(monitorAttributeType) && readMotorCadence(monitorAttributeType) && readMotorSpeed(monitorAttributeType) && readMotorOdometer(monitorAttributeType) && readMotorAssistLevel(monitorAttributeType) && 
      readMotorFirmwareVersion(monitorAttributeType) && readMotorTemp(monitorAttributeType) && readMotorPower(monitorAttributeType) && readPeakAssistLevels(monitorAttributeType) && readMotorSerailNumber(monitorAttributeType))
  {
    return true;
  }
  return false;
}

bool BluetoothBikeController::readBikeOther(MonitorAttributeType monitorAttributeType) {
  if (readBikeOnOffState(monitorAttributeType) && readBikeSerialNumber(monitorAttributeType) && readWheelCircumference(monitorAttributeType) && readAssistPercentages(monitorAttributeType) )
  {
    return true;
  }
  return false;
}

bool BluetoothBikeController::readBatteryHardwareVersion(MonitorAttributeType monitorAttributeType) {
  this->connectedBikeState.batteryHardwareVersion.setMonitorAttributeType(monitorAttributeType); 
  switch (this->connectedBikeType)
  {
    case BikeType::GEN1: return this->readEbikeValue(EbikeStatusArea::BATTERY, EbikeStatusBattery::HARDWARE_VERSION); break;
    case BikeType::GEN2: break;
  }
  return false;
}

bool BluetoothBikeController::readBatterySerialNumber(MonitorAttributeType monitorAttributeType) {
  this->connectedBikeState.batterySerialNumber.setMonitorAttributeType(monitorAttributeType); 
  switch (this->connectedBikeType)
  {
    case BikeType::GEN1: return this->readEbikeValue(EbikeStatusArea::BATTERY, EbikeStatusBattery::SERIAL_NO_GEN1); break;
    case BikeType::GEN2: break;
  }
  return false;
}

bool BluetoothBikeController::readMotorSerailNumber(MonitorAttributeType monitorAttributeType) {
  this->connectedBikeState.motorSerailNumber.setMonitorAttributeType(monitorAttributeType); 
  if (this->readEbikeValue(EbikeStatusArea::MOTOR, EbikeStatusMotor::SERIAL_NO_PART1) &&
    this->readEbikeValue(EbikeStatusArea::MOTOR, EbikeStatusMotor::SERIAL_NO_PART2)) return true;
  return false;
}

bool BluetoothBikeController::readAssistPercentages(MonitorAttributeType monitorAttributeType) {
  this->connectedBikeState.assistPercentage.setMonitorAttributeType(monitorAttributeType); 
  if (this->readEbikeValue(EbikeStatusArea::OTHER, EbikeStatusOther::ASSIST_ECO) && 
    this->readEbikeValue(EbikeStatusArea::OTHER, EbikeStatusOther::ASSIST_TRAIL) &&
    this->readEbikeValue(EbikeStatusArea::OTHER, EbikeStatusOther::ASSIST_TURBO)) return true;
  return false;
}

bool BluetoothBikeController::readBatteryConnected(MonitorAttributeType monitorAttributeType) {
  this->connectedBikeState.bikeSerialNumber.setMonitorAttributeType(monitorAttributeType); 
  if (this->readBikeSerialNumber()) {
    uint32_t time = millis();
    this->connectedBikeState.batteryConnected.setValueAndTime((readBuffer[2] == 0xff && readBuffer[3] == 0xff && readBuffer[4] == 0xff && readBuffer[5]) ? false : true, time);
    return true;
  }
  return false;
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
        case EbikeStatusBattery::CAPACITY:         bikeStatusUpdate = this->connectedBikeState.batteryCapacity.setValueAndTime(this->bufferToUint16(), time); break;
        case EbikeStatusBattery::CAPACITY_REMAIN:  bikeStatusUpdate = this->connectedBikeState.batteryCapacityRemaining.setValueAndTime(this->bufferToUint16(), time); break;
        case EbikeStatusBattery::HEALTH:           bikeStatusUpdate = this->connectedBikeState.batteryHealth.setValueAndTime(this->bufferToUint8(), time); break;
        case EbikeStatusBattery::TEMP:             bikeStatusUpdate = this->connectedBikeState.batteryTemp.setValueAndTime(this->bufferToUint8(), time); break;
        case EbikeStatusBattery::CHARGE_CYCLES:    bikeStatusUpdate = this->connectedBikeState.batteryChargeCycles.setValueAndTime(this->bufferToUint16(), time); break;
        case EbikeStatusBattery::VOLTAGE:          bikeStatusUpdate = this->connectedBikeState.batteryVoltage.setValueAndTime(this->bufferToUint8() / 5.0f + 20, time); break;
        case EbikeStatusBattery::CURRENT:          bikeStatusUpdate = this->connectedBikeState.batteryCurrent.setValueAndTime(this->bufferToUint8() / 5.0f, time); break;
        case EbikeStatusBattery::HARDWARE_VERSION: {            
            // Possible code with GEN2
            HardwareVersion hardwareVersion { this->bufferToUint8(), 0 };
            bikeStatusUpdate = this->connectedBikeState.batteryHardwareVersion.setValueAndTime(hardwareVersion, time);
          } break;
        case EbikeStatusBattery::FIRMWARE_VERSION: {
            FirmwareVersion firmwareVersion { this->bufferToUint8(BATTERY_FIRMWARE_VERSION_MAJOR_NUMBER_BUFFER_INDEX), 
                                              this->bufferToUint8(BATTERY_FIRMWARE_VERSION_MINOR_NUMBER_BUFFER_INDEX),
                                              this->bufferToUint8(BATTERY_FIRMWARE_VERSION_PATCH_NUMBER_BUFFER_INDEX) };
            bikeStatusUpdate = this->connectedBikeState.batteryFirmwareVersion.setValueAndTime(firmwareVersion, time);
          } break;
        case EbikeStatusBattery::SERIAL_NO_GEN1:  bikeStatusUpdate = this->connectedBikeState.batterySerialNumber.setValueAndTime(this->bufferToUint16(), time); break;
        case EbikeStatusBattery::CHARGE_PERCENT:  bikeStatusUpdate = this->connectedBikeState.batteryChargePercent.setValueAndTime(this->bufferToUint8(), time); break;
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
        case EbikeStatusMotor::RIDER_POWER:   bikeStatusUpdate = this->connectedBikeState.riderPower.setValueAndTime(this->bufferToUint16(), time); break;
        case EbikeStatusMotor::CADENCE:       bikeStatusUpdate = this->connectedBikeState.motorCadence.setValueAndTime(this->bufferToUint16() / 10.0f, time); break;
        case EbikeStatusMotor::SPEED:         bikeStatusUpdate = this->connectedBikeState.motorSpeed.setValueAndTime(this->bufferToUint16() / 10.0f, time); break;
        case EbikeStatusMotor::ODOMETER:      bikeStatusUpdate = this->connectedBikeState.motorOdometer.setValueAndTime(this->bufferToUint32(), time); break;
        case EbikeStatusMotor::ASSIST_LEVEL:  bikeStatusUpdate = this->connectedBikeState.motorAssistLevel.setValueAndTime(this->bufferToUint16(), time); break;
        case EbikeStatusMotor::TEMP:          bikeStatusUpdate = this->connectedBikeState.riderPower.setValueAndTime(this->bufferToUint8(), time); break;
        case EbikeStatusMotor::MOTOR_POWER:   bikeStatusUpdate = this->connectedBikeState.riderPower.setValueAndTime(this->bufferToUint16(), time); break;
        case EbikeStatusMotor::FIRMWARE_VERSION: {
          FirmwareVersion firmwareVersion { this->bufferToUint8(MOTOR_FIRMWARE_VERSION_MAJOR_NUMBER_BUFFER_INDEX), 
                                            this->bufferToUint8(MOTOR_FIRMWARE_VERSION_MINOR_NUMBER_BUFFER_INDEX), 
                                            this->bufferToUint8(MOTOR_FIRMWARE_VERSION_PATCH_NUMBER_BUFFER_INDEX) }; 
          bikeStatusUpdate = this->connectedBikeState.motorFirmwareVersion.setValueAndTime(firmwareVersion, time);
        } break;
        case EbikeStatusMotor::PEAK_ASSIST:
          switch (this->connectedBikeType) {
            case BikeType::GEN1: {
              AssistLevels assistLevels = this->connectedBikeState.peakAssistLevels.getValue(); 
              uint8_t peakTypeAndValue = this->bufferToUint8();
              uint8_t peakType = peakTypeAndValue % 10;
              uint8_t peakLevel = peakTypeAndValue - peakType;
              switch (peakType) {
                case 1: assistLevels.eco=peakLevel; break;
                case 2: assistLevels.trail=peakLevel; break;
                case 3: assistLevels.turbo=peakLevel; break;
              }
              bikeStatusUpdate = this->connectedBikeState.peakAssistLevels.setValueAndTime(assistLevels, time);
            } break;
            case BikeType::GEN2: {
              AssistLevels assistLevels = this->connectedBikeState.peakAssistLevels.getValue(); 
              assistLevels.eco=this->bufferToUint8(2);
              assistLevels.trail=this->bufferToUint8(3);
              assistLevels.turbo=this->bufferToUint8(4);
              bikeStatusUpdate = this->connectedBikeState.peakAssistLevels.setValueAndTime(assistLevels, time);
            } break;
          } break;
        case EbikeStatusMotor::HARDWARE_NO_PART1: {
            char* motorHardwareNumber = this->connectedBikeState.motorHardwareNumber.getValue().numberString;
            bufferToCharArray(&(motorHardwareNumber[0]), 18);
            motorHardwareNumber[20] = 0x00;
            bikeStatusUpdate = this->connectedBikeState.motorHardwareNumber.setTime(time);
          } break;
        case EbikeStatusMotor::HARDWARE_NO_PART2: {
            char* motorHardwareNumber = this->connectedBikeState.motorHardwareNumber.getValue().numberString;
            bufferToCharArray(&(motorHardwareNumber[18]), 2);
            motorHardwareNumber[20] = 0x00;
            bikeStatusUpdate = this->connectedBikeState.motorHardwareNumber.setTime(time);
          } break;
        case EbikeStatusMotor::SERIAL_NO_PART1: {
            char* motorSerailNumber = this->connectedBikeState.motorSerailNumber.getValue().numberString;
            bufferToCharArray(&(motorSerailNumber[0]), 18);
            motorSerailNumber[23] = 0x00;
            bikeStatusUpdate = this->connectedBikeState.motorSerailNumber.setTime(time);
          } break;
        case EbikeStatusMotor::SERIAL_NO_PART2: {
            char* motorSerailNumber = this->connectedBikeState.motorSerailNumber.getValue().numberString;
            bufferToCharArray(&(motorSerailNumber[18]), 5);
            motorSerailNumber[23] = 0x00;
            bikeStatusUpdate = this->connectedBikeState.motorSerailNumber.setTime(time);
          } break;
      }
    } break;
    case EbikeStatusArea::OTHER: {
      EbikeStatusOther ebikeStatusOther = static_cast<EbikeStatusOther>(this->readBuffer[1]);
      switch (ebikeStatusOther) {
        case EbikeStatusOther::WHEEL_CIRCUMFERENCE: bikeStatusUpdate = this->connectedBikeState.wheelCircumference.setValueAndTime(this->bufferToUint16(), time); break;
        case EbikeStatusOther::ASSIST_ECO: {
          AssistLevels assistLevels = this->connectedBikeState.assistPercentage.getValue();
          assistLevels.eco = this->bufferToUint8();
          bikeStatusUpdate = this->connectedBikeState.assistPercentage.setValueAndTime(assistLevels, time);
        } 
        break;
        case EbikeStatusOther::ASSIST_TRAIL: {
          AssistLevels assistLevels = this->connectedBikeState.assistPercentage.getValue();
          assistLevels.trail = this->bufferToUint8();
          bikeStatusUpdate = this->connectedBikeState.assistPercentage.setValueAndTime(assistLevels, time);
        } 
        break;
        case EbikeStatusOther::ASSIST_TURBO: {
          AssistLevels assistLevels = this->connectedBikeState.assistPercentage.getValue();
          assistLevels.turbo = this->bufferToUint8();
          bikeStatusUpdate = this->connectedBikeState.assistPercentage.setValueAndTime(assistLevels, time);
        } 
        break;
        case EbikeStatusOther::BIKE_ON_OFF:         bikeStatusUpdate = this->connectedBikeState.bikeOnOffState.setValueAndTime(this->bufferToUint8(), time); break;
        case EbikeStatusOther::BIKE_SERIAL_NO:
          {
            char* bikeSerailNumber = this->connectedBikeState.bikeSerialNumber.getValue().numberString;
            bufferToCharArray(&(bikeSerailNumber[0]), 17);
            bikeSerailNumber[17] = 0x00;
            bikeStatusUpdate = this->connectedBikeState.bikeSerialNumber.setTime(time);
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
    this->ebsNotifyKeyValueBleCha.setEventHandler(BLEUpdated, BluetoothBikeController::update_handler_cb);
    return true;
  }
  return false;
}

//
// Class callback functions
//

void BluetoothBikeController::updateCharacteristicCB(BLEDevice device, BLECharacteristic characteristic)  {
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
