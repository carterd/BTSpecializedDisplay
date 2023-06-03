#include "ButtonLabelBar.h"
#include "../themes/display_theme.h"
#include "Arduino.h"

void ButtonLabelBar::auto_hide_timer_cb(lv_timer_t* timer) {
    ButtonLabelBar* buttonLabelBar = (ButtonLabelBar*) (timer->user_data);
    buttonLabelBar->autoHideTimerCB(timer);
}

ButtonLabelBar::ButtonLabelBar() : BaseLvObject() {
}

ButtonLabelBar::~ButtonLabelBar() {
    ButtonLabelBar::destroyLvObj();
}

lv_obj_t* ButtonLabelBar::createLvObj(lv_obj_t* parent) {

    // get the style we'll need for the bar
    display_theme_styles_t* display_theme_styles = (display_theme_styles_t*)lv_disp_get_theme(lv_obj_get_disp(parent))->user_data;
    lv_style_t* inv_style = &(display_theme_styles->inv);
    lv_style_t* no_scrollbar = &(display_theme_styles->no_scrollbar);
    lv_style_t* button_no_highlight = &(display_theme_styles->button_no_highlight);
    lv_style_t* button_label_bar = &(display_theme_styles->button_label_bar);

    // add button label bar
    this->this_obj = lv_obj_create(parent);
    lv_obj_set_width(this->this_obj, lv_obj_get_width(parent));
    lv_obj_add_style(this->this_obj, button_label_bar, LV_PART_MAIN);
    lv_obj_add_style(this->this_obj, no_scrollbar, LV_PART_SCROLLBAR);
    lv_obj_align(this->this_obj, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_update_layout(this->this_obj);
    this->barHeight = lv_obj_get_style_height(this->this_obj, LV_PART_MAIN);

    int partWidth = lv_obj_get_width(parent) / 3;

    // add button part on the left
    lv_obj_t* left_button_bar_part_obj = lv_obj_create(this_obj);
    lv_obj_set_width(left_button_bar_part_obj, partWidth);
    lv_obj_add_style(left_button_bar_part_obj, button_label_bar, LV_PART_MAIN);
    lv_obj_align(left_button_bar_part_obj, LV_ALIGN_TOP_LEFT, 0, 0);
    this->left_button_label_obj = lv_label_create(left_button_bar_part_obj);
    lv_obj_center(this->left_button_label_obj);

    // add button part in the center
    lv_obj_t* centre_button_bar_part_obj = lv_obj_create(this_obj);
    lv_obj_set_width(centre_button_bar_part_obj, partWidth);
    lv_obj_add_style(centre_button_bar_part_obj, button_label_bar, LV_PART_MAIN);
    lv_obj_align(centre_button_bar_part_obj, LV_ALIGN_TOP_MID, 0, 0);
    this->centre_button_label_obj = lv_label_create(centre_button_bar_part_obj);
    lv_obj_center(this->centre_button_label_obj);

    // add button part on the right
    lv_obj_t* right_button_bar_part_obj = lv_obj_create(this_obj);
    lv_obj_set_width(right_button_bar_part_obj, partWidth);
    lv_obj_add_style(right_button_bar_part_obj, button_label_bar, LV_PART_MAIN);
    lv_obj_align(right_button_bar_part_obj, LV_ALIGN_TOP_RIGHT, 0, 0);
    this->right_button_label_obj = lv_label_create(right_button_bar_part_obj);
    lv_obj_center(this->right_button_label_obj);

    setButtonLabels("", "", "");

    return this->this_obj;
}

void ButtonLabelBar::destroyLvObj() {
    BaseLvObject::destroyLvObj();
    this->left_button_label_obj = NULL;
    this->right_button_label_obj = NULL;
    this->centre_button_label_obj = NULL;
}

void ButtonLabelBar::focusLvObj(BaseLvObject* defocusLvObj)
{
    // we should not be focusing on the label bar
}

void ButtonLabelBar::setButtonLabels(const char* leftLabel, const char* centreLabel, const char* rightLabel)
{
    lv_label_set_text(this->left_button_label_obj, leftLabel);
    lv_label_set_text(this->centre_button_label_obj, centreLabel);
    lv_label_set_text(this->right_button_label_obj, rightLabel);
}

void ButtonLabelBar::hide()
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

void ButtonLabelBar::setHidden()
{
    this->hidden = true;
    lv_obj_set_height(this->this_obj, 0);
}

void ButtonLabelBar::setAutoHide(bool autoHide)
{
    if (autoHide) {
        if (!this->autoHide) {
            this->autoHide = true;
            this->setAutoHideTimer();
        }
    }
    else {
        // Even if a autohide timer is running then don't respond
        this->autoHide = false;
        this->auto_hide_timer = NULL;
    }

}

void ButtonLabelBar::show()
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
        lv_anim_set_values(&animation, lv_obj_get_height(this->this_obj), this->barHeight);
        lv_anim_set_time(&animation, 500);
        lv_anim_set_exec_cb(&animation, (lv_anim_exec_xcb_t)lv_obj_set_height);
        lv_anim_start(&animation);
    }
    if (this->autoHide) {
        this->setAutoHideTimer();
    }
}

void ButtonLabelBar::setShown()
{
    this->hidden = false;
    lv_obj_set_height(this->this_obj, this->barHeight);
}

void ButtonLabelBar::setAutoHideTimer()
{
    if (this->autoHide) {
        this->auto_hide_timer = lv_timer_create(auto_hide_timer_cb, AUTO_HIDE_TIMER_MS, this);
        lv_timer_set_repeat_count(this->auto_hide_timer, 1);
    }
}

void ButtonLabelBar::autoHideTimerCB(lv_timer_t* timer) 
{
    if (this->autoHide && this->auto_hide_timer == timer) {
        // Perform the auto hide function and keep defocus as the current value
        this->hide();
    }    
}