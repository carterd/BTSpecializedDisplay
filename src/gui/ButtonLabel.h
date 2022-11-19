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
    lv_timer_t* auto_hide_timer;

    BaseLvObject* defocusLvObj;

    bool hidden = false;

    bool autoHide = false;
    
public:
    /// <summary>
    /// Constructor of the button label strip
    /// </summary>
    /// <param name="indev">The indev used for taking context of the encoder</param>
    ButtonLabel(lv_indev_t* indev);

    /// <summary>
    /// The Destructor we have lv_groups and lv_objs that potentially need cleaning up
    /// </summary>
    virtual ~ButtonLabel();

    /// <summary>
    /// This Creates the instance of the Lv Objects associated with the instance and sub components associated with it
    /// </summary>
    /// <param name="parent">The LV object parent on which to attach the created lv object for this instance</param>
    /// <returns>The created LV object instance</returns>
    virtual lv_obj_t* createLvObj(lv_obj_t* parent);

    /// <summary>
    /// This will attempt to destroy all the Lv Objects associated with the instance and sub components associcated with it
    /// </summary>
    virtual void destroyLvObj();

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

    /// <summary>
    /// Sets the label to auto hide after a time period
    /// <summary>
    void setAutoHide(bool autoHide);

    /// <summary>
    /// This is the callback when any events detected on the encoder
    /// </summary>
    /// <param name="event"></param>
    void encorderActivityCB(lv_event_t* event);

    /// <summary>
    /// This is a timer callback for a timer that auto hides the button
    /// </summary>
    /// <param name="timer"></param>
    void autoHideTimerCB(lv_timer_t* timer);

public:
    /// <summary>
    /// The static callback for the any events detected on the encoder
    /// </summary>
    /// <param name="event"></param>
    static void lv_event_encorder_cb(lv_event_t* event);

    /// <summary>
    /// The static callback for the hide timer event
    /// </summary>
    /// <param name="timer"></param>
    static void auto_hide_timer_cb(lv_timer_t * timer);
};

#endif