#include "BluetoothMaster.h"
#include <ArduinoBLE.h>

#include "Arduino.h"

BluetoothMaster::BluetoothMaster() {
  this->listener_obj = NULL;
  this->scanning = false;
  this->available = false;
}

void BluetoothMaster::setListenerLvObj(lv_obj_t* listener_obj) {
  this->listener_obj = listener_obj;
}

void BluetoothMaster::init() {
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }
  this->scanning = false; 
  this->available = false; 
}

void BluetoothMaster::startScan() {
  BLE.scan();
  this->scanning = true;
  this->available = false;
}

void BluetoothMaster::stopScan() {
  BLE.stopScan();
  this->scanning = false;
  this->available = false;
}

void BluetoothMaster::checkForChange() {
  if (this->scanning && !this->available) {
    this->availableDevice = BLE.available();
    if (this->availableDevice) {
      Serial.print("FOUND ");
      Serial.println(this->availableDevice.hasAdvertisedServiceUuid());
      this->available = true;
      lv_event_send(this->listener_obj, LV_EVENT_REFRESH, &(this->availableDevice));
    }
  }
  
}

BLEDevice BluetoothMaster::getBLEDevice() {
  this->available = false;
  return this->availableDevice;
}