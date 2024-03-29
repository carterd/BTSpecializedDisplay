#ifndef _BLUETOOTH_CONTROLLER_H
#define _BLUETOOTH_CONTROLLER_H

#include <lvgl.h>
#include <ArduinoBLE.h>
#include <vector>
#include <algorithm>

#include "BluetoothBikeDefinitions.h"
#include "BluetoothBikeState.h"
#include "BluetoothBike.h"
#include "BluetoothScanner.h"
#include "BluetoothHeartRateMonitor.h"
#include "ConfigStore.h"

#define BATTERY_FIRMWARE_VERSION_MAJOR_NUMBER_BUFFER_INDEX 3
#define BATTERY_FIRMWARE_VERSION_MINOR_NUMBER_BUFFER_INDEX 2
#define BATTERY_FIRMWARE_VERSION_PATCH_NUMBER_BUFFER_INDEX 4

#define MOTOR_FIRMWARE_VERSION_MAJOR_NUMBER_BUFFER_INDEX 3
#define MOTOR_FIRMWARE_VERSION_MINOR_NUMBER_BUFFER_INDEX 2
#define MOTOR_FIRMWARE_VERSION_PATCH_NUMBER_BUFFER_INDEX 4

/**
 * @brief This represents the interface to the bike and is a wrapper for BLEDevice interface to the bikes.
 * 
 * The current implementation can only support a single instance of a connected Bike. Rather than allowing
 * multiple instances of BluetoothBikes to be discovered and connected to, but such functaionlity isn't needed here.
 * However, we've not put a restriction on the number of instances of BluetoothController instances 
 * (still only gonna be 1)!
 * 
 * The Controller has two basic modes, that of searching for BLEDevices that could be instances of BluetoothBikes, and 
 * that of attempting to connect to a BLEDevice as an instance of a BluetoothBike.
 * 
 */
class BluetoothController
{
private:
	/// <summary>
	/// This is helpful on processing handlers which will have to find the Controller the update is associated with
	/// </summary>
	static std::vector<BluetoothController*> bluetoothControllers;
public:
	/// <summary>
	/// This is the updateHandler static method called from BLEDevice for connection events
	/// </summary>
	static void connect_handler_cb(BLEDevice device);
	/// <summary>
	/// This is the updateHandler static method called from BLEDevice for disconnect events
	/// </summary>
	static void disconnect_handler_cb(BLEDevice device);

protected:
	/// <summary>
	/// This boolean identifies is scanning is required to be stopped before a connection is made
	/// </summary>
	bool connectionRequiresScanStop;	
	/// <summary>
	/// Bluetooth Device which is bike
	/// </summary>
	BluetoothBike* bluetoothBike;

	/// <summary>
	/// Bluetooth Device which is HRM
	/// </summary>
	BluetoothHeartRateMonitor* bluetoothHeartRateMonitor;

	/// <summary>
	/// Bluetooth Scanner
	/// </summary>
	BluetoothScanner* bluetoothScanner;

protected:
	/// <summary>
	/// This is the class callback for connection event
	/// </summary>	
	void updateConnectCB(BLEDevice device);
	/// <summary>
	/// This is the class callback for disconnect event
	/// </summary>	
	void updateDisconnectCB(BLEDevice device);

public:
	/// <summary>
	/// Constructor of the bluetooth controller object
	/// </summary>
	BluetoothController(BluetoothBike* bluetoothBike, BluetoothHeartRateMonitor* bluetoothHeartRateMonitor, BluetoothScanner* bluetoothScanner, bool connectionRequiresScanStop);

	/// <summary>
	/// Distructor of the bluetooth controller object
	/// </summary>
	~BluetoothController();

	BluetoothBike* getBluetoothBike() { return this->bluetoothBike; }

	BluetoothHeartRateMonitor* getBluetoothHeartRateMonitor() { return this->bluetoothHeartRateMonitor; }

	bool getConnectionRequiresScanStop() { return this->connectionRequiresScanStop; }

	bool getMultipleBtConnectionsAllowed() { return !this->connectionRequiresScanStop; }

	/// <summary>
	/// Check to see the state of the connections, Returns true if connecting to the bike
	/// </summary>
	/// <return>True if a BT device has been connected</return>
	bool checkForConnections(BTAddressesConfig* btAddressesConfig, bool connectBatteryOnly);

	void startConnections(lv_obj_t* listener_obj);

	void stopConnections();

	void stopScanning();
};

#endif