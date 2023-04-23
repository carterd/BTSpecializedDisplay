#ifndef _MAIN_VIEW_H
#define _MAIN_VIEW_H

#include <lvgl.h>
#include <ArduinoBLE.h>

#include "BaseLvObject.h"
#include "ButtonLabelledLvObject.h"
#include "ButtonLabelBar.h"
#include "../dev/BluetoothBikeDefinitions.h"
#include "../dev/BluetoothBikeState.h"

class MainView : public ButtonLabelledLvObject
{
protected:
    /// <summary>
    /// This 
    /// </summary>
    ButtonLabelledLvObject* buttonLabelledLvObject;

    /// <summary>
    /// This function is required to be overloaded and set the correct state and label of the button label bar for this lv object
    /// </summary>
    virtual void updateButtonLabelBar() {}
public:
    /// <summary>
    /// Constructor for the main view object
    /// </summary>
    MainView(lv_indev_t* indev, ButtonLabelBar* buttonLabelBar, BaseLvObject* baseLvObject = NULL);

    /// <summary>
    /// Destructor to ensure the root lv object is deleted
    /// </summary>
    virtual ~MainView();

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
    /// This is an optional setter for the baseLvObject
    /// </summary>
    void setBaseLvObject(ButtonLabelledLvObject* buttonLabelledLvObject) { this->buttonLabelledLvObject = buttonLabelledLvObject;  }
};

#endif