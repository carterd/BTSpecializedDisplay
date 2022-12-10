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
    /// This is the constructor of the menu item which is an image selection
    /// </summary>
    /// <param name="image">The menu items image</param>
    /// <param name="animatedSelect">Boolean specifies if animation is required to select the item</param>
    ScrollMenuItem(lv_img_dsc_t* image, bool animatedSelect = true);

    /// <summary>
    /// This is the constructor of the menu item which is a text selection
    /// </summary>
    /// <param name="itemText"></param>
    /// <param name="animatedSelect"></param>
    ScrollMenuItem(const char* itemText, bool animatedSelect = true);

    virtual ~ScrollMenuItem();

    /// <summary>
    /// Create the lv_obj for the scroll menu item
    /// </summary>
    /// <param name="parent"></param>
    /// <returns></returns>
    virtual lv_obj_t* createLvObj(lv_obj_t* parent);

    /// <summary>
    /// Focus has been selected for this menu item
    /// </summary>
    /// <param name="defocusLvObj"></param>
    virtual void focusLvObj(BaseLvObject* defocusLvObj = NULL);
    lv_img_dsc_t* getLvImage();
    const char* getItemText() {
        return this->itemText;
    }

    /// <summary>
    /// This is the object which is required to 'popup' when the menu item is selected.
    /// </summary>
    /// <param name="popupItem">The item to set as the popup item</param>
    void setPopupItem(BaseLvObject* popupItem);

    /// <summary>
    /// Return the BaseLvObject that should be displayed on selecting the menu item.
    /// </summary>
    /// <returns></returns>
    BaseLvObject* getPopupItem();

    /// <summary>
    /// Return true if the popup item should be scrolled to or simply appear in an instance.
    /// </summary>
    /// <returns></returns>
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