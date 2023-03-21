#include "MotorAssistLevelDotsMonitorSmall.h"
#include "../../../themes/lv_theme.h"

MotorAssistLevelDotMonitorSmall::MotorAssistLevelDotMonitorSmall() {
	lv_style_init(&this->assist_line_style);	
	lv_style_set_line_color(&this->assist_line_style, lv_color_white());
	lv_style_set_line_width(&this->assist_line_style, DOT_SIZE);
	lv_style_set_line_rounded(&this->assist_line_style, DOT_ROUNDED);
}

lv_obj_t* MotorAssistLevelDotMonitorSmall::createLvObj(lv_obj_t* parent) {
	BaseMonitorSmall::createLvObj(parent);
	
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
