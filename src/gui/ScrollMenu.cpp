#include <algorithm>

#include "ScrollMenu.h"
#include "../themes/display_theme.h"

void ScrollMenu::value_changed_cb(lv_event_t* event) {
    ScrollMenu* scrollMenu = (ScrollMenu*) event->user_data;
    scrollMenu->valueChangedCB(event);
}

void ScrollMenu::tile_btn_cb(lv_event_t* event) {
    ScrollMenu* scrollMenu = (ScrollMenu*) event->user_data;
    scrollMenu->tileButtonCB(event);
}

void ScrollMenu::tile_btn_defocus_cb(lv_event_t* event) {
    ScrollMenu* scrollMenu = (ScrollMenu*)event->user_data;
    scrollMenu->tileButtonDefocusCB(event);
}

ScrollMenu::ScrollMenu(lv_indev_t* indev, ButtonLabelBar* buttonLabelBar) : ButtonLabelledLvObject(indev, buttonLabelBar)
{
    // Initialise rest as null
    this->options_tileview_obj = NULL;
    this->selectedItem = 0;
    this->selectedItemAnimated = false;
}

ScrollMenu::~ScrollMenu()
{
    // Frist ensure all ScrollMenuItems are removed
    for (std::vector<ScrollMenuItem*>::iterator it = std::begin(this->scrollMenuItems); it != std::end(this->scrollMenuItems); ++it) {
        delete(*it);
    }
    ScrollMenu::destroyLvObj();
}

lv_obj_t* ScrollMenu::createLvObj(lv_obj_t* parent)
{
    // get the style we'll need for the bar
    display_theme_styles_t* binary_styles = (display_theme_styles_t*)lv_disp_get_theme(lv_obj_get_disp(parent))->user_data;
    lv_style_t* no_scrollbar = &(binary_styles->no_scrollbar);
    //lv_style_t* button_no_highlight_style = &(binary_styles->button_no_highlight);

    // add window to screen object and set it to be the exact size
    this->this_obj = lv_obj_create(parent);
    lv_obj_set_size(this->this_obj, lv_obj_get_width(parent), lv_obj_get_height(parent));
    lv_obj_center(this->this_obj);

    // put in the tile with no scroll bar
    this->options_tileview_obj = lv_tileview_create(this->this_obj);
    lv_obj_add_style(this->options_tileview_obj, no_scrollbar, LV_PART_SCROLLBAR);
    lv_obj_add_event_cb(this->options_tileview_obj, ScrollMenu::value_changed_cb, LV_EVENT_VALUE_CHANGED, this);

    // create a group for the menu items
    this->group = lv_group_create();
    lv_group_set_wrap(this->group, false);

    int tile_pos = 0;
    for (std::vector<ScrollMenuItem*>::iterator it = std::begin(this->scrollMenuItems); it != std::end(this->scrollMenuItems); ++it)
    {
        // Create the lv_tile to the list of current tiles
        lv_obj_t* tile_obj = lv_tileview_add_tile(this->options_tileview_obj, tile_pos, 0, LV_DIR_ALL);
        lv_obj_add_style(tile_obj, no_scrollbar, LV_PART_SCROLLBAR);
        lv_obj_t* btn_obj = (*it)->createLvObj(tile_obj);
        lv_obj_add_event_cb(btn_obj, ScrollMenu::tile_btn_cb, LV_EVENT_SHORT_CLICKED, this);
        lv_obj_add_event_cb(btn_obj, ScrollMenu::tile_btn_defocus_cb, LV_EVENT_DEFOCUSED, this);
        lv_group_add_obj(this->group, btn_obj);

        // Create the popup item if there is one on the scrollMenuItem
        BaseLvObject* popupItem = (*it)->getPopupItem();
        if (popupItem) {
            lv_obj_t* popup_tile = lv_tileview_add_tile(this->options_tileview_obj, tile_pos, 1, LV_DIR_ALL);
            lv_obj_add_style(popup_tile, no_scrollbar, LV_PART_SCROLLBAR);
            lv_obj_set_user_data(popup_tile, this);
            popupItem->createLvObj(popup_tile);
        }        
        tile_pos++;
    }

    return this->this_obj;
}

