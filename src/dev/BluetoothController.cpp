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

BluetoothController::BluetoothController(BluetoothBike* bluetoothBike, BluetoothHeartRateMonitor* bluetoothHeartRateMonitor, BluetoothScanner* bluetoothScanner, bool connectionRequiresScanStop) {
    if (!BluetoothController::bluetoothControllers.size()) {
        BLE.setEventHandler(BLEConnected, BluetoothController::connect_handler_cb);
        BLE.setEventHandler(BLEDisconnected, BluetoothController::disconnect_handler_cb);
    }
    BluetoothController::bluetoothControllers.push_back(this);
    this->bluetoothBike = bluetoothBike;
    this->bluetoothHeartRateMonitor = bluetoothHeartRateMonitor;
    this->bluetoothScanner = bluetoothScanner;
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

void BluetoothController::startConnections(lv_obj_t* listener_obj) {
    this->bluetoothScanner->setListenerLvObj(listener_obj);
    this->bluetoothBike->setListenerLvObj(listener_obj);
    this->bluetoothHeartRateMonitor->setListenerLvObj(listener_obj);

    if (this->getMultipleBtConnectionsAllowed()) {
        if (this->bluetoothBike->isConnected()) return;
    } else {
        if (this->bluetoothBike->isConnected() && this->bluetoothHeartRateMonitor->isConnected()) return;
    }

    this->bluetoothScanner->startScan();
}

void BluetoothController::stopConnections() {
    this->bluetoothScanner->stopScan();
    this->bluetoothBike->disconnect();
    this->bluetoothHeartRateMonitor->disconnect();
}

void BluetoothController::stopScanning() {
    this->bluetoothScanner->stopScan();
}

bool BluetoothController::checkForConnections(BTAddressesConfig* btAddressesConfig, bool connectBatteryOnly) {
    // Check for making connections
    if (this->bluetoothScanner->isScanning()) {
        BLEDevice bleDevice;
        if (this->bluetoothScanner->getScannedDeviceAvailable()) 
            bleDevice = this->bluetoothScanner->getScannedDevice();
        if (!this->bluetoothBike->isConnected()) {
            if (bleDevice) {
                if (btAddressesConfig->containsBTAddress(bleDevice.address().c_str())) {
                    if (this->getConnectionRequiresScanStop()) { 
                        this->bluetoothScanner->stopScan(); 
                    }
                    if (!(this->bluetoothHeartRateMonitor->isConnected() && this->bluetoothHeartRateMonitor->isBleDevice(bleDevice)) && this->bluetoothBike->connect(bleDevice)) {
                        switch (this->bluetoothBike->getBikeType())
                        {
                            case BikeType::GEN1:
                            case BikeType::GEN2:
                                if (this->bluetoothBike->readBikeStateAttribute(BikeStateAttributeIndex::BATTERY_CONNECTED_STATE)
                                    && (this->bluetoothBike->getBikeState().getStateAttribute(BikeStateAttributeIndex::BATTERY_CONNECTED_STATE).bikeStateAttributeValue.valueBool
                                    || connectBatteryOnly))
                                {
                                    return true;
                                }
                            break;
                        }
                        this->bluetoothBike->disconnect();
                    }
                    this->bluetoothScanner->startScan();
                }
            }
        }
        if (this->getMultipleBtConnectionsAllowed()) {
            if (!this->bluetoothHeartRateMonitor->isConnected()) {
                if (bleDevice) {
                    if (btAddressesConfig->containsBTAddress(bleDevice.address().c_str())) {
                        if (!(this->bluetoothBike->isConnected() && this->bluetoothBike->isBleDevice(bleDevice)) && this->bluetoothHeartRateMonitor->connect(bleDevice)) {
                            return true;
                        }
                    }
                }
            }
        }
        // Identify if we are still scanning
        if (this->bluetoothScanner->getScannedDeviceAvailable()) {
            this->bluetoothScanner->continueScan();
        }
        if (this->getMultipleBtConnectionsAllowed()) {
            if (this->bluetoothBike->isConnected() && this->bluetoothHeartRateMonitor->isConnected()) {
                this->bluetoothScanner->stopScan();
            }
        } else {
            if (this->bluetoothBike->isConnected()) { 
                this->bluetoothScanner->stopScan(); 
            }
        }
    }
    return false;
}

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

