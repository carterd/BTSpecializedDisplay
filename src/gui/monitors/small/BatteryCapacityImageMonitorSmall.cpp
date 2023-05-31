#include "BatteryCapacityImageMonitorSmall.h"
#include "../../../themes/display_theme.h"

const lv_point_t BatteryCapacityImageMonitorSmall::battery_line_points_sh1107[] = {
	{1, 8},                             // Bottom
	{1, 13},{2, 14},                     // Bottom Right
	{59, 14}, {60, 13},                 // Top Right
    {60, 10},{62, 9},{62, 6},{60, 5},   // Top Detail
    {60, 2}, {59, 1},                   // Top Left
    {2, 1}, {1, 2},         			// Bottom Left
	{1, 8} };						    // Bottom

const lv_point_t BatteryCapacityImageMonitorSmall::battery_line_points_tdisplay[] = {
	{6, 16},                             // Bottom
	{6, 26},{8, 28},                    // Bottom Right
	{122, 28}, {124, 26},                 // Top Right
    {124, 20},{128, 18},{128, 12},{124, 10},   // Top Detail
    {124, 4}, {122, 2},                   // Top Left
    {8, 2}, {6, 4},         			// Bottom Left
	{6, 16} };						    // Bottom

const lv_point_t BatteryCapacityImageMonitorSmall::battery_line_points_tdisplay_s3[] = {
	{2, 20},                             	// Bottom
	{2, 35},{4, 37},                     	// Bottom Right
	{160, 37}, {163, 35},                 	// Top Right
    {163, 26},{167, 24},{167, 16},{163, 14},   // Top Detail
    {163, 4}, {160, 2},                   	// Top Left
    {4, 2}, {2, 4},         				// Bottom Left
	{2, 20} };						    	// Bottom

const charge_coords_hor_t BatteryCapacityImageMonitorSmall::charge_coords_sh1107 = {4, 58, 8};
const charge_coords_hor_t BatteryCapacityImageMonitorSmall::charge_coords_tdisplay = {8, 120, 15};
const charge_coords_hor_t BatteryCapacityImageMonitorSmall::charge_coords_tdisplay_s3 = {7, 155, 20};

BatteryCapacityImageMonitorSmall::BatteryCapacityImageMonitorSmall() {
}

lv_obj_t* BatteryCapacityImageMonitorSmall::createLvObj(lv_obj_t* parent) {
	BaseMonitorSmall::createLvObj(parent);

	display_theme_styles_t* display_theme_styles = (display_theme_styles_t*)lv_disp_get_theme(lv_obj_get_disp(parent))->user_data;
	lv_style_t* battery_outline_style = &(display_theme_styles->small_battery_outline);
	lv_style_t* battery_charge_good_style = &(display_theme_styles->small_battery_charge_good);
	lv_style_t* battery_charge_moderate_style = &(display_theme_styles->small_battery_charge_moderate);
	lv_style_t* battery_charge_bad_style = &(display_theme_styles->small_battery_charge_bad);
	const lv_point_t* battery_line_points = display_theme_styles->small_battery_line_points;
	this->charge_coords = display_theme_styles->small_battery_coords;

	//  Create a lineand apply the new style
	lv_obj_t* battery_line = lv_line_create(this->this_obj);
	lv_obj_add_style(battery_line, battery_outline_style, LV_PART_MAIN);
	lv_line_set_points(battery_line, battery_line_points, battery_line_point_count);

	for (int i = 0 ; i < 3 ; i++) {
		this->charge_line_points[i][0] = { this->charge_coords->xMin , this->charge_coords->yPos };
		this->charge_line_points[i][1] = { this->calculateXPos(100, this->max_values[i]) , this->charge_coords->yPos };
		this->level[i] = lv_line_create(this->this_obj);
		if (i == 0) { lv_obj_add_style(this->level[i], battery_charge_good_style, LV_PART_MAIN); }
		else if (i == 1) { lv_obj_add_style(this->level[i], battery_charge_moderate_style, LV_PART_MAIN); }
		else { lv_obj_add_style(this->level[i], battery_charge_bad_style, LV_PART_MAIN); }
		lv_line_set_points(this->level[i], this->charge_line_points[i], 2);
	}
	
	return this->this_obj;
}

void BatteryCapacityImageMonitorSmall::statusUpdate()
{
	if (this->bluetoothBike && this->bluetoothBike->isConnected()) {
		// This bleDevice should always be true as we've already idenfied scanned device available		
		if (this->bluetoothBike->getBikeState().getStateAttribute(BikeStateAttributeIndex::BATTERY_CHARGE_PERCENT).bikeStateAttributeValue.valueUint8 != this->displayedPercent) {
			this->displayedPercent = this->bluetoothBike->getBikeState().getStateAttribute(BikeStateAttributeIndex::BATTERY_CHARGE_PERCENT).bikeStateAttributeValue.valueUint8;
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
	this->bluetoothBike->readBikeStateAttribute(BikeStateAttributeIndex::BATTERY_CHARGE_PERCENT, MonitorAttributeType::EVERY_MINUTE);
}

void BatteryCapacityImageMonitorSmall::updateLvObj() {
	for (int i = 0 ; i < 3 ; i++) {
		this->charge_line_points[i][1].x = this->calculateXPos( this->displayedPercent, this->max_values[i] );
		lv_line_set_points(this->level[i], this->charge_line_points[i], 2);
	}
}

lv_coord_t BatteryCapacityImageMonitorSmall::calculateXPos(int percentage, int percentageMax) {
	if (percentage > percentageMax) { percentage = percentageMax; }
	return this->charge_coords->xMin + (this->charge_coords->xMax - this->charge_coords->xMin) * percentage / 100;
}