void ScrollMenu::destroyLvObj()
{
    if (this->group) lv_group_del(this->group);
    this->group = NULL;
    BaseLvObject::destroyLvObj();
    this->options_tileview_obj = NULL;
}

void ScrollMenu::focusLvObj(BaseLvObject* defocusLvObj)
{
    this->updateButtonLabelBar();

    // TODO: Sort this out a bit wonky
    if (lv_obj_get_child_cnt(this->options_tileview_obj)) {
        if (this->selectedItemAnimated) {
            static lv_group_t* empty_group = lv_group_create();
            this->focusAnimation = true;

            lv_obj_set_tile_id(this->options_tileview_obj, this->selectedItem, 0, LV_ANIM_ON);
            lv_indev_set_group(this->indev, empty_group);
        }
        else
        {
            //static lv_group_t* empty_group = lv_group_create();
            //this->focusAnimation = true;

            //lv_obj_set_tile_id(this->options_tileview_obj, this->selectedItem, 0, LV_ANIM_ON);
            //lv_indev_set_group(this->indev, empty_group);
            
            // Attempt to set the correct enables entity
            lv_obj_set_tile_id(this->options_tileview_obj, this->selectedItem, 0, LV_ANIM_OFF);            
            lv_obj_t* active_obj = lv_tileview_get_tile_act(this->options_tileview_obj);
            lv_obj_t* btn_obj = lv_obj_get_child(active_obj, 0);
            lv_indev_set_group(this->indev, this->group);
            lv_group_focus_obj(btn_obj);            
        }
    }
}

void ScrollMenu::updateButtonLabelBar()
{
    if (this->buttonLabelBar) {
        this->buttonLabelBar->setButtonLabels(LV_SYMBOL_LEFT, LV_SYMBOL_OK, LV_SYMBOL_RIGHT);
        this->buttonLabelBar->show();
        this->buttonLabelBar->setAutoHide(false);
    }
}


void ScrollMenu::addMenuItem(ScrollMenuItem* scrollMenuItem)
{
    // Append the scrollMenuItem to the list of menu items for use when item is selected
    this->scrollMenuItems.push_back(scrollMenuItem);
}

void ScrollMenu::selectScrollMenuItem(ScrollMenuItem* scrollMenuItem) {

    // Get the current tile that user has navigated to
    lv_obj_t* obj = lv_tileview_get_tile_act(this->options_tileview_obj);

    std::vector<ScrollMenuItem*>::iterator it = std::find(std::begin(this->scrollMenuItems), std::end(this->scrollMenuItems), scrollMenuItem);

    if (it != std::end(this->scrollMenuItems)) {
        int tilePos = it - std::begin(this->scrollMenuItems);
        this->selectedItem = tilePos;
        this->selectedItemAnimated = scrollMenuItem->getAnimatedSelect();
        lv_obj_set_tile_id(this->options_tileview_obj, tilePos, 1, this->selectedItemAnimated ? LV_ANIM_ON : LV_ANIM_OFF);
        scrollMenuItem->focusLvObj(this);
    }
}

void ScrollMenu::tileButtonCB(lv_event_t* e) {
    // Don't allow clicks while selection is being animated between options
    if (this->selectAnimation) return;

    // Get the current tile that user has navigated to
    lv_obj_t* selected_obj = lv_tileview_get_tile_act(this->options_tileview_obj);

    for (std::vector<ScrollMenuItem*>::iterator it = std::begin(this->scrollMenuItems); it != std::end(this->scrollMenuItems); ++it)
    {
        ScrollMenuItem* scrollMenuItem = *it;
        lv_obj_t* obj_cmp = lv_obj_get_parent(scrollMenuItem->getLvObj());

        // If the scroll menu item's parent object, i.e. the tile holding the item equals the selected_object then this is the scrollMenuItem to select
        if (obj_cmp == selected_obj) {
            this->selectScrollMenuItem(scrollMenuItem);
            return;
        }
    }
}

void ScrollMenu::tileButtonDefocusCB(lv_event_t* e) {
    this->selectAnimation = true;
}

void ScrollMenu::valueChangedCB(lv_event_t* event) {
    this->selectAnimation = false;
    if (this->focusAnimation) {
        // Ensure that on of the activit buttons are active
        lv_indev_set_group(this->indev, this->group);
        // Animation to set focus is over
        this->focusAnimation = false;
    }
}

