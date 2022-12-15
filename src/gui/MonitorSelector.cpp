#include <LvglThemes/lv_theme_binary.h>

#include <algorithm>

#include "MonitorSelector.h"
#include "BluetoothConnection.h"

#include "../img/PressButton.h"

void MonitorSelector::tile_btn_cb(lv_event_t* event) {
    MonitorSelector* monitorSelector = (MonitorSelector*)event->user_data;
    monitorSelector->tileButtonCB(event);
}

void MonitorSelector::tile_btn_defocus_cb(lv_event_t* event) {
    MonitorSelector* monitorSelector = (MonitorSelector*)event->user_data;
    monitorSelector->tileButtonDefocusCB(event);
}

void MonitorSelector::tile_btn_focus_cb(lv_event_t* event) {
    MonitorSelector* monitorSelector = (MonitorSelector*)event->user_data;
    monitorSelector->tileButtonFocusCB(event);
}

void MonitorSelector::exit_btn_cb(lv_event_t* event)
{
    MonitorSelector* monitorSelector = (MonitorSelector*)event->user_data;
    monitorSelector->exitButtonCB(event);
}

void MonitorSelector::value_changed_cb(lv_event_t* event) {
    MonitorSelector* monitorSelector = (MonitorSelector*)event->user_data;
    monitorSelector->valueChangedCB(event);
}

MonitorSelector::MonitorSelector(lv_indev_t* indev, ButtonLabelBar* buttonLabelBar) : ButtonLabelledLvObject(indev, buttonLabelBar)
{
    // Initialise rest as null
    this->options_tileview_obj = NULL;
    this->selectedItem = 0;
    this->selectedItemAnimated = false;
}

MonitorSelector::~MonitorSelector()
{
    // Frist ensure all ScrollMenuItems are removed
    for (std::vector<MonitorLvObject*>::iterator it = std::begin(this->monitorLvObjects); it != std::end(this->monitorLvObjects); ++it) {
        delete(*it);
    }
    MonitorSelector::destroyLvObj();
}

lv_obj_t* MonitorSelector::createLvObj(lv_obj_t* parent)
{
    // get the style we'll need for the bar
    theme_binary_styles_t* binary_styles = (theme_binary_styles_t*)lv_disp_get_theme(lv_obj_get_disp(parent))->user_data;
    lv_style_t* no_scrollbar = &(binary_styles->no_scrollbar);
    lv_style_t* button_no_highlight_style = &(binary_styles->button_no_highlight);

    // add window to screen object and set it to be the exact size
    this->this_obj = lv_obj_create(parent);
    lv_obj_update_layout(parent);
    lv_obj_set_size(this->this_obj, lv_obj_get_width(parent), lv_obj_get_height(parent));
    lv_obj_center(this->this_obj);
    
    // add two activity button object to identify if encoder is being used
    //this->activity_btn_obj = lv_btn_create(this->this_obj);
    //lv_obj_add_style(this->activity_btn_obj, button_no_highlight_style, LV_PART_MAIN);    
    //lv_obj_add_event_cb(this->activity_btn_obj, MonitorSelector::exit_btn_cb, LV_EVENT_LONG_PRESSED, this);
    //lv_group_add_obj(this->group, this->activity_btn_obj);
    
    // put in the tile with no scroll bar
    this->options_tileview_obj = lv_tileview_create(this->this_obj);
    lv_obj_add_style(this->options_tileview_obj, no_scrollbar, LV_PART_SCROLLBAR);
    lv_obj_add_event_cb(this->options_tileview_obj, MonitorSelector::value_changed_cb, LV_EVENT_VALUE_CHANGED, this);

    // create a group for the menu items
    this->group = lv_group_create();
    lv_group_set_wrap(this->group, false);

    int tile_pos = 0;
    for (std::vector<MonitorLvObject*>::iterator it = std::begin(this->monitorLvObjects); it != std::end(this->monitorLvObjects); ++it)
    {
        // Create the lv_tile to the list of current tiles
        lv_obj_t* tile_obj = lv_tileview_add_tile(this->options_tileview_obj, tile_pos, 0, LV_DIR_ALL);
        lv_obj_set_user_data(tile_obj, (*it));
        lv_obj_t* btn_obj = lv_btn_create(tile_obj);
        lv_obj_add_style(btn_obj, button_no_highlight_style, LV_PART_MAIN);
        lv_obj_set_size(btn_obj, lv_obj_get_width(parent), lv_obj_get_height(parent));

        // Over the button which is in the background create the actual monitor contents
        (*it)->createLvObj(tile_obj);

        lv_obj_add_event_cb(btn_obj, MonitorSelector::exit_btn_cb, LV_EVENT_LONG_PRESSED, this);
        lv_obj_add_event_cb(btn_obj, MonitorSelector::tile_btn_cb, LV_EVENT_SHORT_CLICKED, this);
        lv_obj_add_event_cb(btn_obj, MonitorSelector::tile_btn_defocus_cb, LV_EVENT_DEFOCUSED, this);
        lv_obj_add_event_cb(btn_obj, MonitorSelector::tile_btn_focus_cb, LV_EVENT_FOCUSED, this);
        lv_group_add_obj(this->group, btn_obj);

        tile_pos++;
    }

    return this->this_obj;
}


void MonitorSelector::destroyLvObj()
{
    if (this->group) lv_group_del(this->group);
    this->group = NULL;
    BaseLvObject::destroyLvObj();
    this->options_tileview_obj = NULL;
}

