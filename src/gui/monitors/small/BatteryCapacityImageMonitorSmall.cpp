#include "BatteryCapacityImageMonitorSmall.h"
#include <LvglThemes/lv_theme_binary.h>

static lv_style_t style_line(lv_coord_t width, bool rounded = true) {
	lv_style_t style_line;
	lv_style_init(&style_line);
	lv_style_set_line_width(&style_line, width);
	lv_style_set_line_color(&style_line, lv_color_white());
	lv_style_set_line_rounded(&style_line, rounded);
	return style_line;
}

const lv_point_t BatteryCapacityImageMonitorSmall::battery_line_points[] = {
	{1, 8},                             // Bottom
	{1, 13},{2, 14},                     // Bottom Right
	{59, 14}, {60, 13},                 // Top Right
    {60, 10},{62, 9},{62, 6},{60, 5},   // Top Detail
    {60, 2}, {59, 1},                   // Top Left
    {2, 1}, {1, 2},         			// Bottom Left
	{1, 8} };						    // Bottom

BatteryCapacityImageMonitorSmall::BatteryCapacityImageMonitorSmall() {
	this->battery_line_style = style_line(1);
	this->charge_line_style = style_line(8, false);
}

lv_obj_t* BatteryCapacityImageMonitorSmall::createLvObj(lv_obj_t* parent) {
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
	
	this->charge_line_points[0] = { 4 , 8 };
	this->charge_line_points[1] = { 58 , 8 };
	this->level = lv_line_create(this->this_obj);
	lv_obj_add_style(this->level, &this->charge_line_style, LV_PART_MAIN);
	lv_line_set_points(this->level, this->charge_line_points, 2);

	return this->this_obj;
}

void BatteryCapacityImageMonitorSmall::statusUpdate()
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

void BatteryCapacityImageMonitorSmall::initBluetoothStats()
{
	this->bluetoothBikeController->getConnectedBluetoothBike().readBikeStateAttribute(BikeStateAttributeIndex::BATTERY_CHARGE_PERCENT, MonitorAttributeType::EVERY_MINUTE);
}

void BatteryCapacityImageMonitorSmall::updateLvObj() {
	this->charge_line_points[1].x = 58 * this->displayedPercent / 100;
	lv_line_set_points(this->level, this->charge_line_points, 2);
}