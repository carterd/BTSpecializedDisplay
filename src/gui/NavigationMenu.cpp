#include "NavigationMenu.h"
#include "ButtonLabelBar.h"
#include "../themes/display_theme.h"

void NavigationMenu::value_changed_cb(lv_event_t* event) {
	NavigationMenu* navigationMenu = (NavigationMenu*)event->user_data;
	navigationMenu->valueChangedCB(event);
}

void NavigationMenu::exit_btn_cb(lv_event_t* event) {
	((NavigationMenu*) (event->user_data))->exitButtonCB(event);
}

void NavigationMenu::menu_item_btn_cb(lv_event_t* event) {
	if (event->code == LV_EVENT_CLICKED) {
		((NavigationMenu*)(event->user_data))->menuItemButtonCB(event);
	}
	if (event->code == LV_EVENT_FOCUSED) {
		lv_obj_t* btnLabel = lv_obj_get_child(event->target, 1);
		if (btnLabel) lv_label_set_long_mode(btnLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);
	}
	if (event->code == LV_EVENT_DEFOCUSED) {
		lv_obj_t* btnLabel = lv_obj_get_child(event->target, 1);
		if (btnLabel) lv_label_set_long_mode(btnLabel, LV_LABEL_LONG_CLIP);
	}
}

NavigationMenu::NavigationMenu(const char* titleText, const char* exitButtonText, lv_indev_t* indev, ButtonLabelBar* buttonLabel) : BaseLvObject()
{
	this->indev = indev;
    this->titleText = titleText;
    this->exitButtonText = exitButtonText;    
	this->defocusLvObj = NULL;
	this->selectedLvObj = NULL;
    this->focusAnimation = false;
	this->buttonLabelBar = buttonLabel;
}

NavigationMenu::~NavigationMenu()
{
	NavigationMenu::destroyLvObj();
}

lv_obj_t* NavigationMenu::createLvObj(lv_obj_t* parent)
{
	// get the style we'll need for the bar
	display_theme_styles_t* display_theme_styles = (display_theme_styles_t*)lv_disp_get_theme(lv_obj_get_disp(parent))->user_data;
	lv_style_t* menu_label_bar_style = &(display_theme_styles->menu_label_bar);
	lv_style_t* no_scrollbar_style = &(display_theme_styles->no_scrollbar);
	lv_style_t* inv_style = &(display_theme_styles->inv);

	this->group = lv_group_create();
	lv_group_set_wrap(this->group, false);

	// add window to screen object and set it to be the exact size
	this->this_obj = lv_obj_create(parent);
	lv_obj_set_size(this->this_obj, lv_obj_get_width(parent), lv_obj_get_height(parent));
	lv_obj_center(this->this_obj);

	// put in the tile with no scroll bar
	this->option_tileview_obj = lv_tileview_create(this->this_obj);
	lv_obj_add_style(this->option_tileview_obj, no_scrollbar_style, LV_PART_SCROLLBAR);
	lv_obj_add_event_cb(this->option_tileview_obj, NavigationMenu::value_changed_cb, LV_EVENT_VALUE_CHANGED, this);

	// Create the lv_tile to the list of current tiles
	this->menu_tile_obj = lv_tileview_add_tile(this->option_tileview_obj, 0, 0, LV_DIR_ALL);
	this->popup_tile_obj = lv_tileview_add_tile(this->option_tileview_obj, 1, 0, LV_DIR_ALL);
	lv_obj_add_style(this->popup_tile_obj, no_scrollbar_style, LV_PART_SCROLLBAR);
	lv_obj_set_user_data(this->popup_tile_obj, this);
	//lv_obj_set_size(this->menu_obj, lv_obj_get_width(parent), lv_obj_get_height(parent));
	//lv_obj_center(this->menu_obj);

	// add list top label bar
	lv_obj_t* list_label = lv_obj_create(this->menu_tile_obj);
	lv_obj_set_width(list_label, lv_obj_get_width(parent));
	lv_obj_add_style(list_label, menu_label_bar_style, LV_PART_MAIN);
	//lv_obj_add_style(list_label, inv_style, LV_PART_MAIN);
	lv_obj_update_layout(list_label);
	int labelHeight = lv_obj_get_style_height(list_label, LV_PART_MAIN);

	lv_obj_t* scan_label = lv_label_create(list_label);
	lv_label_set_text(scan_label, this->titleText);
	lv_obj_align(scan_label, LV_ALIGN_LEFT_MID, 1, 0);

	// add list
	this->list_obj = lv_list_create(this->menu_tile_obj);
	lv_obj_set_size(this->list_obj, lv_obj_get_width(parent), lv_obj_get_height(parent) - labelHeight - this->buttonLabelBar->getHeight() - 2);
	lv_obj_align(this->list_obj, LV_ALIGN_TOP_MID, 0, labelHeight + 1);	
	
	//Add buttons to the list
	lv_obj_t* btn_label;

	this->exit_button_obj = lv_list_add_btn(this->list_obj, LV_SYMBOL_PREV, this->exitButtonText);
	lv_obj_add_event_cb(this->exit_button_obj, NavigationMenu::exit_btn_cb, LV_EVENT_CLICKED, this);
	btn_label = lv_obj_get_child(this->exit_button_obj, 0);
	if (lv_obj_has_class(btn_label, &lv_label_class)) {
		lv_label_set_long_mode(btn_label, LV_LABEL_LONG_CLIP);
	}
	lv_group_add_obj(this->group, this->exit_button_obj);

    for (std::vector<ScrollMenuItem*>::iterator it = std::begin(this->scrollMenuItems); it != std::end(this->scrollMenuItems); ++it)
    {
        // Create the lv_tile to the list of current tiles
        lv_obj_t* button_obj = this->addMenuItemButton((*it));		
    }

	return this->this_obj;
}

