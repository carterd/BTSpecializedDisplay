#include "ValueSelectMenu.h"
#include "ButtonLabelBar.h"
#include "../themes/display_theme.h"

void ValueSelectMenu::exit_btn_cb(lv_event_t* event) {
	((ValueSelectMenu*) (event->user_data))->exitButtonCB(event);
}

void ValueSelectMenu::menu_item_btn_cb(lv_event_t* event) {
	if (event->code == LV_EVENT_CLICKED) {
		((ValueSelectMenu*)(event->user_data))->valueItemButtonCB(event);
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

ValueSelectMenu::ValueSelectMenu(const char* titleText, const char* exitButtonText, lv_indev_t* indev, ButtonLabelBar* buttonLabelBar) : ButtonLabelledLvObject(indev, buttonLabelBar)
{
    this->titleText = titleText;
    this->exitButtonText = exitButtonText;
	this->defocusLvObj = NULL;
}

ValueSelectMenu::~ValueSelectMenu()
{
	ValueSelectMenu::destroyLvObj();
	this->deleteAllMenuItems();
}

void ValueSelectMenu::destroyLvObj()
{
	if (this->group) lv_group_del(this->group);
	this->group = NULL;
	BaseLvObject::destroyLvObj();
}

lv_obj_t* ValueSelectMenu::createLvObj(lv_obj_t* parent)
{
	// get the style we'll need for the bar
	display_theme_styles_t* display_theme_styles = (display_theme_styles_t*)lv_disp_get_theme(lv_obj_get_disp(parent))->user_data;
	lv_style_t* menu_label_bar_style = &(display_theme_styles->menu_label_bar);
	lv_style_t* inv_style = &(display_theme_styles->inv);

	this->group = lv_group_create();
	lv_group_set_wrap(this->group, false);

	// add window to screen object and set it to be the exact size
	this->this_obj = lv_obj_create(parent);
	lv_obj_set_size(this->this_obj, lv_obj_get_width(parent), lv_obj_get_height(parent));
	lv_obj_center(this->this_obj);

	// add list top label bar
	lv_obj_t* list_label = lv_obj_create(this->this_obj);
	lv_obj_set_width(list_label, lv_obj_get_width(parent));
	lv_obj_add_style(list_label, menu_label_bar_style, LV_PART_MAIN);
	//lv_obj_add_style(list_label, inv_style, LV_PART_MAIN);
	lv_obj_update_layout(list_label);
	int labelHeight = lv_obj_get_style_height(list_label, LV_PART_MAIN);

	lv_obj_t* scan_label = lv_label_create(list_label);
	lv_label_set_text(scan_label, this->titleText);
	lv_obj_align(scan_label, LV_ALIGN_LEFT_MID, 1, 0);

	// add list
	this->list_obj = lv_list_create(this->this_obj);
	lv_obj_set_size(this->list_obj, lv_obj_get_width(parent), lv_obj_get_height(parent) - labelHeight - this->buttonLabelBar->getHeight() - 2);
	lv_obj_align(this->list_obj, LV_ALIGN_TOP_MID, 0, labelHeight + 1);
	
	//Add buttons to the list
	lv_obj_t* btn_label;

	this->exit_button_obj = lv_list_add_btn(this->list_obj, LV_SYMBOL_PREV, this->exitButtonText);
	lv_obj_add_event_cb(this->exit_button_obj, ValueSelectMenu::exit_btn_cb, LV_EVENT_CLICKED, this);
	btn_label = lv_obj_get_child(this->exit_button_obj, 0);
	if (lv_obj_has_class(btn_label, &lv_label_class)) {
		lv_label_set_long_mode(btn_label, LV_LABEL_LONG_CLIP);
	}
	lv_group_add_obj(this->group, this->exit_button_obj);

    for (std::vector<ValueSelectMenuItem*>::iterator it = std::begin(this->valueMenuItems); it != std::end(this->valueMenuItems); ++it)
    {
        // Create the lv_tile to the list of current tiles
        lv_obj_t* button_obj = this->addMenuItemButton((*it));
    }

	return this->this_obj;
}

void ValueSelectMenu::focusLvObj(BaseLvObject* defocusLvObj)
{	
	this->updateLvObj();
	lv_indev_set_group(this->indev, this->group);
	if (defocusLvObj)
	{
		this->defocusLvObj = defocusLvObj;		
	}
	this->updateButtonLabelBar();
}

void ValueSelectMenu::updateLvObj() {
    for (std::vector<ValueSelectMenuItem*>::iterator it = std::begin(this->valueMenuItems); it != std::end(this->valueMenuItems); ++it)
    {
        // Create the lv_tile to the list of current tiles
        this->updateMenuItemButton(*it);
    }

}

void ValueSelectMenu::updateButtonLabelBar()
{
	if (this->buttonLabelBar) {
		this->buttonLabelBar->setButtonLabels(LV_SYMBOL_UP, LV_SYMBOL_OK, LV_SYMBOL_DOWN);
		this->buttonLabelBar->show();
	}
}

lv_obj_t* ValueSelectMenu::addMenuItemButton(ValueSelectMenuItem* menuItem) {
	lv_obj_t* list_btn = lv_list_add_btn(this->list_obj, LV_SYMBOL_OK, menuItem->getItemText());
	lv_obj_set_user_data(list_btn, menuItem);

	lv_obj_t* btn_label = lv_obj_get_child(list_btn, 1);
	if (lv_obj_has_class(btn_label, &lv_label_class)) {
		lv_label_set_long_mode(btn_label, LV_LABEL_LONG_CLIP);
	}
	lv_obj_add_event_cb(list_btn, ValueSelectMenu::menu_item_btn_cb, LV_EVENT_CLICKED, this);
	lv_obj_add_event_cb(list_btn, ValueSelectMenu::menu_item_btn_cb, LV_EVENT_FOCUSED, this);
	lv_obj_add_event_cb(list_btn, ValueSelectMenu::menu_item_btn_cb, LV_EVENT_DEFOCUSED, this);
	lv_group_add_obj(this->group, list_btn);

	this->updateMenuItemButton(menuItem);

	return list_btn;
}

lv_obj_t* ValueSelectMenu::getMenuItemButton(ValueSelectMenuItem* menuItem) {
    for (int i = 0; i < lv_obj_get_child_cnt(this->list_obj); i++) {
        lv_obj_t* menuItemButton = lv_obj_get_child(this->list_obj, i);
        if (lv_obj_get_user_data(menuItemButton) == menuItem) {
            return menuItemButton;
        }
    }
    return NULL;
}

void ValueSelectMenu::updateMenuItemButton(ValueSelectMenuItem* menuItem) {
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

void ValueSelectMenu::addMenuItem(ValueSelectMenuItem* menuItem)
{
    // Append the scrollMenuItem to the list of menu items for use when item is selected
    this->valueMenuItems.push_back(menuItem);
}

void ValueSelectMenu::exitButtonCB(lv_event_t* event)
{	
	if (this->defocusLvObj) {
		this->defocusLvObj->focusLvObj();
	}
}

void ValueSelectMenu::valueItemButtonCB(lv_event_t* event) {
	ValueSelectMenuItem* menuItem = (ValueSelectMenuItem * ) lv_obj_get_user_data(event->target);
    menuItem->selectedCB();
    // Refresh the visability on isChecked items;
    this->updateLvObj();
}