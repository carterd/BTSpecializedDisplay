#include "SavesAppender.h"
#include "../themes/display_theme.h"

void SavesAppender::exit_btn_cb(lv_event_t* event) {
    ((SavesAppender*)(event->user_data))->exitButtonCB(event);
}

SavesAppender::SavesAppender(ConfigStore* configStore, lv_img_dsc_t* image, lv_indev_t* indev, ButtonLabelBar* buttonLabelBar)  : ButtonLabelledLvObject(indev, buttonLabelBar)
{
	this->configStore = configStore;
	this->image = image;
    this->connecting = false;
    this->connected = false;
    this->exiting = false;
}

SavesAppender::~SavesAppender()
{
}

lv_obj_t* SavesAppender::createLvObj(lv_obj_t* parent)
{
    // get the style we'll need for the bar
    display_theme_styles_t* binary_styles = (display_theme_styles_t*)lv_disp_get_theme(lv_obj_get_disp(parent))->user_data;
    lv_style_t* no_scrollbar = &(binary_styles->no_scrollbar);
    lv_style_t* button_no_highlight_style = &(binary_styles->button_no_highlight);
    lv_style_t* button_style = &(binary_styles->button);

    // create a group for the button for exit connection
    this->group = lv_group_create();

    lv_obj_update_layout(parent);
    this->this_obj = lv_obj_create(parent);
    lv_obj_add_style(this->this_obj, no_scrollbar, LV_PART_SCROLLBAR);
    lv_obj_set_size(this->this_obj, lv_obj_get_width(parent), lv_obj_get_height(parent));
    lv_obj_set_align(this->this_obj, LV_ALIGN_CENTER);

    // Create a button
    this->button_obj = lv_btn_create(this->this_obj);
    lv_obj_center(this->button_obj);
    lv_obj_add_style(this->button_obj, button_no_highlight_style, LV_PART_MAIN);
    lv_obj_add_event_cb(this->button_obj, SavesAppender::exit_btn_cb, LV_EVENT_CLICKED, this);
    lv_group_add_obj(this->group, this->button_obj);
    
    // Associate an image
    lv_obj_t* img_obj = lv_img_create(this->button_obj);
    lv_img_set_src(img_obj, this->image);
    lv_obj_center(img_obj);

    // Associate a spinner
    this->spinner_obj = lv_spinner_create(this->this_obj, 1000, 60);
    lv_obj_set_size(this->spinner_obj, 60, 60);
    lv_obj_center(this->spinner_obj);

    // Associate a label connecting
    this->label_obj = lv_label_create(this->this_obj);
    lv_label_set_text(this->label_obj, SAVING_STRING);
    lv_obj_add_style(this->label_obj, button_style, LV_PART_MAIN);
    lv_obj_center(this->label_obj);
    
    // Return the connection lv_obj just in case
    return this->this_obj;
}

void SavesAppender::destroyLvObj() {
    if (this->group) lv_group_del(this->group);
    this->group = NULL;
    BaseLvObject::destroyLvObj();
}

void SavesAppender::focusLvObj(BaseLvObject* defocusLvObj)
{
    this->updateButtonLabelBar();

    // Capture movement from here on in
    lv_indev_set_group(this->indev, this->group);
    lv_group_focus_obj(this->button_obj);

    this->exiting = false;
    this->defocusLvObj = defocusLvObj;
}

void SavesAppender::updateButtonLabelBar() {
    if (this->buttonLabelBar) {
        this->buttonLabelBar->setButtonLabels("", LV_SYMBOL_CLOSE, "");
        this->buttonLabelBar->setAutoHide(false);
    }
}

void SavesAppender::exitButtonCB(lv_event_t* event)
{
    this->exiting = true;
    this->defocusLvObj->focusLvObj();
    // Check which screen is being shown if we're on the wrong screen then perform switch to exit screen
}