#ifndef _BLUETOOTH_BIKE_CONTROLLER_H
#define _BLUETOOTH_BIKE_CONTROLLER_H

#include <lvgl.h>
#include <ArduinoBLE.h>

#include "BluetoothBikeState.h"

#define UUID_CYCLE_SERVICE "1816"
#define UUID_CSC_MEASUREMENT_CHARACTERISTIC  "2a5b"
#define UUID_CSC_FEATURE_CHARACTERISTIC      "2a5c"
#define UUID_SC_CONTROL_POINT_CHARACTERISTIC "2a55"

#define UUID_SPECIALIZED_READ_SERVICE_GEN1   "00000001-0000-4b49-4e4f-525441474947"
#define UUID_SPECIALIZED_READ_VALUE_CHARACTERISTIC_GEN1  "00000011-0000-4b49-4e4f-525441474947"
#define UUID_SPECIALIZED_READ_KEY_CHARACTERISTIC_GEN1    "00000021-0000-4b49-4e4f-525441474947"

#define UUID_SPECIALIZED_WRITE_SERVICE_GEN1  "00000002-0000-4b49-4e4f-525441474947"
#define UUID_SPECIALIZED_WRITE_KEY_VALUE_CHARACTERISTIC_GEN1  "00000012-0000-4b49-4e4f-525441474947"

#define UUID_SPECIALIZED_NOTIFY_SERVICE_GEN1 "00000003-0000-4b49-4e4f-525441474947"
#define UUID_SPECIALIZED_NOTIFY_KEY_VALUE_CHARACTERISTIC_GEN1  "00000013-0000-4b49-4e4f-525441474947"

#define UUID_SPECIALIZED_READ_SERVICE_GEN2   "00000001-3731-3032-494d-484f42525554"
#define UUID_SPECIALIZED_READ_VALUE_CHARACTERISTIC_GEN2  "00000011-3731-3032-494d-484f42525554"
#define UUID_SPECIALIZED_READ_KEY_CHARACTERISTIC_GEN2    "00000021-3731-3032-494d-484f42525554"

#define UUID_SPECIALIZED_WRITE_SERVICE_GEN2  "00000002-3731-3032-494d-484f42525554"
#define UUID_SPECIALIZED_WRITE_KEY_VALUE_CHARACTERISTIC_GEN2  "00000012-3731-3032-494d-484f42525554"

#define UUID_SPECIALIZED_NOTIFY_SERVICE_GEN2 "00000003-3731-3032-494d-484f42525554"
#define UUID_SPECIALIZED_NOTIFY_KEY_VALUE_CHARACTERISTIC_GEN2  "00000013-3731-3032-494d-484f42525554"

enum BikeType { NONE, GEN1, GEN2 };

enum EbikeStatusArea { BATTERY=0, MOTOR=1, OTHER=2 };

enum EbikeStatusBattery { CAPACITY=0x00, CAPACITY_REMAIN=0x01, HEALTH=0x02, TEMP=0x03, CHARGE_CYCLES=0x04, VOLTAGE=0x05, CURRENT=0x06, HARDWARE_VERSION=0x07, FIRMWARE_VERSION=0x08, SERIAL_NO=0x09, CHARGE_PERCENT=0x0C };

enum EbikeStatusMotor { RIDER_POWER=0x00 };

enum EbikeStatusOther { WHEEL_SIZE=0x00 };

class BluetoothBikeController
{
private:
	uint16_t bufferToUint16() { return *((uint16_t*) &(this->readBuffer[2]));	}

	uint8_t bufferToUint8() { return this->readBuffer[2]; }

	bool readEbikeValue(int ebikeStatusArea, int ebikeStatusParameter);

	uint8_t readBuffer[20];
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
	bool available;
	/// <summary>
	/// The available device detected by a scan or used as the device in connections
	/// </summary>
	BLEDevice availableDevice;

	/// <summary>
	/// True when there is a bluetooth device is connected
	/// </summary>
	bool connected;
	/// <summary>
	/// Identifies the type of bike connected
	/// </summary>
	BikeType connectedBikeType;

	BluetoothBikeState connectedBikeState;

	BLECharacteristic cscMeasurementBleCha;

	BLECharacteristic cscFeatureBleCha;

	BLECharacteristic scControlPointBleCha;

	BLECharacteristic ebsReadKeyBleCha;

	BLECharacteristic ebsReadValueBleCha;

	BLECharacteristic ebsWriteKeyValueBleCha;

	BLECharacteristic ebsNotifyKeyValueBleChar;

public:
	/// <summary>
	/// Constructor of the bluetooth controller object
	/// </summary>
	BluetoothBikeController();
	/// <summary>
	/// Set the current LV object that is required to be updated with changes in the bluetooth controller
	/// </summary>
	/// <param name="listener_obj">lv_object to be sent refesh events on changes to BT scan/connection</param>
	void setListenerLvObj(lv_obj_t* listener_obj);
	/// <summary>
	/// Sets the initial state of the bluetooth device
	/// </summary>
	void init();
	/// <summary>
	/// Connect to the given bleDevice
	/// </summary>
	/// <param name="bleDevice">Device to connect to</param>
	/// <returns>True if the connection was made</returns>
	bool connect(BLEDevice* bleDevice);
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
	/// Returns the currently scanned BT device
	/// </summary>
	/// <returns>The currently scanned BT device</returns>
	BLEDevice getBLEDevice();
	/// <summary>
	/// This should be called periodically to update the listener object on changes to the controller
	/// </summary>
	void checkForChange();

	void readBatteryCapacity();
	void readBatteryCapacityRemaining();
	void readBatteryHealth();
	void readBatteryChargeCycles();
	void readBatterySerialNumber();
	void readBatteryChargePercent();
	void readBatteryHealth();
	void readBatteryTemp();
	void readBatteryVoltage();
	void readBatteryCurrent();
	void readBatteryHardwareVersion();
	void readBatteryFirmwareVersion();

	/// <summary>
	/// Returns the type of connected e-bike BT device
	/// </summary>
	/// <returns></returns>
	BikeType getConnectedBikeType();

	const BluetoothBikeState& getConnectedBikeState() { return this->connectedBikeState; };



};

#endif