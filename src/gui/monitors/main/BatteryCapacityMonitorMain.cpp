#include "BatteryCapacityMonitorMain.h"
#include "../../../themes/lv_theme.h"

//static void refresh_cb(lv_event_t* event) {
//	((BatteryMonitorMain*)(event->user_data))->refreshCB(event);
//}


static lv_style_t style_line(lv_coord_t width) {
	lv_style_t style_line;
	lv_style_init(&style_line);
	lv_style_set_line_width(&style_line, width);
	lv_style_set_line_color(&style_line, lv_color_white());
	lv_style_set_line_rounded(&style_line, true);
	return style_line;
}

BatteryCapacityMonitorMain::BatteryCapacityMonitorMain() {
	this->battery_line_style = style_line(3);
	this->charge_line_style = style_line(8);
}

const lv_point_t BatteryCapacityMonitorMain::battery_line_points[] = {
	{32,110},							// Bottom
	{54, 110}, {55, 109},				// Bottom Right
	{55, 6}, {54, 5},					// Top Right
	{42, 5}, {39, 2}, {24, 2}, {21, 5}, // Top Detail
	{9, 5}, {8, 6},						// Top Left
	{8, 109}, {9, 110},					// Bottom Left
	{32, 110} };						// Bottom

lv_obj_t* BatteryCapacityMonitorMain::createLvObj(lv_obj_t* parent) {
	BaseMonitorMain::createLvObj(parent);
	
	//  Create a lineand apply the new style
	lv_obj_t* battery_line = lv_line_create(this->this_obj);
	lv_obj_add_style(battery_line, &this->battery_line_style, LV_PART_MAIN);
	lv_line_set_points(battery_line, battery_line_points, battery_line_point_count);
	
	for (int i = 0; i < 10; i++) {
		this->charge_line_points[i][0] = { 16, (lv_coord_t)(103 - i * 10) };
		this->charge_line_points[i][1] = { 48, (lv_coord_t)(103 - i * 10) };
		this->levels[i] = lv_line_create(this->this_obj);
		lv_obj_add_style(this->levels[i], &this->charge_line_style, LV_PART_MAIN);
		lv_line_set_points(this->levels[i], this->charge_line_points[i], 2);
		lv_obj_add_flag(this->levels[i], LV_OBJ_FLAG_HIDDEN);
	}
	
	return this->this_obj;
}

void BatteryCapacityMonitorMain::statusUpdate()
{
	if (this->bluetoothBikeController && this->bluetoothBikeController->getConnected()) {
		// This bleDevice should always be true as we've already idenfied scanned device available		
		if (this->bluetoothBikeController->getBikeState().getStateAttribute(BikeStateAttributeIndex::BATTERY_CHARGE_PERCENT).bikeStateAttributeValue.valueUint8 != this->displayedPercent) {
			this->displayedPercent = this->bluetoothBikeController->getBikeState().getStateAttribute(BikeStateAttributeIndex::BATTERY_CHARGE_PERCENT).bikeStateAttributeValue.valueUint8;
			MonitorLvObject::statusUpdate();
		}
		// We should use a counter to keep battery upto date ... as probaby needs explicit read from time to time
	}
	else
	{
		// Nolonger connected Requred to scan again for connection
	}
}

void BatteryCapacityMonitorMain::initBluetoothStats()
{
	this->bluetoothBikeController->getConnectedBluetoothBike().readBikeStateAttribute(BikeStateAttributeIndex::BATTERY_CHARGE_PERCENT, MonitorAttributeType::EVERY_MINUTE);
}

void BatteryCapacityMonitorMain::updateLvObj() {
	for (int i = 0; i < 10; i++) {
		lv_obj_t* level = this->levels[i];
		if (this->displayedPercent <= i * 10 ) {
			lv_obj_add_flag(this->levels[i], LV_OBJ_FLAG_HIDDEN);
		} else {
			lv_obj_clear_flag(this->levels[i], LV_OBJ_FLAG_HIDDEN);
			uint8_t batteryChargeLeft = this->displayedPercent - i * 10;
			if (batteryChargeLeft < 10)
			{				
				this->charge_line_points[i][0].x = 31 - (batteryChargeLeft * 3) / 2;
				this->charge_line_points[i][1].x = 33 + (batteryChargeLeft * 3) / 2;
			} else {
				this->charge_line_points[i][0].x = 16;
				this->charge_line_points[i][1].x = 48;
			}
			lv_line_set_points(this->levels[i], this->charge_line_points[i], 2);
		}
	}
}

