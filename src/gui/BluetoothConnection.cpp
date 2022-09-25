#include "BluetoothConnection.h"
#include <LvglThemes/lv_theme_binary.h>

static void refresh_cb(lv_event_t* event) {
	((BluetoothConnection*)(event->user_data))->refreshCB(event);
}

static void exit_btn_cb(lv_event_t* event) {
    ((BluetoothConnection*)(event->user_data))->exitButtonCB(event);
}

BluetoothConnection::BluetoothConnection(BluetoothBikeController* bluetoothController, ConfigStore* configStore, lv_img_dsc_t* image, lv_indev_t* indev) {
	this->bluetoothController = bluetoothController;
	this->configStore = configStore;
	this->image = image;
    this->indev = indev;
    this->group = NULL;
    this->connecting = false;
}

lv_obj_t* BluetoothConnection::createLvObj(lv_obj_t* parent)
{
    // get the style we'll need for the bar
    theme_binary_styles_t* binary_styles = (theme_binary_styles_t*)lv_disp_get_theme(lv_obj_get_disp(parent))->user_data;
    lv_style_t* no_scrollbar = &(binary_styles->no_scrollbar);
    lv_style_t* button_no_highlight_style = &(binary_styles->button_no_highlight);
    lv_style_t* button_style = &(binary_styles->button);

    // Create a button
    this->this_obj = lv_btn_create(parent);
    lv_obj_center(this->this_obj);
    lv_obj_add_style(this->this_obj, button_no_highlight_style, LV_PART_MAIN);
    lv_obj_add_event_cb(this->this_obj, refresh_cb, LV_EVENT_REFRESH, this);
    lv_obj_add_event_cb(this->this_obj, exit_btn_cb, LV_EVENT_CLICKED, this);    

    // Associate an image
    lv_obj_t* img_obj = lv_img_create(this->this_obj);
    lv_img_set_src(img_obj, this->image);
    lv_obj_center(img_obj);

    // Associate a spinner
    lv_obj_t* spin_obj = lv_spinner_create(parent, 1000, 60);
    lv_obj_set_size(spin_obj, 60, 60);
    lv_obj_center(spin_obj);

    // Associate a label connecting
    lv_obj_t* text_obj = lv_label_create(parent);
    lv_label_set_text(text_obj, "Connecting");
    lv_obj_add_style(text_obj, button_style, LV_PART_MAIN);
    lv_obj_center(text_obj);

    // create a group for the button for exit connection
    this->group = lv_group_create();
    lv_group_add_obj(this->group, this->this_obj);

    // Return the button just in case
    return this->this_obj;
}

void BluetoothConnection::setButtonLabel(ButtonLabel* buttonLabel)
{
	this->buttonLabel = buttonLabel;
}

void BluetoothConnection::focusLvObj(BaseLvObject* defocusLvObj)
{
    if (defocusLvObj) {
        this->defocusLvObj = defocusLvObj;
	    hideButtonLabels();
        this->startBTConnection();
    } else {
        // If we are already connecting and we get focus then get ready for user exit
        lv_indev_set_group(this->indev, this->group);
    }
}

void BluetoothConnection::startBTConnection()
{
    if (this->bluetoothController) {
        this->bluetoothController->setListenerLvObj(this->this_obj);
        this->bluetoothController->startScan();
    }
    this->connecting = true;
}

void BluetoothConnection::stopBTConnection()
{
    this->connecting = false;
    if (this->bluetoothController) {
        this->bluetoothController->stopScan();
    }
}


void BluetoothConnection::hideButtonLabels()
{
	if (this->buttonLabel) {
        this->buttonLabel->setButtonLabels("", LV_SYMBOL_CLOSE, "");        
		this->buttonLabel->hide();
        this->buttonLabel->focusLvObj(this);
	}
}


void BluetoothConnection::refreshCB(lv_event_t* event)
{
	if (this->connecting) {
		if (this->bluetoothController) {
			BLEDevice bleDevice = this->bluetoothController->getBLEDevice();
			if (bleDevice) {
                if (this->configStore->containsBTAddress(bleDevice.address().c_str())) {
				    this->bluetoothController->stopScan();
                    if (this->bluetoothController->connect(&bleDevice)) {
                        if (this->bluetoothController->getConnectedBikeType() == GEN1) {
                            LV_LOG_USER("GOT BIKE TYPE GEN1");
                            this->bluetoothController->readBatterySerialNumber();
                            this->bluetoothController->readBatteryCapacity();
                            this->bluetoothController->readBatteryCapacityRemaining();
                            this->bluetoothController->readBatteryHealth();
                            LV_LOG_USER("Capacity = %d", this->bluetoothController->getConnectedBikeState().BatteryCapacity);
                            LV_LOG_USER("CapacityRemain = %d", this->bluetoothController->getConnectedBikeState().BatteryCapacityRemaining);
                            LV_LOG_USER("CapacityHealth = %d", this->bluetoothController->getConnectedBikeState().BatteryHealth);
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

void BluetoothConnection::exitButtonCB(lv_event_t* event)
{
    BaseLvObject* lvBaseObject = (BaseLvObject*)lv_obj_get_user_data(lv_obj_get_parent(this->this_obj));
    if (lvBaseObject) {
        lvBaseObject->focusLvObj();
        this->stopBTConnection();
    }
}