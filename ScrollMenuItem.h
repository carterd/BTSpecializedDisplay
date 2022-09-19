#ifndef SCROLL_MENU_ITEM_H
#define SCROLL_MENU_ITEM_H

#include <lvgl.h>

#include "BaseLvObject.h"

class ScrollMenuItem : public BaseLvObject
{
private:
    lv_img_dsc_t* image;

    BaseLvObject* popupItem;
    bool animatedSelect;

public:
    ScrollMenuItem(lv_img_dsc_t* image, bool animatedSelect = true);
    virtual lv_obj_t* createLvObj(lv_obj_t* parent);
    virtual void focusLvObj();    
    lv_img_dsc_t* getLvImage();
    void setPopupItem(BaseLvObject* popupItem);
    BaseLvObject* getPopupItem();
    bool getAnimatedSelect();
};

#endif