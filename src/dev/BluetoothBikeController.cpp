#include "BluetoothBikeController.h"
#include <ArduinoBLE.h>

#include "Arduino.h"

BluetoothBikeController::BluetoothBikeController() {
  this->listener_obj = NULL;
  this->scanning = false;
  this->available = false;
  this->connected = false;
}

void BluetoothBikeController::setListenerLvObj(lv_obj_t* listener_obj) {
  this->listener_obj = listener_obj;
}

void BluetoothBikeController::init() {
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }
  this->scanning = false; 
  this->available = false; 
}

bool BluetoothBikeController::startScan() {
  // Scans are not possible if already connected to the device
  if (this->connected) return false;
  if (this->scanning) return true;

  BLE.scan();
  this->scanning = true;
  this->available = false;
  return true;
}

bool BluetoothBikeController::continueScan() {
    // Scans are not possible if already connected to the device
    if (this->connected) return false;

    this->available = false;
    return true;
}

bool BluetoothBikeController::stopScan() {
  // Scans are not possible if already connected to the device
  if (this->connected) return false;
  if (!this->scanning) return true;

  BLE.stopScan();
  this->scanning = false;
  this->available = false;
  return true;
}

bool BluetoothBikeController::connect(BLEDevice* bleDevice) {
  if (this->connected) return true;
  this->stopScan();
  
  this->available = false;
  this->availableDevice = *bleDevice;
  if (this->availableDevice.connect()) {
    if (this->availableDevice.discoverAttributes()) {
      this->connectedBikeType = NONE;
      if (this->availableDevice.hasService(UUID_SPECIALIZED_READ_SERVICE_GEN1)) {
        this->connectedBikeType = GEN1;
        this->ebsReadKeyBleCha = this->availableDevice.characteristic(UUID_SPECIALIZED_READ_KEY_CHARACTERISTIC_GEN1);
        this->ebsReadValueBleCha = this->availableDevice.characteristic(UUID_SPECIALIZED_READ_VALUE_CHARACTERISTIC_GEN1);
        this->ebsWriteKeyValueBleCha = this->availableDevice.characteristic(UUID_SPECIALIZED_WRITE_KEY_VALUE_CHARACTERISTIC_GEN1);
        this->ebsNotifyKeyValueBleChar = this->availableDevice.characteristic(UUID_SPECIALIZED_NOTIFY_KEY_VALUE_CHARACTERISTIC_GEN1);
      }
      if (this->availableDevice.hasService(UUID_SPECIALIZED_READ_SERVICE_GEN2)) {
        this->connectedBikeType = GEN2;
        this->ebsReadKeyBleCha = this->availableDevice.characteristic(UUID_SPECIALIZED_READ_KEY_CHARACTERISTIC_GEN2);
        this->ebsReadValueBleCha = this->availableDevice.characteristic(UUID_SPECIALIZED_READ_VALUE_CHARACTERISTIC_GEN2);
        this->ebsWriteKeyValueBleCha = this->availableDevice.characteristic(UUID_SPECIALIZED_WRITE_KEY_VALUE_CHARACTERISTIC_GEN2);
        this->ebsNotifyKeyValueBleChar = this->availableDevice.characteristic(UUID_SPECIALIZED_NOTIFY_KEY_VALUE_CHARACTERISTIC_GEN2);
      }
      if (this->availableDevice.hasService(UUID_CYCLE_SERVICE)) {
        this->cscMeasurementBleCha = this->availableDevice.characteristic(UUID_CSC_MEASUREMENT_CHARACTERISTIC);
        this->cscFeatureBleCha = this->availableDevice.characteristic(UUID_CSC_FEATURE_CHARACTERISTIC);
        this->scControlPointBleCha = this->availableDevice.characteristic(UUID_SC_CONTROL_POINT_CHARACTERISTIC);
      }
      this->connected = true;
      return true;
    }
    this->availableDevice.disconnect();
  }
  // Unable to make connection
  this->connected = false;  
  return false;
}

