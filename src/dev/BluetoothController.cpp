#include "BluetoothController.h"
#include <ArduinoBLE.h>

#include "Arduino.h"


//
// Static instance of bluetooth controller array
//
std::vector<BluetoothController*> BluetoothController::bluetoothControllers;

//
// Static Handler functions are CB functions for BLE state changes
//
void BluetoothController::connect_handler_cb(BLEDevice device) {
}

void BluetoothController::disconnect_handler_cb(BLEDevice device) {
}

//
// Constructor
//

BluetoothController::BluetoothController(bool connectionRequiresScanStop) {
    if (!BluetoothController::bluetoothControllers.size()) {
        BLE.setEventHandler(BLEConnected, BluetoothController::connect_handler_cb);
        BLE.setEventHandler(BLEDisconnected, BluetoothController::disconnect_handler_cb);
    }
    BluetoothController::bluetoothControllers.push_back(this);
    this->connectionRequiresScanStop = connectionRequiresScanStop;
}

//
// Distructor
//

BluetoothController::~BluetoothController() {
    // On deleting this control ensure it's removed form the list of known controllers
    std::vector<BluetoothController*>::iterator it = std::find(std::begin(BluetoothController::bluetoothControllers), std::end(BluetoothController::bluetoothControllers), this);
    if (it != BluetoothController::bluetoothControllers.end()) {
        BluetoothController::bluetoothControllers.erase(it);
    }
}

//
// Connection
//

//
// Update listener for scanning or connection changes
//

//
// Class callback functions
//

void BluetoothController::updateConnectCB(BLEDevice device) {
    // No need we should identify if connected
    // this->connected = true;
}

void BluetoothController::updateDisconnectCB(BLEDevice device) {
    // No need we shoudl identify if disconnected from polling
    //this->connected = false;
}
