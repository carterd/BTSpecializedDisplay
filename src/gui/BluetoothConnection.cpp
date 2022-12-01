#include "BluetoothConnection.h"
#include <LvglThemes/lv_theme_binary.h>

void BluetoothConnection::tile_changed_cb(lv_event_t* event) {
    ((BluetoothConnection*)(event->user_data))->tileChangedhCB(event);
}

void BluetoothConnection::refresh_cb(lv_event_t* event) {
	((BluetoothConnection*)(event->user_data))->refreshCB(event);
}

void BluetoothConnection::exit_btn_cb(lv_event_t* event) {
    ((BluetoothConnection*)(event->user_data))->exitButtonCB(event);
}

void BluetoothConnection::connecting_timer_cb(lv_timer_t * timer) {
    ((BluetoothConnection*)(timer->user_data))->connectingTimerCB(timer);
}

BluetoothConnection::BluetoothConnection(BluetoothBikeController* bluetoothController, ConfigStore* configStore, lv_img_dsc_t* image, lv_indev_t* indev) {
	this->bluetoothController = bluetoothController;
	this->configStore = configStore;
	this->image = image;
    this->indev = indev;
    this->group = NULL;
    this->connecting = false;
    this->connected = false;
    this->monitorLvObjActive = false;
    this->monitorLvObj = NULL;
    this->exiting = false;
}

BluetoothConnection::~BluetoothConnection()
{
    this->monitorLvObj->destroyLvObj();
    //delete(this->monitorLvObj);
    BluetoothConnection::destroyLvObj();
}

lv_obj_t* BluetoothConnection::createLvObj(lv_obj_t* parent)
{
    // get the style we'll need for the bar
    theme_binary_styles_t* binary_styles = (theme_binary_styles_t*)lv_disp_get_theme(lv_obj_get_disp(parent))->user_data;
    lv_style_t* no_scrollbar = &(binary_styles->no_scrollbar);
    lv_style_t* button_no_highlight_style = &(binary_styles->button_no_highlight);
    lv_style_t* button_style = &(binary_styles->button);
 
    lv_obj_update_layout(parent);    
    this->this_obj = lv_obj_create(parent);
    lv_obj_add_style(this->this_obj, no_scrollbar, LV_PART_SCROLLBAR);
    lv_obj_set_size(this->this_obj, lv_obj_get_width(parent), lv_obj_get_height(parent));
    lv_obj_set_align(this->this_obj, LV_ALIGN_CENTER);

    // The connection is created from a tileview
    this->tileview_obj = lv_tileview_create(this->this_obj);
    lv_obj_add_event_cb(this->tileview_obj, BluetoothConnection::tile_changed_cb, LV_EVENT_VALUE_CHANGED, this);
    lv_obj_add_style(tileview_obj, no_scrollbar, LV_PART_SCROLLBAR);
    this->connection_tile_obj = lv_tileview_add_tile(tileview_obj, 0, 0, LV_DIR_ALL);
    lv_obj_add_style(this->connection_tile_obj, no_scrollbar, LV_PART_SCROLLBAR);

    // Create a button
    this->button_obj = lv_btn_create(this->connection_tile_obj);
    lv_obj_center(this->button_obj);
    lv_obj_add_style(this->button_obj, button_no_highlight_style, LV_PART_MAIN);
    lv_obj_add_event_cb(this->button_obj, BluetoothConnection::refresh_cb, LV_EVENT_REFRESH, this);
    lv_obj_add_event_cb(this->button_obj, BluetoothConnection::exit_btn_cb, LV_EVENT_LONG_PRESSED, this);

    // Associate an image
    lv_obj_t* img_obj = lv_img_create(this->button_obj);
    lv_img_set_src(img_obj, this->image);
    lv_obj_center(img_obj);

    // Associate a spinner
    this->spinner_obj = lv_spinner_create(this->this_obj, 1000, 60);
    lv_obj_set_size(this->spinner_obj, 60, 60);
    lv_obj_center(this->spinner_obj);

    // Associate a label connecting
    this->label_obj = lv_label_create(this->this_obj);
    lv_label_set_text(this->label_obj, SEARCHING_STRING);
    lv_obj_add_style(this->label_obj, button_style, LV_PART_MAIN);
    lv_obj_center(this->label_obj);

    // create a group for the button for exit connection
    this->group = lv_group_create();
    lv_group_add_obj(this->group, this->button_obj);
    
    // create the optional monitor tile
    if (this->monitorLvObj) {        
        lv_obj_t* tile_obj = lv_tileview_add_tile(this->tileview_obj, 0, 1, LV_DIR_ALL);
        lv_obj_add_style(tile_obj, no_scrollbar, LV_PART_SCROLLBAR);
        this->monitorLvObj->createLvObj(tile_obj);
    }
    
    // Return the connection lv_obj just in case
    return this->this_obj;
}