bool BluetoothBikeController::disconnect() {
  if (this->connected) {
    this->connected = false;
    return this->availableDevice.disconnect();
  }
  return true;
}

void BluetoothBikeController::checkForChange() {
  if (this->scanning && !this->available) {
    this->availableDevice = BLE.available();
    if (this->availableDevice) {
      this->available = true;
      lv_event_send(this->listener_obj, LV_EVENT_REFRESH, &(this->availableDevice));
    }
  }
}

BLEDevice BluetoothBikeController::getBLEDevice() {
  if (this->available) {
    return this->availableDevice;
  }
  return BLEDevice();
}

BikeType BluetoothBikeController::getConnectedBikeType() {
  if (this->connected) {
    return this->connectedBikeType;
  }  
  return NONE;
}

void BluetoothBikeController::readBatteryCapacity() {
  if (this->readEbikeValue(BATTERY, CAPACITY)) {
    this->connectedBikeState.BatteryCapacity = this->bufferToUint16();
  }
}

void BluetoothBikeController::readBatteryCapacityRemaining() {
  if (this->readEbikeValue(BATTERY, CAPACITY_REMAIN)) {
    this->connectedBikeState.BatteryCapacityRemaining = this->bufferToUint16();
  }
}

void BluetoothBikeController::readBatterySerialNumber() {
  if (this->readEbikeValue(BATTERY, SERIAL_NO)) {
    this->connectedBikeState.BatterySerialNumber = this->bufferToUint16();
  }
}

void BluetoothBikeController::readBatteryChargePercent() {
  if (this->readEbikeValue(BATTERY, CHARGE_PERCENT)) {
    this->connectedBikeState.BatteryChargePercent = this->bufferToUint8();
  }
}

void BluetoothBikeController::readBatteryHealth() {
  if (this->readEbikeValue(BATTERY, HEALTH)) {
    this->connectedBikeState.BatteryHealth = this->bufferToUint8();
  }
}

void BluetoothBikeController::readBatteryTemp() {
  if (this->readEbikeValue(BATTERY, TEMP)) {
    this->connectedBikeState.BatteryTemp = this->bufferToUint8();
  }
}

void BluetoothBikeController::readBatteryChargeCycles() {
  if (this->readEbikeValue(BATTERY, TEMP)) {
    this->connectedBikeState.BatteryChargeCycles = this->bufferToUint16();
  }
}

void BluetoothBikeController::readBatteryVoltage() {
  if (this->readEbikeValue(BATTERY, VOLTAGE)) {
    this->connectedBikeState.BatteryVoltage = this->bufferToUint8();
  }
}

void BluetoothBikeController::readBatteryCurrent() {
  if (this->readEbikeValue(BATTERY, CURRENT)) {
    this->connectedBikeState.BatteryCurrent = this->bufferToUint8();
  }
}

void BluetoothBikeController::readBatteryHardwareVersion() {
  if (this->readEbikeValue(BATTERY, HARDWARE_VERSION)) {
    this->connectedBikeState.BatteryHardwareVersion = this->bufferToUint8();
  }
}

void BluetoothBikeController::readBatteryFirmwareVersion() {
  if (this->readEbikeValue(BATTERY, FIRMWARE_VERSION)) {
    this->connectedBikeState.BatteryFirmwareVersion = this->bufferToUint8();
  }
}

bool BluetoothBikeController::readEbikeValue(int ebikeStatusArea, int ebikeStatusParameter) {
  // int32 requires the bytes in this order (due to endian conversion)
  uint16_t key = ebikeStatusParameter << 8 + ebikeStatusArea;
  if (this->ebsReadKeyBleCha.writeValue(key)) {
    // Ensure we've at least read the key back
    if (this->ebsReadValueBleCha.readValue(this->readBuffer, 20) > 1) {
      if ( (this->readBuffer[0] == ebikeStatusArea) && (this->readBuffer[1] == ebikeStatusParameter)) {
        return true;
      }
    }
  }
  return false;
}

