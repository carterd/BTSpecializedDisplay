#include "TimeticksMonitorSmall.h"
#include "../../../themes/display_theme.h"

TimeticksMonitorSmall::TimeticksMonitorSmall(const char* attributeTitle) {
	this->attributeTitle = attributeTitle;
}

lv_obj_t* TimeticksMonitorSmall::createLvObj(lv_obj_t* parent) {
	BaseMonitorSmall::createLvObj(parent);

	lv_obj_t* label_obj = lv_label_create(this->this_obj);
	lv_obj_set_align(label_obj, LV_ALIGN_LEFT_MID);
	lv_label_set_text(label_obj, this->attributeTitle ? this->attributeTitle : "");

	this->value_obj = lv_label_create(this->this_obj);
	lv_obj_set_style_bg_color(this->value_obj, lv_color_black(), LV_PART_MAIN);
	lv_obj_set_style_bg_opa(this->value_obj, LV_OPA_COVER, LV_PART_MAIN);
	lv_obj_set_align(this->value_obj, this->attributeTitle ? LV_ALIGN_RIGHT_MID : LV_ALIGN_CENTER);
	return this->this_obj;
}

void TimeticksMonitorSmall::updateLvObj()
{
	char ticksString[32];
	long totalSeconds = millis() / 1000.0;
	int seconds = totalSeconds % 60;
	long totalMinutes = (totalSeconds - seconds) / 60;
	int minutes = totalMinutes % 60;
	int hours = (totalMinutes - minutes) / 60;
	sprintf(ticksString, "%02d:%02d:%02d", hours, minutes, seconds);
	lv_label_set_text(this->value_obj, ticksString);
}

void TimeticksMonitorSmall::initBluetoothStats()
{
	this->bluetoothBike->readBikeStateAttribute(BikeStateAttributeIndex::BIKE_ON_OFF_STATE, MonitorAttributeType::EVERY_SECOND);
}