#ifndef _BLUETOOTH_SCAN_LIST_H
#define _BLUETOOTH_SCAN_LIST_H

#include <lvgl.h>

#include "BaseLvObject.h"
#include "ButtonLabel.h"
#include "..\dev\BluetoothBikeController.h"
#include "..\dev\ConfigStore.h"

#include <unordered_map>

#define BLUETOOTH_SCAN_LIST_LABEL_HEIGHT 16

/// <summary>
/// This is the scanning list of possible bluetooth connections
/// </summary>
class BluetoothScanList : public BaseLvObject
{
private:
	lv_obj_t* list_obj;
	lv_obj_t* exit_button_obj;
	lv_obj_t* scan_anim_obj;
	lv_group_t* group;
	lv_indev_t* indev;
	ButtonLabel* buttonLabel;
	BluetoothBikeController* bluetoothController;
	ConfigStore* configStore;
	std::unordered_map<lv_obj_t*, BLEDevice> buttonDeviceMap;
	bool scanning;
	/// <summary>
	/// This is the object given focus on this object loosing focus
	/// </summary>
	BaseLvObject* defocusLvObj;

private:
	/// <summary>
	/// This helper function removes all list entries for the current scan list object
	/// </summary>
	void removeDeviceItems();
	/// <summary>
	/// Displays the button label if one has been assoicated with the scan list
	/// </summary>
	void showButtonLabels();
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
	lv_obj_t* getDeviceButton(BLEDevice* bleDevice);
	
public:
	/// <summary>
	/// Constructor of the Bluetooth scan list hence shows a list of dectected bluetooth devices/
	/// </summary>
	/// <param name="bluetoothMaster">The bluetooth controller object</param>
	/// <param name="configStore">The config store containing know bluetooth devices</param>
	/// <param name="indev">The indev used for taking context of the encoder</param>
	BluetoothScanList(BluetoothBikeController* bluetoothMaster, ConfigStore* configStore, lv_indev_t* indev);

	/// <summary>
	/// This Creates the instance of the Lv Objects associated with the instance and sub components associated with it
	/// </summary>
	/// <param name="parent">The LV object parent on which to attach the created lv object for this instance</param>
	/// <returns>The created LV object instance</returns>
	lv_obj_t* createLvObj(lv_obj_t* parent);

	/// <summary>
	/// This means the object and any sub objects should set any groups to be in focus at this point
	/// </summary>
	virtual void focusLvObj(BaseLvObject* defocusLvObj = NULL);

	/// <summary>
	/// Used to set the button label for this object or NULL for no button label
	/// </summary>
	/// <param name="buttonLabel">The button label object for this gui object</param>
	void setButtonLabel(ButtonLabel* buttonLabel);	

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