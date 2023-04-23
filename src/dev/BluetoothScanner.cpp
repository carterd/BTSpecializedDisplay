#include "BluetoothScanner.h"

BluetoothScanner::BluetoothScanner() {
    this->listener_obj = NULL;
    this->scanning = false;
    this->scannedDeviceAvailable = false;
}

BluetoothScanner::~BluetoothScanner() {
    // Not expected to delete the scanner, it's always going to be around
}

bool BluetoothScanner::startScan() {
    // Scans are not possible if already connected to the device
    if (this->scanning) return true;

    BLE.scan();
    this->scanning = true;
    this->scannedDeviceAvailable = false;
    return true;
}

bool BluetoothScanner::continueScan() {
    // Scans are not possible if already connected to the device
    //if (this->connected) return false;

    this->scannedDeviceAvailable = false;
    return true;
}

bool BluetoothScanner::stopScan() {
    // Scans are not possible if already connected to the device
    if (!this->scanning) return true;

    BLE.stopScan();
    this->scanning = false;
    this->scannedDeviceAvailable = false;
    return true;
}

/**
 * @brief Main entry for generic checking the bluetooth for change scanning or connection
 * 
 */
void BluetoothScanner::checkForChange() {
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
