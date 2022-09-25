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
    lv_indev_t* indev;

    BaseLvObject* defocusLvObj;

    bool hidden = false;
    
public:
    /// <summary>
    /// Constructor of the button label strip
    /// </summary>
    /// <param name="indev">The indev used for taking context of the encoder</param>
    ButtonLabel(lv_indev_t* indev);

    /// <summary>
    /// This Creates the instance of the Lv Objects associated with the instance and sub components associated with it
    /// </summary>
    /// <param name="parent">The LV object parent on which to attach the created lv object for this instance</param>
    /// <returns>The created LV object instance</returns>
    virtual lv_obj_t* createLvObj(lv_obj_t* parent);

    /// <summary>
    /// This means the object and any sub objects should set any groups to be in focus at this point
    /// </summary>
    virtual void focusLvObj(BaseLvObject* defocusLvObj = NULL);

    /// <summary>
    /// Used to set the button label for this object or NULL for no button label
    /// </summary>
    /// <param name="buttonLabel">The button label object for this gui object</param>
    void setButtonLabels(const char* leftLabel, const char* centreLabel, const char* rightLabel);

    /// <summary>
    /// Starts the show animation for the label
    /// </summary>
    void show();

    /// <summary>
    /// Sets the label to be shown
    /// </summary>
    void setShown();

    /// <summary>
    /// Starts the hide animation for the label
    /// </summary>
    void hide();

    /// <summary>
    /// Sets the label to be hidden
    /// </summary>
    void setHidden();

    void encorderActivityCB(lv_event_t* event);
};

#endif