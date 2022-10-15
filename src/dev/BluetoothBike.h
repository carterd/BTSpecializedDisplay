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

	/// <summary>
	/// This helper function converts the current content of the readBuffer to connectedBikeStatus.
	/// </summary>
	void readBufferToEbsBikeState();

    /// <summary>
    /// This helper function converts the current content of the readBuffer to connectedCscMeasurements.
	void readBufferToCscMeasurement();

	/// <summary>
	/// The bluetooth read-buffer that maybe required to be converted into connectedBikeStatus entries.
	/// </summary>
	uint8_t readBuffer[20];

protected:
    /// <summary>
    /// Store of the last time stamp that bike status was updated
    /// </summary>
    uint32_t bikeStatuslastUpdateTime;

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
	BLEDevice device;

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
	void updateCscCharacteristicCB(BLEDevice device, BLECharacteristic characteristic);
	void updateEbsCharacteristicCB(BLEDevice device, BLECharacteristic characteristic);

public:
	/// <summary>
	/// Constructor of the bluetooth controller object
	/// </summary>
	BluetoothBike();

	~BluetoothBike();

    /// <summary>
    /// 
    /// <summary
    bool getBikeStatuslastUpdateTime() { return this->bikeStatuslastUpdateTime; }

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
	void resetBikeStateAttributeValue(const BikeStateAttribute::BikeStateAttributeValue &defaultBikeStateAttributeValue) {
		this->bikeState.setAllAttributeValue(defaultBikeStateAttributeValue);
	}

    /// <summary>
    ///
    /// </summary>
    /// @param bikeStateAttributeIndex This bike attribute index to specify the attribute to read 
    /// @param monitorAttributeType This is the monitor attribute type to set for the attribute
	bool readBikeStateAttribute(BikeStateAttributeIndex bikeStateAttributeIndex, MonitorAttributeType monitorAttributeType = MonitorAttributeType::ONCE);
};

#endif