#ifndef _BLUETOOTH_SCAN_LIST_H
#define _BLUETOOTH_SCAN_LIST_H

#include <lvgl.h>

#include "BaseLvObject.h"
#include "ButtonLabel.h"
#include "BluetoothMaster.h"
#include "ConfigStore.h"

#include <unordered_map>

#define BLUETOOTH_SCAN_LIST_LABEL_HEIGHT 16

class BluetoothScanList : public BaseLvObject
{
private:
	lv_obj_t* list_obj;
	lv_obj_t* exit_button_obj;
	lv_obj_t* scan_anim_obj;
	lv_group_t* group;
	lv_indev_t* indev;
	ButtonLabel* buttonLabel;
	BluetoothMaster* bluetoothMaster;
	ConfigStore* configStore;
	std::unordered_map<lv_obj_t*, BLEDevice> buttonDeviceMap;
	bool scanning;

private:
	void removeDeviceItems();
	void showButtonLabels();
	void startBTScan();
	void stopBTScan();
	lv_obj_t* addDeviceItem(BLEDevice* bleDevice);
	lv_obj_t* addDeviceButton(const char* displayText, bool knownDevice);
	lv_obj_t* getDeviceButton(BLEDevice* bleDevice);
	
public:
	BluetoothScanList(BluetoothMaster* bluetoothMaster, ConfigStore* configStore, lv_indev_t* indev);
	lv_obj_t* createLvObj(lv_obj_t* parent);
	void focusLvObj();	
	void setButtonLabel(ButtonLabel* buttonLabel);	
	void exitButtonCB(lv_event_t* event);
	void deviceButtonCB(lv_event_t* event);
	void refreshCB(lv_event_t* event);
};

#endif