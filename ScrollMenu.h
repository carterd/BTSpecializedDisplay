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
    ScrollMenu(lv_indev_t* indev);
    virtual lv_obj_t* createLvObj(lv_obj_t* parent);
    virtual void focusLvObj();
    void setButtonLabel(ButtonLabel* buttonLabel, bool startShown = true);
    void addMenuItem(ScrollMenuItem* scrollMenuItem);
    void tileButtonCB(lv_event_t* event);
    void tileButtonDefocusCB(lv_event_t* event);
    void valueChangedCB(lv_event_t* event);
};

#endif