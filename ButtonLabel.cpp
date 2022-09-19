#include <LvglThemes/lv_theme_binary.h>

#include "ButtonLabel.h"

static void lv_event_encorder_cb(lv_event_t* e) {
    ButtonLabel* buttonLabel = (ButtonLabel*) (e->user_data);
    buttonLabel->show();
}

ButtonLabel::ButtonLabel(lv_indev_t* indev) : BaseLvObject()
{
    this->indev = indev;
    this->group = lv_group_create();

}

lv_obj_t* ButtonLabel::createLvObj(lv_obj_t* parent) {
    // get the style we'll need for the bar
    theme_binary_styles_t* binary_styles = (theme_binary_styles_t*)lv_disp_get_theme(lv_obj_get_disp(parent))->user_data;
    lv_style_t* inv_style = &(binary_styles->inv);
    lv_style_t* no_scrollbar = &(binary_styles->no_scrollbar);
    lv_style_t* button_no_highlight = &(binary_styles->button_no_highlight);

    // add button label bar
    this->this_obj = lv_obj_create(parent);
    lv_obj_set_size(this->this_obj, lv_obj_get_width(parent), BUTTON_LABEL_BAR_HEIGHT);
    lv_obj_add_style(this->this_obj, inv_style, LV_PART_MAIN);
    lv_obj_add_style(this->this_obj, no_scrollbar, LV_PART_SCROLLBAR);
    lv_obj_align(this->this_obj, LV_ALIGN_BOTTOM_MID, 0, 0);

    // add two activity button object to identify if encoder is being used
    this->activity_btn_1_obj = lv_btn_create(this->this_obj);
    lv_obj_add_style(this->activity_btn_1_obj, button_no_highlight, LV_PART_MAIN);
    lv_obj_add_event_cb(this->activity_btn_1_obj, lv_event_encorder_cb, LV_EVENT_CLICKED, this);
    lv_obj_add_event_cb(this->activity_btn_1_obj, lv_event_encorder_cb, LV_EVENT_DEFOCUSED, this);
    lv_group_add_obj(this->group, this->activity_btn_1_obj);

    this->activity_btn_2_obj = lv_btn_create(this->this_obj);
    lv_obj_add_style(this->activity_btn_2_obj, button_no_highlight, LV_PART_MAIN);
    lv_obj_add_event_cb(this->activity_btn_2_obj, lv_event_encorder_cb, LV_EVENT_CLICKED, this);
    lv_obj_add_event_cb(this->activity_btn_1_obj, lv_event_encorder_cb, LV_EVENT_DEFOCUSED, this);
    lv_group_add_obj(this->group, this->activity_btn_2_obj);

    int partWidth = lv_obj_get_width(parent) / 3;

    // add button part on the left
    lv_obj_t* left_button_bar_part_obj = lv_obj_create(this_obj);
    lv_obj_set_size(left_button_bar_part_obj, partWidth, BUTTON_LABEL_BAR_HEIGHT);
    lv_obj_align(left_button_bar_part_obj, LV_ALIGN_TOP_LEFT, 0, 0);
    this->left_button_label_obj = lv_label_create(left_button_bar_part_obj);
    lv_obj_center(this->left_button_label_obj);

    // add button part in the center
    lv_obj_t* centre_button_bar_part_obj = lv_obj_create(this_obj);
    lv_obj_set_size(centre_button_bar_part_obj, partWidth, BUTTON_LABEL_BAR_HEIGHT);
    lv_obj_align(centre_button_bar_part_obj, LV_ALIGN_TOP_MID, 0, 0);
    this->centre_button_label_obj = lv_label_create(centre_button_bar_part_obj);
    lv_obj_center(this->centre_button_label_obj);

    // add button part on the right
    lv_obj_t* right_button_bar_part_obj = lv_obj_create(this_obj);
    lv_obj_set_size(right_button_bar_part_obj, partWidth, BUTTON_LABEL_BAR_HEIGHT);
    lv_obj_align(right_button_bar_part_obj, LV_ALIGN_TOP_RIGHT, 0, 0);
    this->right_button_label_obj = lv_label_create(right_button_bar_part_obj);
    lv_obj_center(this->right_button_label_obj);

    return this->this_obj;
}

void ButtonLabel::focusLvObj()
{
    // Esure that on of the activit buttons are active    
    lv_group_focus_obj(this->activity_btn_1_obj);
    lv_indev_set_group(this->indev, this->group);
}

void ButtonLabel::setButtonLabels(const char* leftLabel, const char* centreLabel, const char* rightLabel)
{
    lv_label_set_text(this->left_button_label_obj, leftLabel);
    lv_label_set_text(this->centre_button_label_obj, centreLabel);
    lv_label_set_text(this->right_button_label_obj, rightLabel);
}

void ButtonLabel::hide()
{
    if (!this->hidden) {
        // Lets set to hidden so can be undone by show
        this->hidden = true;

        // check we have no running animation
        lv_anim_del(this->this_obj, (lv_anim_exec_xcb_t)lv_obj_set_height);

        // Set up the animation to hide the button label bar
        lv_anim_t animation;
        lv_anim_init(&animation);
        lv_anim_set_var(&animation, this->this_obj);
        lv_anim_set_values(&animation, lv_obj_get_height(this->this_obj), 0);
        lv_anim_set_time(&animation, 500);
        lv_anim_set_exec_cb(&animation, (lv_anim_exec_xcb_t)lv_obj_set_height);
        lv_anim_start(&animation);
    }
}

void ButtonLabel::setHidden()
{
    this->hidden = true;
    lv_obj_set_height(this->this_obj, 0);
}

void ButtonLabel::show()
{
    if (this->hidden) {
        // On being activated disable activity and bring up the labels
        this->hidden = false;

        // check we have no running animation
        lv_anim_del(this->this_obj, (lv_anim_exec_xcb_t)lv_obj_set_height);

        // Set up the animation to show the button label bar
        lv_anim_t animation;
        lv_anim_init(&animation);
        lv_anim_set_var(&animation, this->this_obj);
        lv_anim_set_values(&animation, lv_obj_get_height(this->this_obj), BUTTON_LABEL_BAR_HEIGHT);
        lv_anim_set_time(&animation, 500);
        lv_anim_set_exec_cb(&animation, (lv_anim_exec_xcb_t)lv_obj_set_height);
        lv_anim_start(&animation);
    }
}

void ButtonLabel::setShown()
{
    this->hidden = false;
    lv_obj_set_height(this->this_obj, BUTTON_LABEL_BAR_HEIGHT);
}

