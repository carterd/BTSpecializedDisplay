#include "BatteryCapacityMonitorMain.h"
#include "../../../themes/lv_theme.h"


BatteryCapacityMonitorMain::BatteryCapacityMonitorMain() {
}

const lv_point_t BatteryCapacityMonitorMain::battery_line_points_sh1107[] = {
	{32,110},							// Bottom
	{54, 110}, {55, 109},				// Bottom Right
	{55, 6}, {54, 5},					// Top Right
	{42, 5}, {39, 2}, {24, 2}, {21, 5}, // Top Detail
	{9, 5}, {8, 6},						// Top Left
	{8, 109}, {9, 110},					// Bottom Left
	{32, 110} };						// Bottom

const lv_point_t BatteryCapacityMonitorMain::battery_line_points_tdisplay_s3[] = {
	{86, 275},							// Bottom
	{141, 275}, {143, 272},				// Bottom Right
	{143, 15}, {141, 12},				// Top Right
	{111, 12}, {103, 5}, {66, 5}, {58, 12}, // Top Detail
	{28, 12}, {26, 15},					// Top Left
	{26, 272}, {28, 275},				// Bottom Left
	{86, 275} };						// Bottom

const charge_coords_vert_t BatteryCapacityMonitorMain::charge_coords_sh1107 = {16, 48, 103, 10};
const charge_coords_vert_t BatteryCapacityMonitorMain::charge_coords_tdisplay_s3 = {46, 122, 256, 25};

lv_obj_t* BatteryCapacityMonitorMain::createLvObj(lv_obj_t* parent) {
	BaseMonitorMain::createLvObj(parent);
	// get the style we'll need for the bar
	display_theme_styles_t* display_theme_styles = (display_theme_styles_t*)lv_disp_get_theme(lv_obj_get_disp(parent))->user_data;
	lv_style_t* main_battery_outline_style = &(display_theme_styles->main_battery_outline);
	lv_style_t* main_battery_charge_good_style = &(display_theme_styles->main_battery_charge_good);
	lv_style_t* main_battery_charge_moderate_style = &(display_theme_styles->main_battery_charge_moderate);
	lv_style_t* main_battery_charge_bad_style = &(display_theme_styles->main_battery_charge_bad);	

	//  Create a lineand apply the new style
	lv_obj_t* battery_line = lv_line_create(this->this_obj);
	lv_obj_add_style(battery_line, main_battery_outline_style, LV_PART_MAIN);

	const lv_point_t* battery_line_points = display_theme_styles->main_battery_line_points;
	lv_line_set_points(battery_line, battery_line_points, battery_line_point_count);
	
	this->main_battery_coords = display_theme_styles->main_battery_coords;
	for (int i = 0; i < 10; i++) {
		this->charge_line_points[i][0] = { this->main_battery_coords->xStart, (lv_coord_t)(main_battery_coords->yStart - i * main_battery_coords->yStep) };
		this->charge_line_points[i][1] = { this->main_battery_coords->xEnd, (lv_coord_t)(main_battery_coords->yStart - i * main_battery_coords->yStep) };
		this->levels[i] = lv_line_create(this->this_obj);
		if (i >= 5) { lv_obj_add_style(this->levels[i], main_battery_charge_good_style, LV_PART_MAIN); }
		else if (i >= 2) { lv_obj_add_style(this->levels[i], main_battery_charge_moderate_style, LV_PART_MAIN); }
		else { lv_obj_add_style(this->levels[i], main_battery_charge_bad_style, LV_PART_MAIN); }
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
				lv_coord_t line_width = (this->main_battery_coords->xEnd - this->main_battery_coords->xStart);
				lv_coord_t mid_point = this->main_battery_coords->xStart + line_width / 2;
				line_width = line_width * batteryChargeLeft / 20 + 1;

				this->charge_line_points[i][0].x = mid_point - line_width; // 0 - 10  9*3 / 2 .... 15 would be max === 48-16 = 32 = width /// 
				this->charge_line_points[i][1].x = mid_point + line_width; // 0 - 10  9*3 / 2 = 27/2 = 13 next 33 - 13 = 20
			} else {
				this->charge_line_points[i][0].x = this->main_battery_coords->xStart;
				this->charge_line_points[i][1].x = this->main_battery_coords->xEnd;
			}
			lv_line_set_points(this->levels[i], this->charge_line_points[i], 2);
		}
	}
}

