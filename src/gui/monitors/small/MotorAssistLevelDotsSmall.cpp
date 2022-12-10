#include "MotorAssistLevelDotsSmall.h"

#include <LvglThemes/lv_theme_binary.h>


MotorAssistLevelDotSmall::MotorAssistLevelDotSmall() {
	lv_style_init(&this->assist_line_style);	
	lv_style_set_line_color(&this->assist_line_style, lv_color_white());
	lv_style_set_line_width(&this->assist_line_style, DOT_SIZE);
	lv_style_set_line_rounded(&this->assist_line_style, DOT_ROUNDED);
}

lv_obj_t* MotorAssistLevelDotSmall::createLvObj(lv_obj_t* parent) {
	// get the style we'll need for the bar
	theme_binary_styles_t* binary_styles = (theme_binary_styles_t*)lv_disp_get_theme(lv_obj_get_disp(parent))->user_data;
	lv_style_t* no_scrollbar = &(binary_styles->no_scrollbar);
	lv_style_t* card_style = &(binary_styles->card);
		
	lv_obj_update_layout(parent);
	this->this_obj = lv_obj_create(parent);
	lv_obj_set_size(this->this_obj, lv_obj_get_width(parent), lv_obj_get_height(parent));
	lv_obj_set_align(this->this_obj, LV_ALIGN_CENTER);
	
	for (int i = 0; i < 3; i++) {
		this->assist_line_points[i][0] = { (lv_coord_t)(i * 22 + 5), 8 };
		this->assist_line_points[i][1] = { (lv_coord_t)(i * 22 + 15), 8 };
		this->levels[i] = lv_line_create(this->this_obj);
		lv_obj_add_style(this->levels[i], &this->assist_line_style, LV_PART_MAIN);
		lv_line_set_points(this->levels[i], this->assist_line_points[i], 2);
		lv_obj_add_flag(this->levels[i], LV_OBJ_FLAG_HIDDEN);
	}
	
	return this->this_obj;
}

void MotorAssistLevelDotSmall::statusUpdate()
{
	if (this->bluetoothBikeController && this->bluetoothBikeController->getConnected()) {
		// This bleDevice should always be true as we've already idenfied scanned device available
		if (this->bluetoothBikeController->getBikeState().getStateAttribute(BikeStateAttributeIndex::MOTOR_ASSIST_LEVEL).bikeStateAttributeValue.valueUint16 != this->displayedMotorAssistLevel ||
			this->bluetoothBikeController->getBikeState().getStateAttribute(BikeStateAttributeIndex::BIKE_ON_OFF_STATE).bikeStateAttributeValue.valueUint8 != this->displayedBikeOnOffState) {
			this->update();
		}
		// We should use a counter to keep battery upto date ... as probaby needs explicit read from time to time
	}
	else 
	{
		// Nolonger connected Requred to scan again for connection
	}
}

void MotorAssistLevelDotSmall::initBluetoothStats()
{
	this->bluetoothBikeController->getConnectedBluetoothBike().readBikeStateAttribute(BikeStateAttributeIndex::MOTOR_ASSIST_LEVEL, MonitorAttributeType::EVERY_MINUTE);
	this->bluetoothBikeController->getConnectedBluetoothBike().readBikeStateAttribute(BikeStateAttributeIndex::BIKE_ON_OFF_STATE, MonitorAttributeType::EVERY_TEN_SECONDS);
}

void MotorAssistLevelDotSmall::update() {
	this->displayedMotorAssistLevel = this->bluetoothBikeController->getBikeState().getStateAttribute(BikeStateAttributeIndex::MOTOR_ASSIST_LEVEL).bikeStateAttributeValue.valueUint16;	
	this->displayedBikeOnOffState = this->bluetoothBikeController->getBikeState().getStateAttribute(BikeStateAttributeIndex::BIKE_ON_OFF_STATE).bikeStateAttributeValue.valueUint8;

	for (int i = 0; i < 3; i++) {
		lv_obj_t* level = this->levels[i];
		if (this->displayedMotorAssistLevel <= i || this->displayedBikeOnOffState == 0x00) {
			lv_obj_add_flag(this->levels[i], LV_OBJ_FLAG_HIDDEN);
		} else {
			lv_obj_clear_flag(this->levels[i], LV_OBJ_FLAG_HIDDEN);
		}
	}
}
