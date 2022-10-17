#ifndef _BLUETOOTH_BIKE_CONTROLLER_H
#define _BLUETOOTH_BIKE_CONTROLLER_H

#include <lvgl.h>
#include <ArduinoBLE.h>
#include <vector>
#include <algorithm>

#include "BluetoothBikeDefinitions.h"
#include "BluetoothBikeState.h"
#include "BluetoothBike.h"

#define BATTERY_FIRMWARE_VERSION_MAJOR_NUMBER_BUFFER_INDEX 3
#define BATTERY_FIRMWARE_VERSION_MINOR_NUMBER_BUFFER_INDEX 2
#define BATTERY_FIRMWARE_VERSION_PATCH_NUMBER_BUFFER_INDEX 4

#define MOTOR_FIRMWARE_VERSION_MAJOR_NUMBER_BUFFER_INDEX 3
#define MOTOR_FIRMWARE_VERSION_MINOR_NUMBER_BUFFER_INDEX 2
#define MOTOR_FIRMWARE_VERSION_PATCH_NUMBER_BUFFER_INDEX 4

/**
 * @brief This represents the interface to the bike and is a wrapper for BLEDevice interface to the bikes
 * 
 */
class BluetoothBikeController
{

private:
	/// <summary>
	/// This is helpful on processing handlers which will have to find the Controller the update is associated with
	/// </summary>
	static std::vector<BluetoothBikeController*> bluetoothBikeControllers;
public:
	/// <summary>
	/// This is the updateHandler static method
	/// </summary>
	static void update_csc_handler_cb(BLEDevice device, BLECharacteristic characteristic);
	static void update_ebs_handler_cb(BLEDevice device, BLECharacteristic characteristic);
	static void connect_handler_cb(BLEDevice device);
	static void disconnect_handler_cb(BLEDevice device);
private:
	/// <summary>
	/// Copy the contents of the readBuffer into a given destination
	/// </summary>
	/// <param name="dst">Destination to copy the content of the read buffer</param>
	/// <param name="length">The number of characters to copy to destination</param>
	/// <param name="offset">The offset in the Bluetooth readBuffer</param>
	void bufferToCharArray(void* dst, int length, int offset = 2) { memcpy(dst, &(this->readBuffer[offset]), length); }

	/// <summary>
	/// Extract a 32bit value read into the bluetooth readBuffer
	/// </summary>
	/// <param name="offset">The offset in the Bluetooth readBuffer</param>
	/// <return>The 32bit value read from the bluetooth readBuffer</return>
	uint32_t bufferToUint32(int offset = 2) { return (uint32_t)(this->readBuffer[offset]) + ((uint32_t)(this->readBuffer[offset + 1]) << 8) + ((uint32_t)(this->readBuffer[offset + 2]) << 16) + ((uint32_t)(this->readBuffer[offset + 3]) << 24); }

	/// <summary>
	/// Extract a 16bit value read into the bluetooth readBuffer
	/// </summary>
	/// <param name="offset">The offset in the Bluetooth readBuffer</param>
	/// <return>The 32bit value read from the bluetooth readBuffer</return>
	uint16_t bufferToUint16(int offset = 2) { return (uint16_t)(this->readBuffer[offset]) + ((uint16_t)(this->readBuffer[offset + 1]) << 8); }

	/// <summary>
	/// Extract a 8bit value read into the bluetooth readBuffer
	/// </summary>
	/// <param name="offset">The offset in the Bluetooth readBuffer</param>
	/// <return>The 32bit value read from the bluetooth readBuffer</return>
	uint8_t bufferToUint8(int offset = 2) { return this->readBuffer[offset]; }

	/// <summary>
	/// The bluetooth read-buffer that maybe required to be converted into connectedBikeStatus entries.
	/// </summary>
	uint8_t readBuffer[20];

	void checkForScanningChange();

	void checkForConnectionChange();

