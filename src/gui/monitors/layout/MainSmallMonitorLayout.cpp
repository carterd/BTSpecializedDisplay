#include "MainSmallMonitorLayout.h"

MainSmallMonitorLayout::MainSmallMonitorLayout(MonitorLvObject* mainMonitorLvObject, MonitorLvObject* smallMonitorLvObject) {
    this->mainMonitorLvObject = mainMonitorLvObject;
    this->smallMonitorLvObject = smallMonitorLvObject;
}

void MainSmallMonitorLayout::setBluetoothController(BluetoothBikeController* bluetoothBikeController)
{
	this->bluetoothBikeController = bluetoothBikeController;
	this->mainMonitorLvObject->setBluetoothController(bluetoothBikeController);
	this->smallMonitorLvObject->setBluetoothController(bluetoothBikeController);
}

lv_obj_t* MainSmallMonitorLayout::createLvObj(lv_obj_t* parent) {    
	// get the style we'll need for the bar
	
	this->this_obj = lv_obj_create(parent);
	lv_obj_set_size(this->this_obj, lv_obj_get_width(parent), lv_obj_get_height(parent));
	lv_obj_set_align(this->this_obj, LV_ALIGN_CENTER);	
	
	lv_obj_update_layout(parent);
    lv_obj_t* small_obj = lv_obj_create(this->this_obj);
    lv_obj_set_size(small_obj, lv_obj_get_width(parent), 16);
    lv_obj_set_align(small_obj, LV_ALIGN_TOP_MID);	
    this->smallMonitorLvObject->createLvObj(small_obj);
	
    lv_obj_t* main_obj = lv_obj_create(this->this_obj);
    lv_obj_set_size(main_obj, lv_obj_get_width(parent), 112);
    lv_obj_set_align(main_obj, LV_ALIGN_BOTTOM_MID);
    this->mainMonitorLvObject->createLvObj(main_obj);	
	
	return this->this_obj;
}

void MainSmallMonitorLayout::statusUpdate()
{
	if (this->bluetoothBikeController && this->bluetoothBikeController->getConnected()) {
		// This bleDevice should always be true as we've already idenfied scanned device available
		this->smallMonitorLvObject->statusUpdate();
        this->mainMonitorLvObject->statusUpdate();
	}
}

void MainSmallMonitorLayout::focusLvObj(BaseLvObject* defocusLvObj)
{
	// Ensure this item's state is in focus at this point
	MonitorLvObject::focusLvObj(defocusLvObj);

	this->smallMonitorLvObject->focusLvObj();
	this->mainMonitorLvObject->focusLvObj();
}

void MainSmallMonitorLayout::defocusLvObj() {
	// Ensure this item's state is in defocus at this point hence no contained monitors are drawn on updates
	MonitorLvObject::defocusLvObj();

	this->smallMonitorLvObject->defocusLvObj();
	this->mainMonitorLvObject->defocusLvObj();
}