#include "TimeticksMonitorSmall.h"

#include <LvglThemes/lv_theme_binary.h>


TimeticksMonitorSmall::TimeticksMonitorSmall() {
}

lv_obj_t* TimeticksMonitorSmall::createLvObj(lv_obj_t* parent) {    
	// get the style we'll need for the bar
	theme_binary_styles_t* binary_styles = (theme_binary_styles_t*)lv_disp_get_theme(lv_obj_get_disp(parent))->user_data;
	lv_style_t* no_scrollbar = &(binary_styles->no_scrollbar);
	lv_style_t* card_style = &(binary_styles->card);
		
	lv_obj_update_layout(parent);
	this->this_obj = lv_obj_create(parent);
	lv_obj_set_size(this->this_obj, lv_obj_get_width(parent), lv_obj_get_height(parent));
	lv_obj_set_align(this->this_obj, LV_ALIGN_CENTER);

	lv_obj_t* label_obj = lv_label_create(this->this_obj);
	lv_obj_set_align(label_obj, LV_ALIGN_LEFT_MID);
	lv_label_set_text(label_obj, "Clk:");

	this->value_obj = lv_label_create(this->this_obj);
	lv_obj_set_style_bg_color(this->value_obj, lv_color_black(), LV_PART_MAIN);
	lv_obj_set_style_bg_opa(this->value_obj, LV_OPA_COVER, LV_PART_MAIN);
	lv_obj_set_align(this->value_obj, LV_ALIGN_RIGHT_MID);
	return this->this_obj;
}

void TimeticksMonitorSmall::focusLvObj(BaseLvObject* defocusLvObj)
{
	// The LVObj that'll get the refreshes and should there hook into updates
	this->bluetoothBikeController->getConnectedBluetoothBike().readBikeStateAttribute(BikeStateAttributeIndex::BIKE_ON_OFF_STATE, MonitorAttributeType::EVERY_SECOND);
	this->update();
}

void TimeticksMonitorSmall::statusUpdate()
{
    this->update();
}

void TimeticksMonitorSmall::update() {
    char ticksString[32];
	long totalSeconds = millis() / 1000.0;
	int seconds = totalSeconds % 60;
	long totalMinutes = (totalSeconds - seconds) / 60;
	int minutes = totalMinutes % 60;
	int hours = (totalMinutes - minutes) / 60;
    sprintf(ticksString, "%02d:%02d:%02d", hours, minutes, seconds);
    lv_label_set_text(this->value_obj, ticksString);
}
