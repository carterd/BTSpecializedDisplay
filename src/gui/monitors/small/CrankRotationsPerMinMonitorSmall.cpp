#include "CrankRotationsPerMinMonitorSmall.h"

#include <LvglThemes/lv_theme_binary.h>


CrankRotationsPerMinMonitorSmall::CrankRotationsPerMinMonitorSmall() {
}

lv_obj_t* CrankRotationsPerMinMonitorSmall::createLvObj(lv_obj_t* parent) {    
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
    lv_label_set_text(label_obj, "Cad:");

    this->value_obj = lv_label_create(this->this_obj);
    lv_obj_set_style_bg_color(this->value_obj, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(this->value_obj, LV_OPA_COVER, LV_PART_MAIN);
	lv_obj_set_align(this->value_obj, LV_ALIGN_RIGHT_MID);

	return this->this_obj;
}

void CrankRotationsPerMinMonitorSmall::focusLvObj(BaseLvObject* defocusLvObj)
{
	// The LVObj that'll get the refreshes and should there hook into updates
    this->bluetoothBikeController->getConnectedBluetoothBike().readBikeStateAttribute(BikeStateAttributeIndex::CRANK_ROTATIONS_PER_MIN, MonitorAttributeType::EVERY_SECOND);
	this->bluetoothBikeController->getConnectedBluetoothBike().readBikeStateAttribute(BikeStateAttributeIndex::CRANK_ROTATIONS, MonitorAttributeType::EVERY_TEN_SECONDS);
	this->update();
}

void CrankRotationsPerMinMonitorSmall::statusUpdate()
{
    this->update();
}

void CrankRotationsPerMinMonitorSmall::update() {
    char ticksString[32];
    sprintf(ticksString, "%.1f", this->bluetoothBikeController->getBikeState().getStateAttribute(BikeStateAttributeIndex::CRANK_ROTATIONS_PER_MIN).bikeStateAttributeValue.valueFloat);
    lv_label_set_text(this->value_obj, ticksString);
}
