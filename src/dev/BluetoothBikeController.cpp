#include "BluetoothBikeController.h"
#include <ArduinoBLE.h>

#include "Arduino.h"


//
// Static instance of bluetooth controller array
//
std::vector<BluetoothBikeController*> BluetoothBikeController::bluetoothBikeControllers;

//
// Static Handler functions are CB functions for BLE state changes
//
void BluetoothBikeController::connect_handler_cb(BLEDevice device) {
  for (std::vector<BluetoothBikeController *>::iterator it = std::begin(BluetoothBikeController::bluetoothBikeControllers); it != std::end(BluetoothBikeController::bluetoothBikeControllers); ++it) {
    // Check the addresses match to identify the controller is connected to this device
    if ((*it)->connectedBluetoothBike.isBleDevice(device))
      (*it)->updateConnectCB(device);
  }
}

void BluetoothBikeController::disconnect_handler_cb(BLEDevice device) {
  for (std::vector<BluetoothBikeController *>::iterator it = std::begin(BluetoothBikeController::bluetoothBikeControllers); it != std::end(BluetoothBikeController::bluetoothBikeControllers); ++it) {
    // Check the addresses match to identify the controller is connected to this device
    if ((*it)->connectedBluetoothBike.isBleDevice(device))
      (*it)->updateDisconnectCB(device);
  }
}

void BluetoothBikeController::update_ebs_handler_cb(BLEDevice device, BLECharacteristic characteristic) {
  for (std::vector<BluetoothBikeController *>::iterator it = std::begin(BluetoothBikeController::bluetoothBikeControllers); it != std::end(BluetoothBikeController::bluetoothBikeControllers); ++it) {
    // Check the addresses match to identify the controller is connected to this device
    if ((*it)->connectedBluetoothBike.isBleDevice(device))
      (*it)->updateEbsCharacteristicCB(device, characteristic);
  }
}

void BluetoothBikeController::update_csc_handler_cb(BLEDevice device, BLECharacteristic characteristic) {
  for (std::vector<BluetoothBikeController *>::iterator it = std::begin(BluetoothBikeController::bluetoothBikeControllers); it != std::end(BluetoothBikeController::bluetoothBikeControllers); ++it) {
    // Check the addresses match to identify the controller is connected to this device
    if ((*it)->connectedBluetoothBike.isBleDevice(device))
      (*it)->updateCscCharacteristicCB(device, characteristic);
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
  this->connectedBikeStatusLastUpdateTime = 0;
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

//
// Init the bluetooth as we are the controller
//

void BluetoothBikeController::init() {
  if (!BLE.begin()) {
    //Serial.println("starting BLE failed!");
    while (1);
  }
  this->scanning = false; 
  this->scannedDeviceAvailable = false; 
  this->connected = false;
}

//
// Scanning
//

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

//
// Connection
//

bool BluetoothBikeController::connect(BLEDevice& bleDevice) {
  this->stopScan();
  this->scannedDeviceAvailable = false;
  
  // Ensure any existing bluetoothBike is cleaned up
  this->connectedBluetoothBike.setBleDevice(bleDevice);
  this->connectedBluetoothBike.connect();
  this->connected = this->connectedBluetoothBike.isConnected();
  return this->connected;
}

bool BluetoothBikeController::disconnect() {
  if (this->connectedBluetoothBike.isConnected()) {
    this->connected = !(this->connectedBluetoothBike.disconnect());
  } else {
    this->connected = false;
  }
  return !this->connected;
}

//
// Update listener for scanning or connection changes
//

/**
 * @brief Main entry for generic checking the bluetooth for change scanning or connection
 * 
 */
void BluetoothBikeController::checkForChange() {
  this->checkForScanningChange();
  this->checkForConnectionChange();
}

/**
 * @brief Checking if the countroller is scanning more updates listener if there is changes from scanning
 * 
 */
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

/**
 * @brief Checking if the controller is connected to a bike if there are changes with the state of this bike
 * 
 */
void BluetoothBikeController::checkForConnectionChange() {
  // Controller believes the bike is connected
  if (this->connected) {
    // Attempt to poll the device and receive any notifications
    this->connectedBluetoothBike.poll();
    // If we have lost connection then update the controller so this is the final update of state to allow reconnect functionality
    if (!this->connectedBluetoothBike.isConnected()) {
      // Ensure update will occure
      this->connectedBikeStatusLastUpdateTime = 0;
      this->connected = false;
    }

    uint32_t timeStamp = millis();
    if (timeStamp > 60 * 1000) this->checkForStaleBikeStateAttribute(MonitorAttributeType::EVERY_MINUTE, timeStamp - 60 * 1000);
    if (timeStamp > 10 * 1000)  this->checkForStaleBikeStateAttribute(MonitorAttributeType::EVERY_TEN_SECONDS, timeStamp - 10 * 1000);
    if (timeStamp > 1000)  this->checkForStaleBikeStateAttribute(MonitorAttributeType::EVERY_SECOND, timeStamp - 1000);
    if (timeStamp > 20) this->checkForStaleBikeStateAttribute(MonitorAttributeType::CONSTANTLY, timeStamp - 20);

    // If there is a listener and we've got some udpate information for the gui then perform callback
    if (this->listener_obj && this->getConnectedBikeStateUpdated()) {
      lv_event_send(this->listener_obj, LV_EVENT_REFRESH, this);
    }
    this->connectedBikeStatusLastUpdateTime = this->connectedBluetoothBike.getBikeStatusLastUpdateTime();
  }
}


void BluetoothBikeController::checkForStaleBikeStateAttribute(MonitorAttributeType monitorAttributeType, uint32_t maximumTime) {
  // If we have had an update then don't do extra checks for attributes at this time
  if (!this->getConnectedBikeStateUpdated()) {
      BikeStateAttributeIndex bikeStateAttributeIndex = this->getBikeState().getOldestStateAttribute(monitorAttributeType);
    if (bikeStateAttributeIndex != BikeStateAttributeIndex::BIKE_STATE_ATTRIBUTE_SIZE && 
      this->getBikeState().getStateAttribute(bikeStateAttributeIndex).lastFetchTimeTicks < maximumTime) {
        this->connectedBluetoothBike.readBikeStateAttribute(bikeStateAttributeIndex, monitorAttributeType);
    }
  }
}

//
// Class callback functions
//

void BluetoothBikeController::updateCscCharacteristicCB(BLEDevice device, BLECharacteristic characteristic) {
  if (this->connectedBluetoothBike.isBleDevice(device)) {
    this->connectedBluetoothBike.updateCscCharacteristicCB(device, characteristic);
  }
}


void BluetoothBikeController::updateEbsCharacteristicCB(BLEDevice device, BLECharacteristic characteristic)  {
  if (this->connectedBluetoothBike.isBleDevice(device)) {
    this->connectedBluetoothBike.updateEbsCharacteristicCB(device, characteristic);
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