void BluetoothConnection::destroyLvObj() {
    if (this->group) lv_group_del(this->group);
    this->group = NULL;
    BaseLvObject::destroyLvObj();
}

void BluetoothConnection::focusLvObj(BaseLvObject* defocusLvObj)
{
    if (defocusLvObj) {
        // Ensure the connection view is shown if jumping to the connection
        lv_obj_set_tile_id(this->tileview_obj, 0, 0, LV_ANIM_OFF);
        this->monitorLvObjActive = false;
        this->exiting = false;
        this->defocusLvObj = defocusLvObj;
	    this->hideButtonLabels();
        this->startBTConnection();
    } else {
        // If we are already connecting and we get focus only from the label bar
        lv_indev_set_group(this->indev, this->group);
    }
}

void BluetoothConnection::startBTConnection()
{
    this->connectingStartTime = millis();
    this->connecting_timer = lv_timer_create(connecting_timer_cb, CONNECTING_TIMEOUT_MILLIS, this);

    if (this->bluetoothController) {
        this->bluetoothController->setListenerLvObj(this->button_obj);
        this->bluetoothController->startScan();
    }
    this->connecting = true;
    this->connected = false;

    lv_obj_clear_flag(this->label_obj, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(this->spinner_obj, LV_OBJ_FLAG_HIDDEN);
}

void BluetoothConnection::stopBTConnection()
{
    this->connecting = false;
    this->connected = false;
    if (this->bluetoothController) {
        this->bluetoothController->stopScan();
        this->bluetoothController->disconnect();
    }
}

void BluetoothConnection::restartBTConnection() {
    this->startBTConnection();
}

void BluetoothConnection::hideButtonLabels()
{
	if (this->buttonLabel) {
        this->buttonLabel->setButtonLabels("", LV_SYMBOL_CLOSE, "");        
		this->buttonLabel->hide();
        this->buttonLabel->focusLvObj(this);
	}
}

void BluetoothConnection::switchToMonitorLvObject()
{
    // Ensure the monitor screen is set up and move the gui to showing it
    if (this->monitorLvObj) {
        this->monitorLvObj->setBluetoothController(this->bluetoothController);
        this->monitorLvObj->focusLvObj(this);
        lv_obj_set_tile_id(this->tileview_obj, 0, 1, LV_ANIM_ON);
        this->monitorLvObjActive = true;
    }
}

void BluetoothConnection::switchToConnectionLvObject()
{
    // Ensure the monitor screen is set up and move the gui to showing it
    if (this->monitorLvObjActive) {
        lv_obj_set_tile_id(this->tileview_obj, 0, 0, LV_ANIM_ON);
        this->monitorLvObjActive = false;
    }
}

void BluetoothConnection::connectionSuccess() 
{
    lv_obj_add_flag(this->label_obj, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(this->spinner_obj, LV_OBJ_FLAG_HIDDEN);

    // On successful connection we should ensure the bike config is set correctly
    BikeConfig bikeConfig = this->configStore->getBikeConfig();
    BluetoothBike& bluetoothBike = this->bluetoothController->getConnectedBluetoothBike();
    if (bikeConfig.beeper.managed) bluetoothBike.writeBikeStateAttribute(BikeStateAttributeIndex::BEEP_ON_OFF_STATE, &(bikeConfig.beeper.value));
    //if (bikeConfig.wheelCircumference.managed) bluetoothBike.writeBikeStateAttribute(BikeStateAttributeIndex::WHEEL_CIRCUMFERENCE, &(bikeConfig.wheelCircumference.value));
    if (bikeConfig.supportAssistLevels.managed) bluetoothBike.writeBikeStateAttribute(BikeStateAttributeIndex::SUPPORT_ASSIST_LEVELS, &(bikeConfig.supportAssistLevels.value));
    if (bikeConfig.peakPowerAssistLevels.managed) bluetoothBike.writeBikeStateAttribute(BikeStateAttributeIndex::PEAK_POWER_ASSIST_LEVELS, &(bikeConfig.peakPowerAssistLevels.value));
    
    // Ensure the monitor screen is set up and move the gui to showing it
    this->switchToMonitorLvObject();

    // The referesh is now going to connect into the monitored panel(s)
    this->connecting = false;
    this->connected = true;
}

void BluetoothConnection::checkForConnection() {
    if (this->connecting) {
        if (this->bluetoothController && this->bluetoothController->getScannedDeviceAvailable()) {
            // This should always be true as we've checked there is a scanned device above
            BLEDevice bleDevice = this->bluetoothController->getScannedDevice();
            if (bleDevice) {
                if (this->configStore->containsBTAddress(bleDevice.address().c_str())) {
                    this->bluetoothController->stopScan();
                    if (this->bluetoothController->connect(bleDevice)) {
                        switch (this->bluetoothController->getConnectedBluetoothBike().getBikeType())
                        {
                        case BikeType::GEN1:
                        case BikeType::GEN2:
                            if (this->bluetoothController->getConnectedBluetoothBike().readBikeStateAttribute(BikeStateAttributeIndex::BATTERY_CONNECTED_STATE)
                                && (this->bluetoothController->getBikeState().getStateAttribute(BikeStateAttributeIndex::BATTERY_CONNECTED_STATE).bikeStateAttributeValue.valueBool
                                    || configStore->getDisplayConfig().connectBatteryOnly))
                            {
                                // Connected
                                this->connectionSuccess();
                                return;
                            }
                            break;
                        }
                        this->bluetoothController->disconnect();
                    }
                    this->bluetoothController->startScan();
                }
            }
            this->bluetoothController->continueScan();
        }
    }
}

void BluetoothConnection::checkForConnectedUpdates() {
    if (this->connected) {
        // Check that the BT connection is still there
        if (this->bluetoothController->getConnected()) {
            // We are begin refereshed by the connection now
            this->monitorLvObj->statusUpdate();
        }
        // If the BT connection has gone down then we need to attempt to re-establish
        else {
            this->restartBTConnection();
        }
    }
}

void BluetoothConnection::refreshCB(lv_event_t* event)
{
    this->checkForConnection();
    this->checkForConnectedUpdates();
}

void BluetoothConnection::exitButtonCB(lv_event_t* event)
{
    this->exiting = true;
    // Check which screen is being shown if we're on the wrong screen then perform switch to exit screen
    if (this->monitorLvObjActive) {        
        this->switchToConnectionLvObject();
        return;
    }

    if (this->defocusLvObj) {
        this->defocusLvObj->focusLvObj();
        this->stopBTConnection();
    }
}

void BluetoothConnection::tileChangedhCB(lv_event_t* event)
{
    // Update the monitorLvObjActive to be the state we've moved to
    this->monitorLvObjActive = lv_tileview_get_tile_act(this->tileview_obj) != this->connection_tile_obj;
    if (this->exiting) this->exitButtonCB(event);
}

void BluetoothConnection::connectingTimerCB(lv_timer_t* timer) 
{
    if (this->connecting_timer == timer && this->connecting) {
        this->exitButtonCB();
    }
}