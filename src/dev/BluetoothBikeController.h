#ifndef _BLUETOOTH_BIKE_CONTROLLER_H
#define _BLUETOOTH_BIKE_CONTROLLER_H

#include <lvgl.h>
#include <ArduinoBLE.h>
#include <vector>
#include <algorithm>

#include "BluetoothBikeDefinitions.h"
#include "BluetoothBikeState.h"

#define BATTERY_FIRMWARE_VERSION_MAJOR_NUMBER_BUFFER_INDEX 3
#define BATTERY_FIRMWARE_VERSION_MINOR_NUMBER_BUFFER_INDEX 2
#define BATTERY_FIRMWARE_VERSION_PATCH_NUMBER_BUFFER_INDEX 4

#define MOTOR_FIRMWARE_VERSION_MAJOR_NUMBER_BUFFER_INDEX 3
#define MOTOR_FIRMWARE_VERSION_MINOR_NUMBER_BUFFER_INDEX 2
#define MOTOR_FIRMWARE_VERSION_PATCH_NUMBER_BUFFER_INDEX 4

class BluetoothBikeController
{

private:
	/// <summary>
	/// This is helpful on processing handlers which will have to find the Controller the update is associated with
	/// </summary>
	static std::vector<BluetoothBikeController*> bluetoothBikeControllers;
	static BikeStateToBluetoothBikeRequest bikeStateToBluetoothBikeRequest;
public:
	/// <summary>
	/// This is the updateHandler static method
	/// </summary>
	static void update_handler_cb(BLEDevice device, BLECharacteristic characteristic);
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
	/// Read an ebike value specifcally a battery parameter, the value is read into the connectedBikeStatus.
	/// </summary>
	/// <param name="ebikeStatusArea">The area parameter to read from this should be 0x00 or 0x04 for battery and extra battery</param>
	/// <param name="ebikeStatusBatteryParameter">The specific battery parameter to read</param>
	/// <return>True if the parameter has been read correctly.</return>
	bool readEbikeValue(EbikeStatusArea ebikeStatusArea, EbikeStatusAttribute ebikeStatusAttribute) {
		switch (ebikeStatusArea) {
		case EbikeStatusArea::BATTERY:
		case EbikeStatusArea::SECONDARY_BATTERY:
			return this->readEbikeValue(static_cast<int>(ebikeStatusArea), static_cast<int>(ebikeStatusAttribute.ebikeStatusBattery));
		case EbikeStatusArea::MOTOR:
			return this->readEbikeValue(static_cast<int>(ebikeStatusArea), static_cast<int>(ebikeStatusAttribute.ebikeStatusMotor));
		case EbikeStatusArea::OTHER:
			return this->readEbikeValue(static_cast<int>(ebikeStatusArea), static_cast<int>(ebikeStatusAttribute.ebikeStatusOther));
		}
	}

	/// <summary>
	/// Read an ebike value specifcally a battery parameter, the value is read into the connectedBikeStatus.
	/// </summary>
	/// <param name="ebikeStatusArea">The area parameter to read from this should be 0x00 or 0x04 for battery and extra battery</param>
	/// <param name="ebikeStatusBatteryParameter">The specific battery parameter to read</param>
	/// <return>True if the parameter has been read correctly.</return>
	bool readEbikeValue(EbikeStatusArea ebikeStatusArea, EbikeStatusBattery ebikeStatusBatteryParameter) { return this->readEbikeValue(static_cast<int>(ebikeStatusArea), static_cast<int>(ebikeStatusBatteryParameter)); }

	/// <summary>
	/// Read an ebike value specifcally a motor parameter, the value is read into the connectedBikeStatus.
	/// </summary>
	/// <param name="ebikeStatusArea">The area parameter to read from this should be motor</param>
	/// <param name="ebikeStatusBatteryParameter">The specific motor parameter to read</param>
	/// <return>True if the parameter has been read correctly.</return>
	bool readEbikeValue(EbikeStatusArea ebikeStatusArea, EbikeStatusMotor ebikeStatusMotorParameter) { return this->readEbikeValue(static_cast<int>(ebikeStatusArea), static_cast<int>(ebikeStatusMotorParameter)); }

	/// <summary>
	/// Read an ebike value specifcally a other bike parameters, the value is read into the connectedBikeStatus.
	/// </summary>
	/// <param name="ebikeStatusArea">The area parameter to read from this should be 0x02 for other bike parameters</param>
	/// <param name="ebikeStatusBatteryParameter">The specific other bike parameter to read</param>
	/// <return>True if the parameter has been read correctly.</return>
	bool readEbikeValue(EbikeStatusArea ebikeStatusArea, EbikeStatusOther ebikeStatusOtherParameter)  { return this->readEbikeValue(static_cast<int>(ebikeStatusArea), static_cast<int>(ebikeStatusOtherParameter)); }

	/// <summary>
	/// Read an ebike value specifcally a battery parameter, the value is read into the connectedBikeStatus.
	/// </summary>
	/// <param name="ebikeStatusArea">The area parameter to read from this should be 0x00 or 0x04 for battery and extra battery</param>
	/// <param name="ebikeStatusBatteryParameter">The specific battery parameter to read</param>
	/// <return>True if the parameter has been read correctly.</return>
	bool readEbikeValue(int ebikeStatusArea, int ebikeStatusParameter);

