#include "MonitorLvObject.h"

MonitorLvObject::MonitorLvObject() {
    this->bluetoothBikeController = NULL;
}

void MonitorLvObject::setBluetoothController(BluetoothBikeController* bluetoothBikeController) {
    this->bluetoothBikeController = bluetoothBikeController;
}

lv_coord_t MonitorLvObject::getHeight() {
    return lv_obj_get_height(this->this_obj);;
}

lv_obj_t* MonitorLvObject::createLvObj(lv_obj_t* parent) {
	// get the style we'll need for the bar
	display_theme_styles_t* display_theme_styles = (display_theme_styles_t*)lv_disp_get_theme(lv_obj_get_disp(parent))->user_data;
	lv_style_t* monitor_panel_style = this->getMonitorPanelStyle(display_theme_styles);
		
	lv_obj_update_layout(parent);
	this->this_obj = lv_obj_create(parent);
	lv_obj_set_width(this->this_obj, lv_obj_get_width(parent));
	lv_obj_add_style(this->this_obj, monitor_panel_style, LV_PART_MAIN);
	lv_obj_set_align(this->this_obj, LV_ALIGN_CENTER);
	lv_obj_update_layout(this->this_obj);

    return this->this_obj;
}