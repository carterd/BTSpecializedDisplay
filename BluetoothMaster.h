#ifndef _BLUETOOTH_MASTER_H
#define _BLUETOOTH_MASTER_H

#include <lvgl.h>
#include <ArduinoBLE.h>

class BluetoothMaster 
{
protected:
	lv_obj_t* listener_obj;
	bool scanning;
	bool available;
	BLEDevice availableDevice;
public:
	BluetoothMaster();
	void setListenerLvObj(lv_obj_t* listener_obj);
	void init();
	void startScan();
	void stopScan();
	BLEDevice getBLEDevice();
	void checkForChange();
};

#endif