void NavigationMenu::destroyLvObj()
{
    if (this->selectedLvObj) this->selectedLvObj->destroyLvObj();
	if (this->group) lv_group_del(this->group);
	this->group = NULL;
	BaseLvObject::destroyLvObj();
}

void NavigationMenu::focusLvObj(BaseLvObject* defocusLvObj)
{
	lv_indev_set_group(this->indev, this->group);
	if (defocusLvObj)
	{
		this->initMenuItems();
		this->defocusLvObj = defocusLvObj;
	}
	else {
		this->focusAnimation = true;
		lv_obj_set_tile_id(this->option_tileview_obj, 0, 0, LV_ANIM_ON);
	}
	this->updateLvObj();
	showButtonLabels();
}

void NavigationMenu::finishMenuItems() {
	for (std::vector<ScrollMenuItem*>::iterator it = std::begin(this->scrollMenuItems); it != std::end(this->scrollMenuItems); ++it)
	{
		// Create the lv_tile to the list of current tiles
		(*it)->menuItemFinishedCB();
	}
}

void NavigationMenu::initMenuItems() {
	for (std::vector<ScrollMenuItem*>::iterator it = std::begin(this->scrollMenuItems); it != std::end(this->scrollMenuItems); ++it)
	{
		// Create the lv_tile to the list of current tiles
		(*it)->menuItemInitCB();
	}
}

void NavigationMenu::updateLvObj() {
    for (std::vector<ScrollMenuItem*>::iterator it = std::begin(this->scrollMenuItems); it != std::end(this->scrollMenuItems); ++it)
    {
        // Create the lv_tile to the list of current tiles
        this->updateMenuItemButton(*it);
    }
}


void NavigationMenu::setButtonLabel(ButtonLabelBar* buttonLabel)
{
	this->buttonLabelBar = buttonLabel;
}

void NavigationMenu::showButtonLabels()
{
	if (this->buttonLabelBar) {
		this->buttonLabelBar->setButtonLabels(LV_SYMBOL_UP, LV_SYMBOL_OK, LV_SYMBOL_DOWN);
		this->buttonLabelBar->show();
	}
}


