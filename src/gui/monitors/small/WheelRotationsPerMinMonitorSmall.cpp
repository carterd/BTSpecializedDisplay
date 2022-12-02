#include "WheelRotationsPerMinMonitorSmall.h"

#include <LvglThemes/lv_theme_binary.h>


WheelRotationsPerMinMonitorSmall::WheelRotationsPerMinMonitorSmall() {
    this->wheelCircumferenceMm = 0;
}

lv_obj_t* WheelRotationsPerMinMonitorSmall::createLvObj(lv_obj_t* parent) {    
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
    lv_label_set_text(label_obj, "km/h:");

    this->value_obj = lv_label_create(this->this_obj);
    lv_obj_set_style_bg_color(this->value_obj, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(this->value_obj, LV_OPA_COVER, LV_PART_MAIN);
	lv_obj_set_align(this->value_obj, LV_ALIGN_RIGHT_MID);

	return this->this_obj;
}

void WheelRotationsPerMinMonitorSmall::focusLvObj(BaseLvObject* defocusLvObj)
{    
	// The LVObj that'll get the refreshes and should there hook into updates
    this->bluetoothBikeController->getConnectedBluetoothBike().readBikeStateAttribute(BikeStateAttributeIndex::WHEEL_CIRCUMFERENCE, MonitorAttributeType::ONCE);
    this->bluetoothBikeController->getConnectedBluetoothBike().readBikeStateAttribute(BikeStateAttributeIndex::WHEEL_ROTATIONS_PER_MIN, MonitorAttributeType::EVERY_SECOND);
	this->bluetoothBikeController->getConnectedBluetoothBike().readBikeStateAttribute(BikeStateAttributeIndex::WHEEL_ROTATIONS, MonitorAttributeType::EVERY_TEN_SECONDS);    
    this->wheelCircumferenceMm = this->bluetoothBikeController->getBikeState().getStateAttribute(BikeStateAttributeIndex::WHEEL_CIRCUMFERENCE).bikeStateAttributeValue.valueUint16;
	this->update();
}

void WheelRotationsPerMinMonitorSmall::statusUpdate()
{
    this->update();
}

void WheelRotationsPerMinMonitorSmall::update() {
    char valueString[32];
    float kmph = this->bluetoothBikeController->getBikeState().getStateAttribute(BikeStateAttributeIndex::WHEEL_ROTATIONS_PER_MIN).bikeStateAttributeValue.valueFloat * this->wheelCircumferenceMm * 60.0f / 1000000.0f;
    float mph = kmph * 0.621371;
    sprintf(valueString, "%.1f", kmph );
    lv_label_set_text(this->value_obj, valueString);
}