	void checkForStaleBikeStateAttribute(MonitorAttributeType monitorAttributeType, uint32_t maximumTime);

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
	/// True when there is a bluetooth device is connected
	/// </summary>
	bool connected;	

	/// <summary>
	/// A time stamp used to track if connected bike status has changed
	/// </summary>
	unsigned long connectedBikeStatusLastUpdateTime;

	/// <summary>
	/// The available device detected by a scan
	/// </summary>
	BLEDevice scannedDevice;

	/// <summary>
	/// The bluetooth bike instance 
	/// </summary>
	BluetoothBike connectedBluetoothBike;

	/// <summary>
	/// The available device detected by a scan or used as the device in connections
	/// </summary>
	//BLEDevice connectedDevice;

	/// <summary>
	/// This is the BLE Characteristic for CSC Measurement 
	/// </summary>
	//BLECharacteristic cscMeasurementBleCha;

	/// <summary>
	/// This is the BLE Characteristic for CSC Feature
	/// </summary>
	//BLECharacteristic cscFeatureBleCha;

	/// <summary>
	/// This is the BLE Characteristic for SC Control Point
	/// </summary>
	//BLECharacteristic scControlPointBleCha;

	/// <summary>
	/// This is the BLE Characteristic for Ebike Specialized Read Key
	/// </summary>
	//BLECharacteristic ebsReadKeyBleCha;

	/// <summary>
	/// This is the BLE Characteristic for Ebike Specialized Read Value
	/// </summary>
	//BLECharacteristic ebsReadValueBleCha;

	/// <summary>
	/// This is the BLE Characteristic for Ebike Specialized Write Key-Value
	/// </summary>
	//BLECharacteristic ebsWriteKeyValueBleCha;

	/// <summary>
	/// This is the BLE Characteristic for Ebike Specialized Notify Key Value 
	/// </summary>
	//BLECharacteristic ebsNotifyKeyValueBleCha;
protected:
	void updateCscCharacteristicCB(BLEDevice device, BLECharacteristic characteristic);
	void updateEbsCharacteristicCB(BLEDevice device, BLECharacteristic characteristic);
	void updateConnectCB(BLEDevice device);
	void updateDisconnectCB(BLEDevice device);

public:
	/// <summary>
	/// Constructor of the bluetooth controller object
	/// </summary>
	BluetoothBikeController();

	~BluetoothBikeController();

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
	/// Returns the currently connected device or blank device if no connection</return>
	/// </summary>
	/// <returns>The currently connected device or blank device if no connection</return>
	//BLEDevice getConnectedDevice() { if (this->connected) { return this->connectedDevice; } else { return BLEDevice(); } }

	/// <summary>
	/// Returns true if the controller has connected to a device
	/// </summary>
	/// <returns></returns>
	bool getConnected() { return this->connected; }

	///
	BluetoothBike& getConnectedBluetoothBike() { return this->connectedBluetoothBike; }

	bool getConnectedBikeStateUpdated() {
		// If the connectedBikeLastUpdate time of the controller is smaller than update of the connected bike then bike state updated
		return this->connectedBikeStatusLastUpdateTime < this->connectedBluetoothBike.getBikeStatusLastUpdateTime();
	}

	/// <summary>
	/// Returns the connectedBikeStatus structure so values of the bike can be read from the structure.
	/// </summary>
	/// <returns>The bike status of the connected e-bike</returns>
	BikeState& getBikeState() { return this->connectedBluetoothBike.getBikeState(); }

	/// <summary>
	/// Sets the initial state of the bluetooth device
	/// </summary>
	void init();

	/// <summary>
	/// Connect to the given bleDevice
	/// </summary>
	/// <param name="bleDevice">Device to connect to</param>
	/// <returns>True if the connection was made</returns>
	bool connect(BLEDevice& bleDevice);

	/// <summary>
	/// Disconnect from an existing connection
	/// </summary>
	/// <returns>True if the connection was correctly disconnected</returns>
	bool disconnect();

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
	/// </summary>
	void checkForChange();
};

#endif