#ifndef _BUTTON_LABEL_H
#define _BUTTON_LABEL_H

#include <lvgl.h>

#include "BaseLvObject.h"

#define BUTTON_LABEL_BAR_HEIGHT 10

/// <summary>
/// This is the Button Label that identifies what buttons do
/// </summary>
class ButtonLabel : public BaseLvObject {
private:

    lv_obj_t* activity_btn_1_obj;
    lv_obj_t* activity_btn_2_obj;
    lv_obj_t* left_button_label_obj;
    lv_obj_t* right_button_label_obj;
    lv_obj_t* centre_button_label_obj;
    lv_group_t* group;

    bool hidden = false;
    lv_indev_t* indev;
    
public:
    ButtonLabel(lv_indev_t* indev);
    lv_obj_t* createLvObj(lv_obj_t* parent);
    void focusLvObj();
    void setButtonLabels(const char* leftLabel, const char* centreLabel, const char* rightLabel);
    void show();
    void setShown();
    void hide();
    void setHidden();
};

#endif