	/// <summary>
	/// This helper function converts the current content of the readBuffer to connectedBikeStatus.
	/// </summary>
	void readBufferToBikeState();

	/// <summary>
	/// The bluetooth read-buffer that maybe required to be converted into connectedBikeStatus entries.
	/// </summary>
	uint8_t readBuffer[20];

	void checkForScanningChange();

	void checkForConnectionChange();

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

	///
	///
	///
	bool connectedBikeStateUpdated;

	/// <summary>
	/// Identifies the type of bike connected
	/// </summary>
	BikeType connectedBikeType;

	/// <summary>
	/// The status of the connected bike
	/// </summary>
	BikeState bikeState;

	/// <summary>
	/// The available device detected by a scan
	/// </summary>
	BLEDevice scannedDevice;

	/// <summary>
	/// The available device detected by a scan or used as the device in connections
	/// </summary>
	BLEDevice connectedDevice;

	/// <summary>
	/// This is the BLE Characteristic for CSC Measurement 
	/// </summary>
	BLECharacteristic cscMeasurementBleCha;

	/// <summary>
	/// This is the BLE Characteristic for CSC Feature
	/// </summary>
	BLECharacteristic cscFeatureBleCha;

	/// <summary>
	/// This is the BLE Characteristic for SC Control Point
	/// </summary>
	BLECharacteristic scControlPointBleCha;

	/// <summary>
	/// This is the BLE Characteristic for Ebike Specialized Read Key
	/// </summary>
	BLECharacteristic ebsReadKeyBleCha;

	/// <summary>
	/// This is the BLE Characteristic for Ebike Specialized Read Value
	/// </summary>
	BLECharacteristic ebsReadValueBleCha;

	/// <summary>
	/// This is the BLE Characteristic for Ebike Specialized Write Key-Value
	/// </summary>
	BLECharacteristic ebsWriteKeyValueBleCha;

	/// <summary>
	/// This is the BLE Characteristic for Ebike Specialized Notify Key Value 
	/// </summary>
	BLECharacteristic ebsNotifyKeyValueBleCha;
protected:
	void updateCharacteristicCB(BLEDevice device, BLECharacteristic characteristic);
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
	BLEDevice getConnectedDevice() { if (this->connected) { return this->connectedDevice; } else { return BLEDevice(); } }

	/// <summary>
	/// Returns true if the controller has connected to a device
	/// </summary>
	/// <returns></returns>
	bool getConnected() { return this->connected; }

	/// <summary>
	/// Returns the type of connected e-bike BT device
	/// </summary>
	/// <returns>The bike type of the connected e-bike</returns>
	BikeType getConnectedBikeType() { if (this->connected) { return this->connectedBikeType; } else { return BikeType::NONE; } }

	/// <summary>
	/// Returns the connectedBikeStatus structure so values of the bike can be read from the structure.
	/// </summary>
	/// <returns>The bike status of the connected e-bike</returns>
	const BikeState& getBikeState() { return bikeState; }

	void resetConnectedBikeStateMonitorAttributeType(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE) { 
		this->bikeState.setMonitorAttributeType(monitorAttributeType);
	}

	void resetConnectedBikeStateTime(uint32_t time = 0) { 
		this->bikeState.setLastFetchTimeTicks(time);
	}

