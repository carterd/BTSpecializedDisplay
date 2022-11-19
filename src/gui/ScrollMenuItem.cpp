#include <LvglThemes/lv_theme_binary.h>

#include "ScrollMenuItem.h"


ScrollMenuItem::ScrollMenuItem(lv_img_dsc_t* image, bool animatedSelect) : BaseLvObject()
{
    this->image = image;
    this->itemText = NULL;
    this->popupItem = NULL;
    this->animatedSelect = animatedSelect;
}

ScrollMenuItem::ScrollMenuItem(const char* itemText, bool animatedSelect) : BaseLvObject()
{
    this->itemText = itemText;
    this->image = NULL;
    this->popupItem = NULL;
    this->animatedSelect = animatedSelect;
}

ScrollMenuItem::~ScrollMenuItem()
{
    this->popupItem->destroyLvObj();
    BaseLvObject::destroyLvObj();
    delete (this->popupItem);
}

lv_obj_t* ScrollMenuItem::createLvObj(lv_obj_t* parent)
{
    if (this->image) {
        // get the style we'll need for the bar
        theme_binary_styles_t* binary_styles = (theme_binary_styles_t*)lv_disp_get_theme(lv_obj_get_disp(parent))->user_data;
        lv_style_t* button_no_highlight_style = &(binary_styles->button_no_highlight);

        // Create a button
        this->this_obj = lv_btn_create(parent);
        lv_obj_center(this->this_obj);
        lv_obj_add_style(this->this_obj, button_no_highlight_style, LV_PART_MAIN);

        // Associate an image
        lv_obj_t* img_obj = lv_img_create(this->this_obj);
        lv_img_set_src(img_obj, this->image);
        lv_obj_center(img_obj);
    }
    // Return the button just in case
    return this->this_obj;
}

void ScrollMenuItem::focusLvObj(BaseLvObject* defocusLvObj)
{
    if (this->popupItem) {
        this->popupItem->focusLvObj(defocusLvObj);
    }
}

lv_img_dsc_t* ScrollMenuItem::getLvImage()
{
    return this->image;
}

void ScrollMenuItem::setPopupItem(BaseLvObject* popupItem)
{
    this->popupItem = popupItem;
}

BaseLvObject* ScrollMenuItem::getPopupItem()
{
    return this->popupItem;
}

bool ScrollMenuItem::getAnimatedSelect() {
    return this->animatedSelect;
}