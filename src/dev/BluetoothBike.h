#ifndef _BLUETOOTH_BIKE_H
#define _BLUETOOTH_BIKE_H

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

/**
 * @brief This represents the interface to the bike and is a wrapper for BLEDevice interface to the bikes
 * 
 */
class BluetoothBike
{

private:
	/// <summary>
	/// This is helpful on processing handlers which will have to find the Controller the update is associated with
	/// </summary>
	static BikeStateToBluetoothBikeRequest bikeStateToBluetoothBikeRequest;

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
	bool readEbsBikeValue(EbikeStatusArea ebikeStatusArea, EbikeStatusAttribute ebikeStatusAttribute) {
		switch (ebikeStatusArea) {
		case EbikeStatusArea::BATTERY:
		case EbikeStatusArea::SECONDARY_BATTERY:
			return this->readEbsBikeValue(static_cast<int>(ebikeStatusArea), static_cast<int>(ebikeStatusAttribute.ebikeStatusBattery));
		case EbikeStatusArea::MOTOR:
			return this->readEbsBikeValue(static_cast<int>(ebikeStatusArea), static_cast<int>(ebikeStatusAttribute.ebikeStatusMotor));
		case EbikeStatusArea::OTHER:
			return this->readEbsBikeValue(static_cast<int>(ebikeStatusArea), static_cast<int>(ebikeStatusAttribute.ebikeStatusOther));
		}
	}

	/// <summary>
	/// Read an ebike value specifcally a battery parameter, the value is read into the connectedBikeStatus.
	/// </summary>
	/// <param name="ebikeStatusArea">The area parameter to read from this should be 0x00 or 0x04 for battery and extra battery</param>
	/// <param name="ebikeStatusBatteryParameter">The specific battery parameter to read</param>
	/// <return>True if the parameter has been read correctly.</return>
	bool readEbsBikeValue(EbikeStatusArea ebikeStatusArea, EbikeStatusBattery ebikeStatusBatteryParameter) { return this->readEbsBikeValue(static_cast<int>(ebikeStatusArea), static_cast<int>(ebikeStatusBatteryParameter)); }

	/// <summary>
	/// Read an ebike value specifcally a motor parameter, the value is read into the connectedBikeStatus.
	/// </summary>
	/// <param name="ebikeStatusArea">The area parameter to read from this should be motor</param>
	/// <param name="ebikeStatusBatteryParameter">The specific motor parameter to read</param>
	/// <return>True if the parameter has been read correctly.</return>
	bool readEbsBikeValue(EbikeStatusArea ebikeStatusArea, EbikeStatusMotor ebikeStatusMotorParameter) { return this->readEbsBikeValue(static_cast<int>(ebikeStatusArea), static_cast<int>(ebikeStatusMotorParameter)); }

	/// <summary>
	/// Read an ebike value specifcally a other bike parameters, the value is read into the connectedBikeStatus.
	/// </summary>
	/// <param name="ebikeStatusArea">The area parameter to read from this should be 0x02 for other bike parameters</param>
	/// <param name="ebikeStatusBatteryParameter">The specific other bike parameter to read</param>
	/// <return>True if the parameter has been read correctly.</return>
	bool readEbsBikeValue(EbikeStatusArea ebikeStatusArea, EbikeStatusOther ebikeStatusOtherParameter)  { return this->readEbsBikeValue(static_cast<int>(ebikeStatusArea), static_cast<int>(ebikeStatusOtherParameter)); }

	/// <summary>
	/// Read an ebike value specifcally a battery parameter, the value is read into the connectedBikeStatus.
	/// </summary>
	/// <param name="ebikeStatusArea">The area parameter to read from this should be 0x00 or 0x04 for battery and extra battery</param>
	/// <param name="ebikeStatusBatteryParameter">The specific battery parameter to read</param>
	/// <return>True if the parameter has been read correctly.</return>
	bool readEbsBikeValue(int ebikeStatusArea, int ebikeStatusParameter);

	bool readRequest(BluetoothBikeRequest bluetoothBikeRequest);

	bool readRequestCommand(BluetoothBikeRequest::BluetoothBikeRequestCommand bluetoothBikeRequestCommand);

	/// <summary>
	/// This helper function converts the current content of the readBuffer to connectedBikeStatus.
	/// </summary>
	void readBufferToEbsBikeState();

    /// <summary>
    /// This helper function converts the current content of the readBuffer to connectedCscMeasurements.
	void readBufferToCscMeasurement();

	///
	///
	///
	bool subscribeEbsNotifications();

	///
	///
	/// 
	bool subscribeCscNotifications();

	/// <summary>
	/// The bluetooth read-buffer that maybe required to be converted into connectedBikeStatus entries.
	/// </summary>
	uint8_t readBuffer[20];

protected:

    /// <summary>
    /// Store of the last time stamp that bike status was updated
    /// </summary>
    uint32_t bikeStatusLastUpdateTime;

	/// <summary>
	/// Identifies the type of bike connected
	/// </summary>
	BikeType bikeType;

	/// <summary>
	/// The status of the connected bike
	/// </summary>
	BikeState bikeState;

	/// <summary>
	/// The BLEDevice of the bike
	/// </summary>
	BLEDevice bleDevice;

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
public:
	void updateCscCharacteristicCB(BLEDevice device, BLECharacteristic characteristic);
	void updateEbsCharacteristicCB(BLEDevice device, BLECharacteristic characteristic);

public:
	/// <summary>
	/// Constructor of the bluetooth controller object
	/// </summary>
	BluetoothBike();