void MonitorSelector::focusLvObj(BaseLvObject* defocusLvObj)
{
    this->updateButtonLabelBar();
    this->buttonLabelBar->setAutoHide(true);
    this->initBluetoothStats();
    // Ensures the monitor is selected
    this->selectMonitor();
    
    // Ensure there is some children in the tileview
    if (lv_obj_get_child_cnt(this->options_tileview_obj)) {
        // Attempt to set the correct enables entity
        lv_obj_set_tile_id(this->options_tileview_obj, this->selectedItem, 0, LV_ANIM_OFF);
        lv_obj_t* active_obj = lv_tileview_get_tile_act(this->options_tileview_obj);
        lv_obj_t* btn_obj = lv_obj_get_child(active_obj, 0);
        lv_indev_set_group(this->indev, this->group);
        lv_group_focus_obj(btn_obj);
    }
}

void MonitorSelector::addMonitorLvObject(MonitorLvObject* monitorLvObject)
{
    this->monitorLvObjects.push_back(monitorLvObject);
}

void MonitorSelector::updateButtonLabelBar()
{
    if (this->buttonLabelBar) {
        this->buttonLabelBar->setButtonLabels(LV_SYMBOL_LEFT, LV_SYMBOL_CLOSE, LV_SYMBOL_RIGHT);
    }
}

void MonitorSelector::initBluetoothStats() {
    // Frist ensure all ScrollMenuItems are removed
    for (std::vector<MonitorLvObject*>::iterator it = std::begin(this->monitorLvObjects); it != std::end(this->monitorLvObjects); ++it) {
        (*it)->initBluetoothStats();
    }
}

void MonitorSelector::setBluetoothController(BluetoothBikeController* bluetoothBikeController)
{
    // Frist ensure all ScrollMenuItems are removed
    for (std::vector<MonitorLvObject*>::iterator it = std::begin(this->monitorLvObjects); it != std::end(this->monitorLvObjects); ++it) {
        (*it)->setBluetoothController(bluetoothBikeController);        
    }
}

void MonitorSelector::statusUpdate() {
    // Frist ensure all ScrollMenuItems are removed    
    for (std::vector<MonitorLvObject*>::iterator it = std::begin(this->monitorLvObjects); it != std::end(this->monitorLvObjects); ++it) {
        (*it)->statusUpdate();
    }
}

void MonitorSelector::deselectMonitor() {
    // Get the current tile that user has navigated to
    lv_obj_t* selected_obj = lv_obj_get_child(this->options_tileview_obj, this->selectedItem);
    // Identify the monitor object specifically
    MonitorLvObject* monitorLvObject = (MonitorLvObject*)lv_obj_get_user_data(selected_obj);
    this->deselectMonitor(monitorLvObject);
}

void MonitorSelector::deselectMonitor(MonitorLvObject* monitorLvObject) {
    std::vector<MonitorLvObject*>::iterator it = std::find(std::begin(this->monitorLvObjects), std::end(this->monitorLvObjects), monitorLvObject);

    if (it != std::end(this->monitorLvObjects)) monitorLvObject->defocusLvObj();
}

void MonitorSelector::selectMonitor() {
    // Get the current tile that user has navigated to
    lv_obj_t* selected_obj = lv_obj_get_child(this->options_tileview_obj, this->selectedItem);
    // Identify the monitor object specifically
    MonitorLvObject* monitorLvObject = (MonitorLvObject*)lv_obj_get_user_data(selected_obj);
    this->selectMonitor(monitorLvObject);
}

void MonitorSelector::selectMonitor(MonitorLvObject* monitorLvObject) {
    std::vector<MonitorLvObject*>::iterator it = std::find(std::begin(this->monitorLvObjects), std::end(this->monitorLvObjects), monitorLvObject);

    if (it != std::end(this->monitorLvObjects)) {
        int tilePos = it - std::begin(this->monitorLvObjects);
        this->deselectMonitor();
        this->selectedItem = tilePos;
        monitorLvObject->focusLvObj(this);
    }
}

void MonitorSelector::tileButtonCB(lv_event_t* e) {
    this->buttonLabelBar->show();

    // Don't allow clicks while selection is being animated
    if (this->selectAnimation) return;

    // Get the current tile that user has navigated to
    lv_obj_t* selected_obj = lv_tileview_get_tile_act(this->options_tileview_obj);
    // Identify the monitor object specifically
    MonitorLvObject* monitorLvObject = (MonitorLvObject *) lv_obj_get_user_data(selected_obj);

    if (monitorLvObject) {
        this->selectMonitor(monitorLvObject);
    }
}

void MonitorSelector::tileButtonDefocusCB(lv_event_t* e) {
    // For the moment lets disable showing the menu bar while in connected mode
}

void MonitorSelector::tileButtonFocusCB(lv_event_t* e) {

    // For the moment lets disable showing the menu bar while in connected mode
    lv_obj_t* tile_obj = lv_obj_get_parent(lv_group_get_focused(this->group));
    if (lv_obj_check_type(tile_obj, &lv_tileview_tile_class) && lv_obj_get_user_data(tile_obj)) {
        // Set the tile as being focused instantly ... no scroll        
        lv_obj_set_tile(this->options_tileview_obj, tile_obj, LV_ANIM_OFF);
        // Identify the tile number
        MonitorLvObject* monitorLvObject = (MonitorLvObject*)lv_obj_get_user_data(tile_obj);
        this->selectMonitor(monitorLvObject);
    }
}

void MonitorSelector::valueChangedCB(lv_event_t* event) {
    this->selectAnimation = false;
}

void MonitorSelector::exitButtonCB(lv_event_t* event)
{
    this->bluetoothConnection->exitButtonCB();
}