#ifndef _BUTTON_LABELLED_LV_OBJECT_H
#define _BUTTON_LABELLED_LV_OBJECT_H

#include "BaseLvObject.h"
#include "ButtonLabelBar.h"

/// <summary>
/// This is the base of all our LvObjects which should be built frist as an abstarct
/// collection then a call to createLvObj should instanciate the required lv objects.
/// </summary>
class ButtonLabelledLvObject : public BaseLvObject
{
protected:
    /// <summary>
    /// This is the group used for navigation between items
    /// </summary>
    lv_group_t* group = NULL;

    /// <summary>
    /// The indev required for giving context to the scroll menu group to the encoder
    /// </summary>
    lv_indev_t* indev = NULL;

    /// <summary>
    /// This is the LV instance of the encapulated functionality of this classes instance
    /// </summary>
    ButtonLabelBar* buttonLabelBar = NULL;
    /// <summary>
    /// This function is required to be overloaded and set the correct state and label of the button label bar for this lv object
    /// </summary>
    virtual void updateButtonLabelBar() = 0;

public:
    ButtonLabelledLvObject(lv_indev_t* indev, ButtonLabelBar* buttonLabelBar) { this->indev = indev; this->buttonLabelBar = buttonLabelBar; }

    /// <summary>
    /// Accessor to set the button label bar of this button labelled object
    /// </summary>
    void setButtonLabelBar(ButtonLabelBar* buttonLabelBar) { this->buttonLabelBar = buttonLabelBar; }

    /// <summary>
    /// Returns the button label bar of this lv object
    /// </summary>
    /// <returns>The LV object instance to represent this class instance</returns>
    ButtonLabelBar* getButtonLabelBar() { return this->buttonLabelBar;  }
};

#endif