	void setBleDevice(BLEDevice& bleDevice);

    /// <summary>
    /// Performs the connect of the BluetoothBike to it's bleDevice
    /// </summary>
    bool connect();

    /// <summary>
    /// Performs the disconnect of the BluethoothBike form it's bleDevice
    /// </summary>
    bool disconnect();

	void poll();

    /// <summary>
    /// Performs a check on the state of the bluetooth connection
    /// </summary>
    bool isConnected();

    /// <summary>
    /// Returns the last update time of any of the bikes states 
    /// </summary>
    uint32_t getBikeStatusLastUpdateTime() { return this->bikeStatusLastUpdateTime; }

    /// <summary>
    /// returns true if the given bleDeivce matched the one used by this bike object
    /// </summary>
    bool isBleDevice(BLEDevice& bleDevice) { return this->bleDevice == bleDevice;}

	/// <summary>
	/// Returns the type of connected e-bike BT device
	/// </summary>
	/// <returns>The bike type of the connected e-bike</returns>
	BikeType getBikeType() { return this->bikeType; }

	/// <summary>
	/// Returns the connectedBikeStatus structure so values of the bike can be read from the structure.
	/// </summary>
	/// <returns>The bike status of the connected e-bike</returns>
	BikeState& getBikeState() { return bikeState; }

    /// <summary>
    /// Resets the monitorAttributeType of all bike state attributes to a given type or defaults to read once Always Ignore
    /// </summary>
    /// @param defaultMonitorAttributeType The monitor type to set for all the bike state attributes
	void resetBikeStateMonitorAttributeType(MonitorAttributeType defaultMonitorAttributeType = MonitorAttributeType::ALWAYS_IGNORE) { 
		this->bikeState.setAllMonitorAttributeType(defaultMonitorAttributeType);
	}

    /// <summary>
    /// Resets the state last fetch time of all bike state attributes to a given value defualts to 0
    /// </summary>
    /// @param defaultLastFetchTime The default time to set all bike state attributes
	void resetBikeStateLastFetchTime(uint32_t defaultLastFetchTime = 0) { 
		this->bikeState.setAllLastFetchTimeTicks(defaultLastFetchTime);
	}

    /// <summary>
    /// Resets the bike state attribute value of all bike state attributes, note this doesn't take into account the
    /// values types so all attribute irrespective of union type will be set with the same value.
    /// </summary>
    /// @param defaultBikeStateAttributeValue The default bike state value to set all bike state attributes    
	void resetBikeStateAttributeValue() {
		BikeStateAttribute::BikeStateAttributeValue attribute;
		attribute.valueNumberString = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		this->bikeState.setAllAttributeValueOfType(BikeStateAttributeType::NUMBER_STRING, attribute);
		attribute.cscReading = { 0, 0 };
		this->bikeState.setAllAttributeValueOfType(BikeStateAttributeType::SPEED_CADENCE_READING, attribute);
		attribute.valueUint32 = 0;
		this->bikeState.setAllAttributeValueOfType(BikeStateAttributeType::UINT32_T, attribute);
		attribute.valueFirmwareVersion = {0x00, 0x00, 0x00};
		this->bikeState.setAllAttributeValueOfType(BikeStateAttributeType::FIRMWARE_VERSION, attribute);
		attribute.valueAssistLevels = { 0x00, 0x00, 0x00 };
		this->bikeState.setAllAttributeValueOfType(BikeStateAttributeType::ASSIST_LEVELS, attribute);
		attribute.valueHardwareVersion = { 0x00, 0x00 };
		this->bikeState.setAllAttributeValueOfType(BikeStateAttributeType::HARDWARE_VERSION, attribute);
		attribute.valueUint16 = 0;
		this->bikeState.setAllAttributeValueOfType(BikeStateAttributeType::UINT16_T, attribute);
		attribute.valueUint8 = 0;
		this->bikeState.setAllAttributeValueOfType(BikeStateAttributeType::UINT8_T, attribute);
		attribute.valueBool = false;
		this->bikeState.setAllAttributeValueOfType(BikeStateAttributeType::BOOL, attribute);
		attribute.valueFloat = 0.0;
		this->bikeState.setAllAttributeValueOfType(BikeStateAttributeType::FLOAT_T, attribute);
	}

    /// <summary>
    ///
    /// </summary>
    /// @param bikeStateAttributeIndex This bike attribute index to specify the attribute to read 
    /// @param monitorAttributeType This is the monitor attribute type to set for the attribute
	bool readBikeStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE);

	void setBikeStateMonitorAttributeType(BikeStateAttributeIndex bikeStateAttributeIndex, MonitorAttributeType monitorAttributeType) {
		this->bikeState.setMonitorAttributeType(bikeStateAttributeIndex, monitorAttributeType);
	}

	void setMinimumBikeStateMonitorAttributeType(BikeStateAttributeIndex bikeStateAttributeIndex, MonitorAttributeType monitorAttributeType) {
		if (monitorAttributeType > this->bikeState.getStateAttribute(bikeStateAttributeIndex).monitorAttributeType) {
        	this->bikeState.setMonitorAttributeType(bikeStateAttributeIndex, monitorAttributeType);
    	}		
	}
};

#endif