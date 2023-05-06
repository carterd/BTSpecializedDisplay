#ifndef _BLUETOOTH_HEART_RATE_MONITOR_H
#define _BLUETOOTH_HEART_RATE_MONITOR_H

#include "BluetoothHeartRateMonitorDefinitions.h"
#include "BluetoothheartRateMonitorState.h"
#include <lvgl.h>
#include <ArduinoBLE.h>
#include <vector>
#include <algorithm>

/**
 * @brief This represents the interface to the heart rate monitor and is a wrapper for BLEDevice interface to the turbo bikes.
 * 
 * Only on connect() will the BluetoothBike object attempt to obtain the correct BluetoothCharacteristics and from these
 * obtain the type and state of the Heart Rate being connected to.
 * 
 */
class BluetoothHeartRateMonitor
{
public:
	/// <summary>
	/// This is the updateHandler static method called from BLEDevice for notifications of CSC characterisitics of a bike.
	/// </summary>
	static void update_heartrate_handler_cb(BLEDevice device, BLECharacteristic characteristic);

	static BluetoothHeartRateMonitor* singleton;

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
    /// This helper function converts the current content of the readBuffer to connectedCscMeasurements.
	/// </summary>
	void readBufferToHeartRateMeasurement();

	/// <summary>
	/// Set the BLEDevice for the BluetoothBike, the BLEDevice should be treated as disconnected and hence
	/// require a subsequence call to connect() after the setBleDevice() call. 
	/// </summary>
	/// Interestingly we only reset  attributes if the BLEDevice has changed, therefore state can be mantained 
	/// between loss of signal disconnects. NOTE: However if you require say MonitorAttributeType to be reset 
	/// then the user is explicitly required to set the BLEDevice to an empty BLEDeive to ensure all parameters
	/// of BluetoothBike are reset (as if connecting for the first time).
	void setBleDevice(BLEDevice& bleDevice);

	/// <summary>
	/// This helper function attempts to enable notification of EbsCharacteristics so changes are processed by 
	/// updateEbsCharacteristicCB function.
	/// </summary>
	bool subscribeNotifications();

	/// <summary>
	/// The bluetooth read-buffer that maybe required to be converted into connectedBikeStatus entries.
	/// </summary>
	uint8_t readBuffer[20];

	/// <summary>
	/// The read-buffer number of bytes read.
	/// </summary>
	uint8_t bufferSize;

protected:
	/// <summary>
	/// This is the lv object which has to be infromed of updates to say scanning
	/// </summary>
	lv_obj_t* listener_obj;

    /// <summary>
    /// Store of the last time stamp that bike status was updated
    /// </summary>
    uint32_t statusLastUpdateTime;

    /// <summary>
    /// Store of the last time stamp that bike status was checked and acted upon
    /// </summary>
	uint32_t statusLastCheckTime;

	/// <summary>
	/// The BLEDevice of the bike
	/// </summary>
	BLEDevice bleDevice;

	/// <summary>
	/// This is the BLE Characteristic for CSC Measurement 
	/// </summary>
	BLECharacteristic heartRateMeasurementBleCha;

	/// <summary>
	/// This is the BLE Characteristic for CSC Measurement 
	/// </summary>
	BLECharacteristic bodySensorLocationBleCha;

	/// <summary>
	/// The status of the connected Monitor
	/// </summary>
	HeartRateMonitorState heartRateMonitorState;

public:
	/// <summary>
	/// The callback for CscCharacteritic notifications from the BleDevice.
	/// </summary>
	void updateHeartRateCharacteristicCB(BLEDevice device, BLECharacteristic characteristic);

public:
	/// <summary>
	/// Constructor of the bluetooth heart rate object
	/// </summary>
	BluetoothHeartRateMonitor();

	/// <summary>
	/// Set the current LV object that is required to be updated with changes in the bluetooth controller
	/// </summary>
	/// <param name="listener_obj">lv_object to be sent refesh events on changes to BT scan/connection</param>
	void setListenerLvObj(lv_obj_t* listener_obj) { this->listener_obj = listener_obj; }

    /// <summary>
    /// Performs the connect of the BluetoothBike to it's bleDevice
    /// </summary>
    bool connect(BLEDevice bleDevice);

    /// <summary>
    /// Performs the disconnect of the BluethoothBike form it's bleDevice
    /// </summary>
    bool disconnect();

	/// <summary>
	/// Rquired to be called periodically to check state of the bike and connection
	/// </summary>
	void checkForChange();

	/// <summary>
    /// Performs poll on the BLEDevice, and hence possibly fires updates due to notifications.
    /// </summary>
	void poll();

    /// <summary>
    /// Performs a check on the state of the bluetooth connection
    /// </summary>
    bool isConnected();

    /// <summary>
    /// Returns the last update time of any of the bikes states 
    /// </summary>
    uint32_t getStatusLastUpdateTime() { return this->statusLastUpdateTime; }

 	/// <summary>
	/// Returns true if the bike state of the connected bike has been updated since the last call to checkForConnectionChange(),
	/// hence we should at this point be identifying that an update of the GUI should be performed.
	/// </summary>
	bool getConnectedStateUpdated() {
		// If the connectedBikeLastUpdate time of the controller is smaller than update of the connected bike then bike state updated
		return this->statusLastCheckTime < this->statusLastUpdateTime;
	}

    /// <summary>
    /// returns true if the given bleDeivce matched the one used by this bike object
    /// </summary>
    bool isBleDevice(BLEDevice& bleDevice) { return this->bleDevice == bleDevice;}

};

#endif