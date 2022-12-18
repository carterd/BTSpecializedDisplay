#ifndef SAVES_APPENDER_H
#define SAVES_APPENDER_H

#include <lvgl.h>

#include "ButtonLabelledLvObject.h"
#include "ButtonLabelBar.h"
#include "../dev/ConfigStore.h"

#define SAVING_STRING "Saving"

/// <summary>
/// The idea is this will popup when saving
/// </summary>
class SavesAppender : public ButtonLabelledLvObject
{
private:
    /// <summary>
    /// This is the LV background image
    /// </summary>
    lv_img_dsc_t* image;

    /// <summary>
    /// The connection button
    /// </summary>
    lv_obj_t* button_obj;
    /// <summary>
    /// The connection label
    /// </summary>
    lv_obj_t* label_obj;
    /// <summary>
    /// The Spinner graphic to show connecting activity
    /// </summary>
    lv_obj_t* spinner_obj;

    /// <summary>
    /// The config store which contains the bluetooth devices to connect to
    /// </summary>
    ConfigStore* configStore;

    /// <summary>
    /// The state of the connection is in connecting state
    /// </summary>
    bool connecting;

    /// <summary>
    /// The state of the connection is connected state
    /// </summary>
    bool connected;

    /// <summary>
    /// Ture when the exit button has been pressed and hence actively exiting the connection
    /// </summary>
    bool exiting;

    /// <summary>
    /// This is the object given focus on this object loosing focus
    /// </summary>
    BaseLvObject* defocusLvObj;

protected:
    virtual void updateButtonLabelBar();

public:
    /// <summary>
    /// The construct of for the bluetooth connection object
    /// </summary>
    /// <param name="bluetoothMaster">an instance of the bluetooth controller</param>
    /// <param name="configStore">an instance of the config store for devices to connect to</param>
    /// <param name="configStore">an image to be the background image while connecting</param>
    SavesAppender(ConfigStore* configStore, lv_img_dsc_t* image, lv_indev_t* indev, ButtonLabelBar* buttonLabelBar = NULL);

    virtual ~SavesAppender();

    /// <summary>
    /// Returns the LV object instance to represent this class instance
    /// </summary>
    /// <returns>The LV object instance to represent this class instance</returns>
    virtual lv_obj_t* createLvObj(lv_obj_t* parent);

    /// <summary>
    /// Destroy all the LV objects associated with the connection
    /// </summary>
    virtual void destroyLvObj();

    /// <summary>
    /// This means the object and any sub objects should set any groups to be in focus at this point
    /// </summary>
    virtual void focusLvObj(BaseLvObject* defocusLvObj = NULL);

    /// <summary>
    /// This is called when the user presses the exit button for the connection
    /// </summary>
    /// <param name="event">The event object for refresh being called into by the scanning</param>
    void exitButtonCB(lv_event_t* event = NULL);

public:
    static void exit_btn_cb(lv_event_t* event);
};

#endif