#ifndef _BLUETOOTH_SCAN_LIST_H
#define _BLUETOOTH_SCAN_LIST_H

#include <lvgl.h>

#include "ButtonLabelledLvObject.h"
#include "ButtonLabelBar.h"
#include "../dev/BluetoothBikeController.h"
#include "../dev/ConfigStore.h"

#include <unordered_map>

/// <summary>
/// This is the scanning list of possible bluetooth connections
/// </summary>
class BluetoothScanList : public ButtonLabelledLvObject
{
private:
	lv_obj_t* list_obj;
	lv_obj_t* exit_button_obj;
	lv_obj_t* scan_anim_obj;
	BluetoothBikeController* bluetoothController;
	ConfigStore* configStore;
	std::unordered_map<lv_obj_t*, String> buttonAddressMap;
	bool scanning;
	/// <summary>
	/// This is the object given focus on this object loosing focus
	/// </summary>
	BaseLvObject* defocusLvObj;
	BTAddressesConfig btAddressConfig;

protected:
	/// <summary>
	/// Update the state of the button label bar
	/// </summary>
	virtual void updateButtonLabelBar();

private:
	/// <summary>
	/// This helper function removes all list entries for the current scan list object
	/// </summary>
	void removeDeviceItems();
	/// <summary>
	/// This helper function adds list entries of already known devices to the list
	/// </summary>
	void addKnownDeviceItems();
	/// <summary>
	/// This starts the BT scanning
	/// </summary>
	void startBTScan();
	/// <summary>
	/// This stops the BT scanning
	/// </summary>
	void stopBTScan();
	/// <summary>
	/// Add a detected BLEDevice to the scan list object.
	/// </summary>
	/// <param name="bleDevice">The BLEDevice to add to the scan list</param>
	/// <returns>The device button created in the list</returns>
	lv_obj_t* addDeviceItem(BLEDevice* bleDevice);
	/// <summary>
	/// Add a detected Deivce giving the Address and display String
	/// </summary>
	/// <param name="addressString"></param>
	/// <param name="displayString"></param>
	/// <returns></returns>
	lv_obj_t* addDeviceItem(const String& addressString, const String& displayString);
	/// <summary>
	/// Add a button onto the list with given display text and a possible known device icon
	/// </summary>
	/// <param name="displayText">Display text in the list</param>
	/// <param name="knownDevice">True if the device is known hence shown with an icon</param>
	/// <returns>The created list item button object</returns>
	lv_obj_t* addDeviceButton(const char* displayText, bool knownDevice);
	/// <summary>
	/// Returns a given matching list item button object for a given BLEDevice
	/// </summary>
	/// <param name="bleDevice">The BLE device for which to find the </param>
	/// <returns></returns>
	lv_obj_t* getDeviceButton(const String& addressString);
	
public:
	/// <summary>
	/// Constructor of the Bluetooth scan list hence shows a list of dectected bluetooth devices/
	/// </summary>
	/// <param name="bluetoothMaster">The bluetooth controller object</param>
	/// <param name="configStore">The config store containing know bluetooth devices</param>
	/// <param name="indev">The indev used for taking context of the encoder</param>
	BluetoothScanList(BluetoothBikeController* bluetoothMaster, ConfigStore* configStore, lv_indev_t* indev, ButtonLabelBar* buttonLabelBar = NULL);

	/// <summary>
	/// Distructor foer the scan list object
	/// </summary>
	virtual ~BluetoothScanList();

	/// <summary>
	/// This Creates the instance of the Lv Objects associated with the instance and sub components associated with it
	/// </summary>
	/// <param name="parent">The LV object parent on which to attach the created lv object for this instance</param>
	/// <returns>The created LV object instance</returns>
	virtual lv_obj_t* createLvObj(lv_obj_t* parent);

	/// <summary>
	/// Destroy the lv objects for this object
	/// </summary>
	virtual void destroyLvObj();

	/// <summary>
	/// This means the object and any sub objects should set any groups to be in focus at this point
	/// </summary>
	virtual void focusLvObj(BaseLvObject* defocusLvObj = NULL);

	/// <summary>
	/// The callback on the exit button clicked
	/// </summary>
	/// <param name="event">The lv event that identifies pressing the exit button</param>
	void exitButtonCB(lv_event_t* event);

	/// <summary>
	/// The callback on a specific detected device clicked
	/// </summary>
	/// <param name="event">The lv event that identifies pressing the device entry</param>
	void deviceButtonCB(lv_event_t* event);

	/// <summary>
	/// The callback on the list required to be updated, i.e. a bluetooth device detected
	/// </summary>
	/// <param name="event">The lv event that identifies pressing the device entry</param>
	void refreshCB(lv_event_t* event);

public:
	static void exit_btn_cb(lv_event_t* event);
	static void device_btn_cb(lv_event_t* event);
	static void refresh_cb(lv_event_t* event);
};

#endif