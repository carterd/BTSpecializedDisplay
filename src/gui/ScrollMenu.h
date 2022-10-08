#ifndef SCROLL_MENU_H
#define SCROLL_MENU_H

#include <lvgl.h>
#include <vector>

#include "BaseLvObject.h"
#include "ScrollMenuItem.h"
#include "ButtonLabel.h"

class ScrollMenu : public BaseLvObject
{
private:
    lv_group_t* group;
    lv_obj_t* options_tileview_obj;
    lv_indev_t* indev;

    std::vector <ScrollMenuItem*> scrollMenuItems;
    ButtonLabel* buttonLabel = NULL;
    bool buttonLabelStartShown = false;
    bool focusAnimation = false;
    bool selectAnimation = false;
    int selectedItem;

public:
    /// <summary>
    /// Constructor for the scroll menu
    /// </summary>
    /// <param name="indev">The indev used for taking context of the encoder</param>
    ScrollMenu(lv_indev_t* indev);

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
    /// <param name="startShown">True if the button label is started shown</param>
    void setButtonLabel(ButtonLabel* buttonLabel, bool startShown = true);

    /// <summary>
    /// Add the given menu item objec to this list in the order called with this function
    /// </summary>
    /// <param name="scrollMenuItem">The menu item to add to the list</param>
    void addMenuItem(ScrollMenuItem* scrollMenuItem);

    /// <summary>
    /// Callback when a tile item button is clicked
    /// </summary>
    /// <param name="event"></param>
    void tileButtonCB(lv_event_t* event);

    /// <summary>
    /// Callback when a tile item button is de-focused
    /// </summary>
    /// <param name="event"></param>
    void tileButtonDefocusCB(lv_event_t* event);

    /// <summary>
    /// Callback when the scroll menu has moved onto a new item
    /// </summary>
    /// <param name="event"></param>
    void valueChangedCB(lv_event_t* event);

public:
    static void value_changed_cb(lv_event_t* event);
    static void tile_btn_cb(lv_event_t* event);
    static void tile_btn_defocus_cb(lv_event_t* event);
};

#endif