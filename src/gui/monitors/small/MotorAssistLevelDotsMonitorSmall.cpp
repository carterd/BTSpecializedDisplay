#include "MotorAssistLevelDotsMonitorSmall.h"
#include "../../../themes/lv_theme.h"

const assist_coords_t MotorAssistLevelDotMonitorSmall::assist_coords_sh1107 = {10, 22, 8};
const assist_coords_t MotorAssistLevelDotMonitorSmall::assist_coords_tdisplay = {20, 50, 10};
const assist_coords_t MotorAssistLevelDotMonitorSmall::assist_coords_tdisplay_s3 = {25, 55, 20};

MotorAssistLevelDotMonitorSmall::MotorAssistLevelDotMonitorSmall() {
	lv_style_init(&this->assist_line_style);	
	lv_style_set_line_color(&this->assist_line_style, lv_color_white());
	lv_style_set_line_width(&this->assist_line_style, DOT_SIZE);
	lv_style_set_line_rounded(&this->assist_line_style, DOT_ROUNDED);
}

lv_obj_t* MotorAssistLevelDotMonitorSmall::createLvObj(lv_obj_t* parent) {
	BaseMonitorSmall::createLvObj(parent);
	display_theme_styles_t* display_theme_styles = (display_theme_styles_t*)lv_disp_get_theme(lv_obj_get_disp(parent))->user_data;
	lv_style_t* small_assist_dot_line_style = &(display_theme_styles->small_assist_dot_line);

	const assist_coords_t* assist_coords = display_theme_styles->small_assist_coords;
	lv_coord_t width = lv_obj_get_width( this->this_obj);

	for (int i = 0; i < 3; i++) {
		this->assist_line_points[i][0] = { (lv_coord_t)((i - 1) * assist_coords->xStep + (width - assist_coords->xSize) / 2), assist_coords->yPos }; 
		this->assist_line_points[i][1] = { (lv_coord_t)((i - 1) * assist_coords->xStep + (width + assist_coords->xSize) / 2), assist_coords->yPos }; 
		this->levels[i] = lv_line_create(this->this_obj);
		lv_obj_add_style(this->levels[i], small_assist_dot_line_style, LV_PART_MAIN);
		lv_line_set_points(this->levels[i], this->assist_line_points[i], 2);
		lv_obj_add_flag(this->levels[i], LV_OBJ_FLAG_HIDDEN);
	}
	
	return this->this_obj;
}

void MotorAssistLevelDotMonitorSmall::statusUpdate()
{
	if (this->bluetoothBikeController && this->bluetoothBikeController->getConnected()) {
		// This bleDevice should always be true as we've already idenfied scanned device available
		if (this->bluetoothBikeController->getBikeState().getStateAttribute(BikeStateAttributeIndex::MOTOR_ASSIST_LEVEL).bikeStateAttributeValue.valueUint16 != this->displayedMotorAssistLevel ||
			this->bluetoothBikeController->getBikeState().getStateAttribute(BikeStateAttributeIndex::BIKE_ON_OFF_STATE).bikeStateAttributeValue.valueUint8 != this->displayedBikeOnOffState) {
			this->displayedMotorAssistLevel = this->bluetoothBikeController->getBikeState().getStateAttribute(BikeStateAttributeIndex::MOTOR_ASSIST_LEVEL).bikeStateAttributeValue.valueUint16;
			this->displayedBikeOnOffState = this->bluetoothBikeController->getBikeState().getStateAttribute(BikeStateAttributeIndex::BIKE_ON_OFF_STATE).bikeStateAttributeValue.valueUint8;
			MonitorLvObject::statusUpdate();
		}
		// We should use a counter to keep battery upto date ... as probaby needs explicit read from time to time
	}
	else 
	{
		// Nolonger connected Requred to scan again for connection
	}
}

void MotorAssistLevelDotMonitorSmall::initBluetoothStats()
{
	this->bluetoothBikeController->getConnectedBluetoothBike().readBikeStateAttribute(BikeStateAttributeIndex::MOTOR_ASSIST_LEVEL, MonitorAttributeType::EVERY_MINUTE);
	this->bluetoothBikeController->getConnectedBluetoothBike().readBikeStateAttribute(BikeStateAttributeIndex::BIKE_ON_OFF_STATE, MonitorAttributeType::EVERY_TEN_SECONDS);
}

void MotorAssistLevelDotMonitorSmall::updateLvObj() {
	for (int i = 0; i < 3; i++) {
		lv_obj_t* level = this->levels[i];
		if (this->displayedMotorAssistLevel <= i || this->displayedBikeOnOffState == 0x00) {
			lv_obj_add_flag(this->levels[i], LV_OBJ_FLAG_HIDDEN);
		} else {
			lv_obj_clear_flag(this->levels[i], LV_OBJ_FLAG_HIDDEN);
		}
	}
}