	bool readBikeStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE);

	// Battery
	bool readBattery(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE);
	bool readBatteryCapacity(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE) { 
		this->connectedBikeState.batteryCapacity.setMonitorAttributeType(monitorAttributeType); 
		return this->readEbikeValue(EbikeStatusArea::BATTERY, EbikeStatusBattery::CAPACITY); 
	}
	bool readBatteryCapacityRemaining(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE) { 
		this->connectedBikeState.batteryCapacityRemaining.setMonitorAttributeType(monitorAttributeType); 
		return this->readEbikeValue(EbikeStatusArea::BATTERY, EbikeStatusBattery::CAPACITY_REMAIN); 
	}
	bool readBatteryChargeCycles(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE) { 
		this->connectedBikeState.batteryChargeCycles.setMonitorAttributeType(monitorAttributeType); 
		return this->readEbikeValue(EbikeStatusArea::BATTERY, EbikeStatusBattery::CHARGE_CYCLES); 
	}	
	bool readBatteryChargePercent(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE) { 
		this->connectedBikeState.batteryChargePercent.setMonitorAttributeType(monitorAttributeType); 

		this->readBikeStateAttribute(BikeStateAttributeIndex::BATTERY_CAPACITY, monitorAttributeType);

		return this->readEbikeValue(EbikeStatusArea::BATTERY, EbikeStatusBattery::CHARGE_PERCENT); 
	}
	bool readBatteryHealth(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE) {
		this->connectedBikeState.batteryHealth.setMonitorAttributeType(monitorAttributeType);  
		return this->readEbikeValue(EbikeStatusArea::BATTERY, EbikeStatusBattery::HEALTH); 
	}
	bool readBatteryTemp(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE) { 
		this->connectedBikeState.batteryTemp.setMonitorAttributeType(monitorAttributeType); 
		return this->readEbikeValue(EbikeStatusArea::BATTERY, EbikeStatusBattery::TEMP); 
	}
	bool readBatteryVoltage(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE) { 
		this->connectedBikeState.batteryVoltage.setMonitorAttributeType(monitorAttributeType); 
		return this->readEbikeValue(EbikeStatusArea::BATTERY, EbikeStatusBattery::VOLTAGE); 
	}
	bool readBatteryCurrent(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE) { 
		this->connectedBikeState.batteryCurrent.setMonitorAttributeType(monitorAttributeType); 
		return this->readEbikeValue(EbikeStatusArea::BATTERY, EbikeStatusBattery::CURRENT); 
	}
	bool readBatteryFirmwareVersion(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE) { 
		this->connectedBikeState.batteryFirmwareVersion.setMonitorAttributeType(monitorAttributeType); 
		return this->readEbikeValue(EbikeStatusArea::BATTERY, EbikeStatusBattery::FIRMWARE_VERSION);  
	}
	bool readBatteryHardwareVersion(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE);
	bool readBatterySerialNumber(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE);

	// Motor
	bool readMotor(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE);
	bool readMotorTemp(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE) { 
		this->connectedBikeState.motorTemp.setMonitorAttributeType(monitorAttributeType); 
		return this->readEbikeValue(EbikeStatusArea::MOTOR, EbikeStatusMotor::TEMP); 
	}
    bool readRiderPower(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE) { 
		this->connectedBikeState.riderPower.setMonitorAttributeType(monitorAttributeType); 
		return this->readEbikeValue(EbikeStatusArea::MOTOR, EbikeStatusMotor::RIDER_POWER); 
	}
    bool readMotorCadence(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE) { 
		this->connectedBikeState.motorCadence.setMonitorAttributeType(monitorAttributeType); 
		return this->readEbikeValue(EbikeStatusArea::MOTOR, EbikeStatusMotor::CADENCE); 
	}
    bool readMotorSpeed(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE) {
		this->connectedBikeState.motorSpeed.setMonitorAttributeType(monitorAttributeType); 
		return this->readEbikeValue(EbikeStatusArea::MOTOR, EbikeStatusMotor::SPEED); 
	}
    bool readMotorOdometer(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE) { 
		this->connectedBikeState.motorOdometer.setMonitorAttributeType(monitorAttributeType); 
		return this->readEbikeValue(EbikeStatusArea::MOTOR, EbikeStatusMotor::ODOMETER); 
	}
    bool readMotorAssistLevel(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE) { 
		this->connectedBikeState.motorAssistLevel.setMonitorAttributeType(monitorAttributeType); 
		return this->readEbikeValue(EbikeStatusArea::MOTOR, EbikeStatusMotor::ASSIST_LEVEL); 
	}
    bool readMotorFirmwareVersion(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE) { 
		this->connectedBikeState.motorFirmwareVersion.setMonitorAttributeType(monitorAttributeType); 
		return this->readEbikeValue(EbikeStatusArea::MOTOR, EbikeStatusMotor::FIRMWARE_VERSION); 
	}
    bool readMotorPower(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE) { 
		this->connectedBikeState.motorPower.setMonitorAttributeType(monitorAttributeType); 
		return this->readEbikeValue(EbikeStatusArea::MOTOR, EbikeStatusMotor::MOTOR_POWER); 
	}
    bool readPeakAssistLevels(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE) {
		this->connectedBikeState.peakAssistLevels.setMonitorAttributeType(monitorAttributeType); 
		 return this->readEbikeValue(EbikeStatusArea::MOTOR, EbikeStatusMotor::PEAK_ASSIST); 
	}
    bool readMotorSerailNumber(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE);

	// Other
	bool readBikeOther(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE);
	bool readBikeOnOffState(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE) { 
		this->connectedBikeState.bikeOnOffState.setMonitorAttributeType(monitorAttributeType); 
		return this->readEbikeValue(EbikeStatusArea::OTHER, EbikeStatusOther::BIKE_ON_OFF); 
	}
	bool readBikeSerialNumber(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE) {
		this->connectedBikeState.bikeSerialNumber.setMonitorAttributeType(monitorAttributeType); 
		 return this->readEbikeValue(EbikeStatusArea::OTHER, EbikeStatusOther::BIKE_SERIAL_NO); 
	}
	bool readWheelCircumference(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE) { 
		this->connectedBikeState.wheelCircumference.setMonitorAttributeType(monitorAttributeType); 
		return this->readEbikeValue(EbikeStatusArea::OTHER, EbikeStatusOther::WHEEL_CIRCUMFERENCE); 
	}
	bool readAssistPercentages(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE);
	bool readBatteryConnected(MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE);

	bool subscribeEbikeNotifications();

	// Cadence


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
	/// This should be called periodically to update the listener object on changes to the controller
	/// </summary>
	void checkForChange();
};

#endif