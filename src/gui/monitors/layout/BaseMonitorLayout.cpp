#include "BaseMonitorLayout.h"

BaseMonitorLayout::BaseMonitorLayout(MonitorLvObject** monitorLvObjects, int numberOfMonitorObjects) {
    this->monitorLvObjects = monitorLvObjects;
    this->numberOfMonitorObjects = numberOfMonitorObjects;
}

void BaseMonitorLayout::setBluetoothController(BluetoothBikeController* bluetoothBikeController)
{
	this->bluetoothBikeController = bluetoothBikeController;
	for (int i = 0; i < this->numberOfMonitorObjects; i++) {
		this->monitorLvObjects[i]->setBluetoothController(bluetoothBikeController);
	}
}

lv_obj_t* BaseMonitorLayout::createLvObj(lv_obj_t* parent) {    
	// get the style we'll need for the bar
	
	this->this_obj = lv_obj_create(parent);
	lv_obj_set_size(this->this_obj, lv_obj_get_width(parent), lv_obj_get_height(parent));
	lv_obj_set_align(this->this_obj, LV_ALIGN_CENTER);
	
	lv_obj_update_layout(parent);
	lv_obj_t* monitor_obj_parent;
	
    int posHeight = 0;
	for (int i = 0; i < this->numberOfMonitorObjects; i++) {
		if (this->monitorLvObjects[i]) {
			monitor_obj_parent = lv_obj_create(this->this_obj);
			lv_obj_set_width(monitor_obj_parent, lv_obj_get_width(parent));
			lv_obj_align(monitor_obj_parent, LV_ALIGN_TOP_MID, 0, posHeight);
			this->monitorLvObjects[i]->createLvObj(monitor_obj_parent);
			lv_obj_set_height(monitor_obj_parent, this->monitorLvObjects[i]->getHeight());
		}
        posHeight += this->monitorLvObjects[i]->getHeight();
	}

	return this->this_obj;
}

void BaseMonitorLayout::statusUpdate()
{
	if (this->bluetoothBikeController && this->bluetoothBikeController->getConnected()) {
		// This bleDevice should always be true as we've already idenfied scanned device available
		for (int i = 0; i < this->numberOfMonitorObjects; i++) {
			if (this->monitorLvObjects[i]) {
				this->monitorLvObjects[i]->statusUpdate();
			}
		}
	}
}

void BaseMonitorLayout::initBluetoothStats()
{
	for (int i = 0; i < this->numberOfMonitorObjects; i++) {
		if (this->monitorLvObjects[i]) {
			this->monitorLvObjects[i]->initBluetoothStats();
		}
	}
}

void BaseMonitorLayout::focusLvObj(BaseLvObject* defocusLvObj)
{
	// Ensure this item's state is in focus at this point
	MonitorLvObject::focusLvObj(defocusLvObj);
	for (int i = 0; i < this->numberOfMonitorObjects; i++) {
		if (this->monitorLvObjects[i]) {
			this->monitorLvObjects[i]->focusLvObj();
		}
	}
}

void BaseMonitorLayout::defocusLvObj() {
	// Ensure this item's state is in defocus at this point hence no contained monitors are drawn on updates
	MonitorLvObject::defocusLvObj();
	for (int i = 0; i < this->numberOfMonitorObjects; i++) {
		if (this->monitorLvObjects[i]) {
			this->monitorLvObjects[i]->defocusLvObj();
		}
	}
}
