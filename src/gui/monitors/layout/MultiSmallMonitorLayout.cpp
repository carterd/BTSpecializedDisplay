#include "MultiSmallMonitorLayout.h"

MultiSmallMonitorLayout::MultiSmallMonitorLayout(MonitorLvObject* smallMonitorLvObject1, MonitorLvObject* smallMonitorLvObject2, MonitorLvObject* smallMonitorLvObject3, MonitorLvObject* smallMonitorLvObject4, MonitorLvObject* smallMonitorLvObject5, MonitorLvObject* smallMonitorLvObject6, MonitorLvObject* smallMonitorLvObject7, MonitorLvObject* smallMonitorLvObject8) {
    this->smallMonitorLvObject1 = smallMonitorLvObject1;
    this->smallMonitorLvObject2 = smallMonitorLvObject2;
	this->smallMonitorLvObject3 = smallMonitorLvObject3;
	this->smallMonitorLvObject4 = smallMonitorLvObject4;
	this->smallMonitorLvObject5 = smallMonitorLvObject5;
	this->smallMonitorLvObject6 = smallMonitorLvObject6;
	this->smallMonitorLvObject7 = smallMonitorLvObject7;
	this->smallMonitorLvObject8 = smallMonitorLvObject8;
}

void MultiSmallMonitorLayout::setBluetoothController(BluetoothBikeController* bluetoothBikeController)
{
	this->bluetoothBikeController = bluetoothBikeController;
	this->smallMonitorLvObject1->setBluetoothController(bluetoothBikeController);
	this->smallMonitorLvObject2->setBluetoothController(bluetoothBikeController);
	this->smallMonitorLvObject3->setBluetoothController(bluetoothBikeController);
	this->smallMonitorLvObject4->setBluetoothController(bluetoothBikeController);
	this->smallMonitorLvObject5->setBluetoothController(bluetoothBikeController);
	this->smallMonitorLvObject6->setBluetoothController(bluetoothBikeController);
	this->smallMonitorLvObject7->setBluetoothController(bluetoothBikeController);
	this->smallMonitorLvObject8->setBluetoothController(bluetoothBikeController);
}

lv_obj_t* MultiSmallMonitorLayout::createLvObj(lv_obj_t* parent) {    
	// get the style we'll need for the bar
	
	this->this_obj = lv_obj_create(parent);
	lv_obj_set_size(this->this_obj, lv_obj_get_width(parent), lv_obj_get_height(parent));
	lv_obj_set_align(this->this_obj, LV_ALIGN_CENTER);	
	
	lv_obj_update_layout(parent);
    lv_obj_t* small_obj = lv_obj_create(this->this_obj);
    lv_obj_set_size(small_obj, lv_obj_get_width(parent), 16);
    lv_obj_set_align(small_obj, LV_ALIGN_TOP_MID);	
    this->smallMonitorLvObject1->createLvObj(small_obj);
	
    small_obj = lv_obj_create(this->this_obj);
    lv_obj_set_size(small_obj, lv_obj_get_width(parent), 16);
    lv_obj_align(small_obj, LV_ALIGN_TOP_MID, 0, 16);
    this->smallMonitorLvObject2->createLvObj(small_obj);
	
	small_obj = lv_obj_create(this->this_obj);
    lv_obj_set_size(small_obj, lv_obj_get_width(parent), 16);
    lv_obj_align(small_obj, LV_ALIGN_TOP_MID, 0, 32);
    this->smallMonitorLvObject3->createLvObj(small_obj);

	small_obj = lv_obj_create(this->this_obj);
    lv_obj_set_size(small_obj, lv_obj_get_width(parent), 16);
    lv_obj_align(small_obj, LV_ALIGN_TOP_MID, 0, 48);
    this->smallMonitorLvObject4->createLvObj(small_obj);

	small_obj = lv_obj_create(this->this_obj);
    lv_obj_set_size(small_obj, lv_obj_get_width(parent), 16);
    lv_obj_align(small_obj, LV_ALIGN_TOP_MID, 0, 64);
    this->smallMonitorLvObject5->createLvObj(small_obj);

	small_obj = lv_obj_create(this->this_obj);
    lv_obj_set_size(small_obj, lv_obj_get_width(parent), 16);
    lv_obj_align(small_obj, LV_ALIGN_TOP_MID, 0, 80);
    this->smallMonitorLvObject6->createLvObj(small_obj);

	small_obj = lv_obj_create(this->this_obj);
    lv_obj_set_size(small_obj, lv_obj_get_width(parent), 16);
    lv_obj_align(small_obj, LV_ALIGN_TOP_MID, 0, 96);
    this->smallMonitorLvObject7->createLvObj(small_obj);

	small_obj = lv_obj_create(this->this_obj);
    lv_obj_set_size(small_obj, lv_obj_get_width(parent), 16);
    lv_obj_align(small_obj, LV_ALIGN_TOP_MID, 0, 112);
    this->smallMonitorLvObject8->createLvObj(small_obj);

	return this->this_obj;
}

void MultiSmallMonitorLayout::focusLvObj(BaseLvObject* defocusLvObj)
{
    this->smallMonitorLvObject1->focusLvObj();
    this->smallMonitorLvObject2->focusLvObj();
	this->smallMonitorLvObject3->focusLvObj();
	this->smallMonitorLvObject4->focusLvObj();
	this->smallMonitorLvObject5->focusLvObj();
	this->smallMonitorLvObject6->focusLvObj();
	this->smallMonitorLvObject7->focusLvObj();
	this->smallMonitorLvObject8->focusLvObj();
}

void MultiSmallMonitorLayout::statusUpdate()
{
	if (this->bluetoothBikeController && this->bluetoothBikeController->getConnected()) {
		// This bleDevice should always be true as we've already idenfied scanned device available
        this->smallMonitorLvObject1->statusUpdate();
	    this->smallMonitorLvObject2->statusUpdate();
		this->smallMonitorLvObject3->statusUpdate();
		this->smallMonitorLvObject4->statusUpdate();
		this->smallMonitorLvObject5->statusUpdate();
		this->smallMonitorLvObject6->statusUpdate();
		this->smallMonitorLvObject7->statusUpdate();
		this->smallMonitorLvObject8->statusUpdate();
	}
	else 
	{
		// Nolonger connected Requred to scan again for connection
	}
}