lv_obj_t* NavigationMenu::addMenuItemButton(ScrollMenuItem* menuItem) {
	lv_obj_t* list_btn = lv_list_add_btn(this->list_obj, LV_SYMBOL_OK, menuItem->getItemText());
    lv_obj_set_user_data(list_btn, menuItem);

	lv_obj_t* btn_label = lv_obj_get_child(list_btn, 1);
	if (lv_obj_has_class(btn_label, &lv_label_class)) {
		lv_label_set_long_mode(btn_label, LV_LABEL_LONG_CLIP);
	}
	lv_obj_add_event_cb(list_btn, NavigationMenu::menu_item_btn_cb, LV_EVENT_CLICKED, this);
	lv_obj_add_event_cb(list_btn, NavigationMenu::menu_item_btn_cb, LV_EVENT_FOCUSED, this);
	lv_obj_add_event_cb(list_btn, NavigationMenu::menu_item_btn_cb, LV_EVENT_DEFOCUSED, this);
	lv_group_add_obj(this->group, list_btn);

	this->updateMenuItemButton(menuItem);

	return list_btn;
}

lv_obj_t* NavigationMenu::getMenuItemButton(ScrollMenuItem* menuItem) {
    for (int i = 0; i < lv_obj_get_child_cnt(this->list_obj); i++) {
        lv_obj_t* menuItemButton = lv_obj_get_child(this->list_obj, i);
        if (lv_obj_get_user_data(menuItemButton) == menuItem) {
            return menuItemButton;
        }
    }
    return NULL;
}

void NavigationMenu::updateMenuItemButton(ScrollMenuItem* menuItem) {
    lv_obj_t* menuItemButton = this->getMenuItemButton(menuItem);
    if (menuItemButton) {
       	lv_obj_t* btn_img = lv_obj_get_child(menuItemButton, 0);
	    if (lv_obj_has_class(btn_img, &lv_img_class) && !menuItem->isChecked()) {
            if (!lv_obj_has_flag(btn_img, LV_OBJ_FLAG_HIDDEN))
		        lv_obj_add_flag(btn_img, LV_OBJ_FLAG_HIDDEN);
	    } else {
            if (lv_obj_has_flag(btn_img, LV_OBJ_FLAG_HIDDEN))
                lv_obj_clear_flag(btn_img, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

void NavigationMenu::addMenuItem(ScrollMenuItem* scrollMenuItem)
{
    // Append the scrollMenuItem to the list of menu items for use when item is selected
    this->scrollMenuItems.push_back(scrollMenuItem);
}

void NavigationMenu::exitButtonCB(lv_event_t* event)
{	
	if (this->defocusLvObj) {
		// Ensure we store any state associate with navigation items
		this->finishMenuItems();
		this->defocusLvObj->focusLvObj();
	}
}

void NavigationMenu::menuItemButtonCB(lv_event_t* event) {
	// Disable menuItem clicking until the selection has been removed
	if (this->focusAnimation == true) return;

	ScrollMenuItem* scrollMenuItem = (ScrollMenuItem * ) lv_obj_get_user_data(event->target);

	lv_obj_clean(this->popup_tile_obj);

	// Create the popup item if there is one on the scrollMenuItem
	BaseLvObject* popupItem = scrollMenuItem->getPopupItem();
	if (popupItem) {
		this->selectedLvObj = popupItem;
		popupItem->createLvObj(this->popup_tile_obj);
		popupItem->focusLvObj(this);		
		lv_obj_set_tile_id(this->option_tileview_obj, 1, 0, LV_ANIM_ON);
	}
	else {
		scrollMenuItem->noPopupAction();
	}
	
	this->updateLvObj();
}

void NavigationMenu::valueChangedCB(lv_event_t* event) {
	if (this->focusAnimation) {
		// Ensure that on of the activit buttons are active
		lv_indev_set_group(this->indev, this->group);
		// Animation to set focus is over
		this->focusAnimation = false;
		this->selectedLvObj->destroyLvObj();
		this->selectedLvObj = NULL;
	}
	this->updateLvObj();
}