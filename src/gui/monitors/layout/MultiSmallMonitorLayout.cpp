#include "MultiSmallMonitorLayout.h"

MultiSmallMonitorLayout::MultiSmallMonitorLayout(MonitorLvObject* smallMonitorLvObject1, MonitorLvObject* smallMonitorLvObject2, MonitorLvObject* smallMonitorLvObject3, MonitorLvObject* smallMonitorLvObject4, MonitorLvObject* smallMonitorLvObject5, MonitorLvObject* smallMonitorLvObject6, MonitorLvObject* smallMonitorLvObject7, MonitorLvObject* smallMonitorLvObject8) {
    this->smallMonitorLvObjects[0] = smallMonitorLvObject1;
    this->smallMonitorLvObjects[1] = smallMonitorLvObject2;
	this->smallMonitorLvObjects[2] = smallMonitorLvObject3;
	this->smallMonitorLvObjects[3] = smallMonitorLvObject4;
	this->smallMonitorLvObjects[4] = smallMonitorLvObject5;
	this->smallMonitorLvObjects[5] = smallMonitorLvObject6;
	this->smallMonitorLvObjects[6] = smallMonitorLvObject7;
	this->smallMonitorLvObjects[7] = smallMonitorLvObject8;
}

void MultiSmallMonitorLayout::setBluetoothController(BluetoothBikeController* bluetoothBikeController)
{
	this->bluetoothBikeController = bluetoothBikeController;
	for (int i = 0; i < MAX_MULTI_SMALL_MONITOR_OBJECTS; i++) {
		this->smallMonitorLvObjects[i]->setBluetoothController(bluetoothBikeController);
	}
}

lv_obj_t* MultiSmallMonitorLayout::createLvObj(lv_obj_t* parent) {    
	// get the style we'll need for the bar
	
	this->this_obj = lv_obj_create(parent);
	lv_obj_set_size(this->this_obj, lv_obj_get_width(parent), lv_obj_get_height(parent));
	lv_obj_set_align(this->this_obj, LV_ALIGN_CENTER);	
	
	lv_obj_update_layout(parent);
	lv_obj_t* small_obj;
	
	for (int i = 0; i < MAX_MULTI_SMALL_MONITOR_OBJECTS; i++) {
		small_obj = lv_obj_create(this->this_obj);
		lv_obj_set_size(small_obj, lv_obj_get_width(parent), SMALL_MONITOR_LV_OBJECT_HEIGHT);
		lv_obj_align(small_obj, LV_ALIGN_TOP_MID, 0, i * SMALL_MONITOR_LV_OBJECT_HEIGHT);
		this->smallMonitorLvObjects[i]->createLvObj(small_obj);
	}

	return this->this_obj;
}

void MultiSmallMonitorLayout::statusUpdate()
{
	if (this->bluetoothBikeController && this->bluetoothBikeController->getConnected()) {
		// This bleDevice should always be true as we've already idenfied scanned device available
		for (int i = 0; i < MAX_MULTI_SMALL_MONITOR_OBJECTS; i++) {
			if (smallMonitorLvObjects[i] != NULL) {
				smallMonitorLvObjects[i]->statusUpdate();
			}
		}
	}
	else 
	{
		// Nolonger connected Requred to scan again for connection
	}
}

void MultiSmallMonitorLayout::initBluetoothStats()
{
	for (int i = 0; i < MAX_MULTI_SMALL_MONITOR_OBJECTS; i++) {
		if (smallMonitorLvObjects[i] != NULL) {
			smallMonitorLvObjects[i]->initBluetoothStats();
		}
	}
}
