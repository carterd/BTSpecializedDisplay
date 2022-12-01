#ifndef SCROLL_MENU_ITEM_H
#define SCROLL_MENU_ITEM_H

#include <lvgl.h>

#include "BaseLvObject.h"

class ScrollMenuItem : public BaseLvObject
{
private:
    lv_img_dsc_t* image;

    const char* itemText;
    BaseLvObject* popupItem;
    bool animatedSelect;

public:
    /// <summary>
    /// This is the constructor of the menu item
    /// </summary>
    /// <param name="image">The menu items image</param>
    /// <param name="animatedSelect">Boolean specifies if animation is required to select the item</param>
    ScrollMenuItem(lv_img_dsc_t* image, bool animatedSelect = true);

    ScrollMenuItem(const char* itemText, bool animatedSelect = true);

    virtual ~ScrollMenuItem();

    virtual lv_obj_t* createLvObj(lv_obj_t* parent);

    virtual void focusLvObj(BaseLvObject* defocusLvObj = NULL);
    lv_img_dsc_t* getLvImage();
    const char* getItemText() {
        return this->itemText;
    }
    void setPopupItem(BaseLvObject* popupItem);
    BaseLvObject* getPopupItem();
    bool getAnimatedSelect();

    /// <summary>
    /// It maybe possible for text menu items to be checkable this returns the current checked state of the menuItem
    /// </summary>
    virtual bool isChecked() {
        return false;
    }
    /// <summary>
    /// This function will be called by navigation menu if there is no popup item for this menu item to perform an action rather navigation.
    /// </summary>
    virtual void noPopupAction() {
        // default is do no action
    }
    /// <summary>
    /// It's possible that the scroll menu item with navigation has some state that needs initialising for display purposes, (i.e. pre focusLvObj)
    /// </summary>
    virtual void menuItemInitCB() {
        // default is do no action
    }
    /// <summary>
    /// It's possible that the scroll menu item with navigation has some state that needs setting before exiting the menu to store state
    /// </summary>
    virtual void menuItemFinishedCB() {
        // default is do no action
    }
};

#endif