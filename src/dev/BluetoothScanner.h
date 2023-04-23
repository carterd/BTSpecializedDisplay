#ifndef _BLUETOOTH_SCANNER_H
#define _BLUETOOTH_SCANNER_H

#include <lvgl.h>
#include <ArduinoBLE.h>

/**
 * @brief This represents scanner wrapper for obtaining BLEDevices from the BLE library.
 * 
 * Searches for BLEDevices that could be instances of BluetoothBikes.
 * 
 */
class BluetoothScanner
{
protected:
	/// <summary>
	/// This is the lv object which has to be infromed of updates to say scanning
	/// </summary>
	lv_obj_t* listener_obj;

	/// <summary>
	/// True when the bluetooth scan is active
	/// </summary>
	bool scanning;
    
	/// <summary>
	/// True when there is a bluetooth device available for connection
	/// </summary>
	bool scannedDeviceAvailable;
    
	/// <summary>
	/// The available device detected by a scan
	/// </summary>
	BLEDevice scannedDevice;

public:
	/// <summary>
	/// Constructor of the bluetooth controller object
	/// </summary>
	BluetoothScanner();

	/// <summary>
	/// Distructor of the bluetooth controller object
	/// </summary>
	~BluetoothScanner();
    
	/// <summary>
	/// Set the current LV object that is required to be updated with changes in the bluetooth controller
	/// </summary>
	/// <param name="listener_obj">lv_object to be sent refesh events on changes to BT scan/connection</param>
	void setListenerLvObj(lv_obj_t* listener_obj) { this->listener_obj = listener_obj; }

	/// <summary>
	/// Returns true if there is a scanned device available for processing
	/// </summary>
	/// <returns>True if the scan has a scanned device available for processing</returns>
	bool getScannedDeviceAvailable() { return this->scannedDeviceAvailable; }

	/// <summary>
	/// Returns the currently scanned BT device
	/// </summary>
	/// <returns>The currently scanned BT device</returns>
	BLEDevice getScannedDevice() { if (this->scannedDeviceAvailable) { return this->scannedDevice; } else { return BLEDevice(); } }

    /// <summary>
	/// Starts scanning for BT devices
	/// </summary>
	/// <returns>True if the scan was successfully started</returns>
	bool startScan();

	/// <summary>
	/// Stop scanning for BT devices
	/// </summary>
	/// <returns>True if the scan was successfully stopped</returns>
	bool stopScan();

	/// <summary>
	/// Resume scanning for BT devices, on detecting a device the scanning is paused.
	/// </summary>
	/// <returns>True if the scan was successfully restarted</returns>
	bool continueScan();

	/// <summary>
	/// This should be called periodically to update the listener object on changes to the controller
	/// Identify if any new devices have been found when in scanning mode and hence requires such changes to be processed by 
	/// the current lvgl listener object via LV_EVENT_REFRESH event.
	/// </summary>
	void checkForChange();
};

#endif