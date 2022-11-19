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
};

#endif