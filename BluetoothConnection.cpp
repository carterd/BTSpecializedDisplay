#include "BluetoothConnection.h"
#include <LvglThemes/lv_theme_binary.h>

BluetoothConnection::BluetoothConnection(BluetoothMaster* bluetoothMaster, ConfigStore* configStore, lv_img_dsc_t* image) {
	this->bluetoothMaster = bluetoothMaster;
	this->configStore = configStore;
	this->image = image;
}

lv_obj_t* BluetoothConnection::createLvObj(lv_obj_t* parent)
{
    // get the style we'll need for the bar
    theme_binary_styles_t* binary_styles = (theme_binary_styles_t*)lv_disp_get_theme(lv_obj_get_disp(parent))->user_data;
    lv_style_t* no_scrollbar = &(binary_styles->no_scrollbar);
    lv_style_t* button_no_highlight_style = &(binary_styles->button_no_highlight);

    // Create a button
    this->this_obj = lv_btn_create(parent);
    lv_obj_center(this->this_obj);
    lv_obj_add_style(this->this_obj, button_no_highlight_style, LV_PART_MAIN);

    // Associate an image
    lv_obj_t* img_obj = lv_img_create(this->this_obj);
    lv_img_set_src(img_obj, this->image);
    lv_obj_center(img_obj);

    // Associate a spinner
    lv_obj_t* spin_obj = lv_spinner_create(parent, 1000, 60);
    lv_obj_set_size(spin_obj, 60, 60);
    lv_obj_center(spin_obj);

    // Return the button just in case
    return this->this_obj;
}

void BluetoothConnection::setButtonLabel(ButtonLabel* buttonLabel)
{
	this->buttonLabel = buttonLabel;
}

void BluetoothConnection::focusLvObj()
{	
	hideButtonLabels();
}

void BluetoothConnection::hideButtonLabels()
{
	if (this->buttonLabel) {
        this->buttonLabel->focusLvObj();
		this->buttonLabel->hide();
	}
}