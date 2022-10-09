#include "BatteryMonitorMain.h"
#include <LvglThemes/lv_theme_binary.h>

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

BatteryMonitorMain::BatteryMonitorMain() {
	this->battery_line_style = style_line(3);
	this->charge_line_style = style_line(8);
	lv_obj_t* levels[10];
	lv_point_t line_points[10][2];
	lv_style_t battery_line_style;
	lv_style_t charge_line_style;
}

const lv_point_t BatteryMonitorMain::battery_line_points[] = {
	{32,110},							// Bottom
	{54, 110}, {55, 109},				// Bottom Right
	{55, 6}, {54, 5},					// Top Right
	{42, 5}, {39, 2}, {24, 2}, {21, 5}, // Top Detail
	{9, 5}, {8, 6},						// Top Left
	{8, 109}, {9, 110},					// Bottom Left
	{32, 110} };						// Bottom

lv_obj_t* BatteryMonitorMain::createLvObj(lv_obj_t* parent) {
	// get the style we'll need for the bar
	theme_binary_styles_t* binary_styles = (theme_binary_styles_t*)lv_disp_get_theme(lv_obj_get_disp(parent))->user_data;
	lv_style_t* no_scrollbar = &(binary_styles->no_scrollbar);
	lv_style_t* card_style = &(binary_styles->card);
		
	lv_obj_update_layout(parent);
	this->this_obj = lv_obj_create(parent);
	lv_obj_set_size(this->this_obj, lv_obj_get_width(parent), lv_obj_get_height(parent));
	lv_obj_set_align(this->this_obj, LV_ALIGN_CENTER);
	
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

void BatteryMonitorMain::statusUpdate()
{
	if (this->bluetoothBikeController && this->bluetoothBikeController->getConnected()) {
		// This bleDevice should always be true as we've already idenfied scanned device available		
		if (this->bluetoothBikeController->getBikeState().getStateAttribute(BikeStateAttributeIndex::BATTERY_CHARGE_PERCENT).bikeStateAttributeValue.valueUint8 != this->displayedPercent) {
			this->update();
		}
		// We should use a counter to keep battery upto date ... as probaby needs explicit read from time to time
	}
	else 
	{
		// Nolonger connected Requred to scan again for connection
	}
}


void BatteryMonitorMain::update() {
	this->displayedPercent = this->bluetoothBikeController->getBikeState().getStateAttribute(BikeStateAttributeIndex::BATTERY_CHARGE_PERCENT).bikeStateAttributeValue.valueUint8;
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

		}
	}
}

void BatteryMonitorMain::focusLvObj(BaseLvObject* defocusLvObj)
{
	// The LVObj that'll get the refreshes and should there hook into updates
	this->bluetoothBikeController->readBikeStateAttribute(BikeStateAttributeIndex::BATTERY_CHARGE_PERCENT, MonitorAttributeType::EVERY_MINUTE);
	this->update();
}
