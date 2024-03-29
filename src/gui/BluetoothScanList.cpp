#include "BluetoothScanList.h"
#include "ButtonLabelBar.h"
#include "../themes/display_theme.h"

void BluetoothScanList::exit_btn_cb(lv_event_t* event) {
	((BluetoothScanList*) (event->user_data))->exitButtonCB(event);
}

void BluetoothScanList::device_btn_cb(lv_event_t* event) {
	if (event->code == LV_EVENT_CLICKED) {
		((BluetoothScanList*)(event->user_data))->deviceButtonCB(event);
	}
	if (event->code == LV_EVENT_FOCUSED) {
		lv_obj_t* btnLabel = lv_obj_get_child(event->target, 1);
		if (btnLabel)  lv_label_set_long_mode(btnLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);
	}
	if (event->code == LV_EVENT_DEFOCUSED) {
		lv_obj_t* btnLabel = lv_obj_get_child(event->target, 1);
		if (btnLabel) lv_label_set_long_mode(btnLabel, LV_LABEL_LONG_CLIP);
	}
}

void BluetoothScanList::refresh_cb(lv_event_t* event) {
	((BluetoothScanList*)(event->user_data))->refreshCB(event);
}

BluetoothScanList::BluetoothScanList(BluetoothScanner* bluetoothScanner, ConfigStore* configStore, lv_indev_t* indev, ButtonLabelBar* buttonLabelBar) : ButtonLabelledLvObject(indev, buttonLabelBar)
{
	this->indev = indev;
	this->bluetoothScanner = bluetoothScanner;
	this->configStore = configStore;
	this->scanning = false;
	this->defocusLvObj = NULL;
}

BluetoothScanList::~BluetoothScanList()
{
	BluetoothScanList::destroyLvObj();
}

lv_obj_t* BluetoothScanList::createLvObj(lv_obj_t* parent)
{
	// get the style we'll need for the bar
	display_theme_styles_t* display_theme_styles = (display_theme_styles_t*)lv_disp_get_theme(lv_obj_get_disp(parent))->user_data;
	lv_style_t* menu_label_bar_style = &(display_theme_styles->menu_label_bar);
	lv_style_t* inv_style = &(display_theme_styles->inv);

	this->group = lv_group_create();
	lv_group_set_wrap(this->group, false);

	this->this_obj = lv_obj_create(parent);	
	lv_obj_set_size(this->this_obj, lv_obj_get_width(parent), lv_obj_get_height(parent));
	lv_obj_center(this->this_obj);
	lv_obj_add_event_cb(this->this_obj, BluetoothScanList::refresh_cb, LV_EVENT_REFRESH, this);

	// add list top label bar
	lv_obj_t* list_label = lv_obj_create(this->this_obj);
	lv_obj_set_width(list_label, lv_obj_get_width(parent));
	lv_obj_add_style(list_label, menu_label_bar_style, LV_PART_MAIN);
	//lv_obj_add_style(list_label, inv_style, LV_PART_MAIN);
	lv_obj_update_layout(list_label);
	int labelHeight = lv_obj_get_style_height(list_label, LV_PART_MAIN);

	lv_obj_t* scan_label = lv_label_create(list_label);
	lv_label_set_text(scan_label, "Scanning");
	lv_obj_align(scan_label, LV_ALIGN_LEFT_MID, 1, 0);

	this->scan_anim_obj = lv_label_create(list_label);
	lv_label_set_text(this->scan_anim_obj, "...");
	lv_obj_set_width(this->scan_anim_obj, 8);
	lv_obj_align(this->scan_anim_obj, LV_ALIGN_RIGHT_MID, -1, 0);
	lv_label_set_long_mode(this->scan_anim_obj, LV_LABEL_LONG_CLIP);

	// add list
	this->list_obj = lv_list_create(this->this_obj);
	lv_obj_set_size(this->list_obj, lv_obj_get_width(parent), lv_obj_get_height(parent) - labelHeight - this->buttonLabelBar->getHeight() - 2);
	lv_obj_align(this->list_obj, LV_ALIGN_TOP_MID, 0, labelHeight + 1);	
	
	//Add buttons to the list
	lv_obj_t* btn_label;

	this->exit_button_obj = lv_list_add_btn(this->list_obj, LV_SYMBOL_PREV, "Exit");
	lv_obj_add_event_cb(this->exit_button_obj, BluetoothScanList::exit_btn_cb, LV_EVENT_CLICKED, this);
	btn_label = lv_obj_get_child(this->exit_button_obj, 0);
	if (lv_obj_has_class(btn_label, &lv_label_class)) {
		lv_label_set_long_mode(btn_label, LV_LABEL_LONG_CLIP);
	}
	lv_group_add_obj(this->group, this->exit_button_obj);

	return this->this_obj;
}

void BluetoothScanList::destroyLvObj()
{
	if (this->group) lv_group_del(this->group);
	this->group = NULL;
	BaseLvObject::destroyLvObj();
}

void BluetoothScanList::focusLvObj(BaseLvObject* defocusLvObj)
{	
	this->defocusLvObj = defocusLvObj;
	lv_indev_set_group(this->indev, this->group);
	updateButtonLabelBar();

	this->btAddressConfig = this->configStore->getBTAddressesConfig();

	removeDeviceItems();
	this->buttonAddressMap.clear();
	addKnownDeviceItems();
	startBTScan();
}

void BluetoothScanList::updateButtonLabelBar() {
	if (this->buttonLabelBar) {
		this->buttonLabelBar->setButtonLabels(LV_SYMBOL_UP, LV_SYMBOL_OK, LV_SYMBOL_DOWN);
		this->buttonLabelBar->show();
	}
}

void BluetoothScanList::startBTScan()
{
	if (this->bluetoothScanner) {
		this->bluetoothScanner->setListenerLvObj(this->this_obj);
		this->bluetoothScanner->startScan();
	}
	lv_label_set_long_mode(this->scan_anim_obj, LV_LABEL_LONG_SCROLL_CIRCULAR);
	this->scanning = true;
}

void BluetoothScanList::stopBTScan()
{
	this->scanning = false;
	if (this->bluetoothScanner) {
		this->bluetoothScanner->stopScan();
	}
	lv_label_set_long_mode(this->scan_anim_obj, LV_LABEL_LONG_CLIP);	
}

lv_obj_t* BluetoothScanList::addDeviceItem(BLEDevice* bleDevice)
{
	// Attempt to get the current button on the screen for the given device
	String addressString = bleDevice->address();

	// Generate the display string
	String displayString = bleDevice->hasLocalName() ? bleDevice->localName().c_str() : bleDevice->deviceName().c_str();
	if (displayString.length() == 0) {
#ifdef _XSTRING_
		displayString.append("unknown (");
		displayString.append(addressString);
		displayString.append(")");
#else
		displayString.concat("unknown (");
		displayString.concat(addressString);
		displayString.concat(")");
#endif
	}
	return this->addDeviceItem(addressString, displayString);
}

lv_obj_t* BluetoothScanList::addDeviceItem(const String& addressString, const String& displayString) {
	bool knownDevice = false;

	// Get the list button for the given address if already in the list
	lv_obj_t* list_btn = this->getDeviceButton(addressString);

	// If the config has this address configured then we need to set knowDevice and also check to update an existing label
	if (this->btAddressConfig.containsBTAddress(addressString)) {
		knownDevice = true;
		if (displayString != (*(this->btAddressConfig.getBTAddressDisplayString(addressString)))) {
			// It's a known device but we need to update the label in the store
			this->btAddressConfig.addBTAddress(addressString, displayString);
			if (list_btn != NULL) {
				lv_obj_t* btn_label = lv_obj_get_child(list_btn, 1);
				if (lv_obj_has_class(btn_label, &lv_label_class)) {
					lv_label_set_text(btn_label, displayString.c_str());
				}
			}
		}
	}
	
	// If the list button doesn't already exist lets create it now
	if (list_btn == NULL) {
		list_btn = this->addDeviceButton(displayString.c_str(), knownDevice);		
	}
	// Update the found list button with any changes in device
	this->buttonAddressMap[list_btn] = addressString;

	return list_btn;
}

lv_obj_t* BluetoothScanList::addDeviceButton(const char* displayText, bool knownDevice) {
	lv_obj_t* list_btn = lv_list_add_btn(this->list_obj, LV_SYMBOL_OK, displayText);	

	lv_obj_t* btn_img = lv_obj_get_child(list_btn, 0);
	if (lv_obj_has_class(btn_img, &lv_img_class) && !knownDevice) {
		lv_obj_add_flag(btn_img, LV_OBJ_FLAG_HIDDEN);
	}
	lv_obj_t* btn_label = lv_obj_get_child(list_btn, 1);
	if (lv_obj_has_class(btn_label, &lv_label_class)) {
		lv_label_set_long_mode(btn_label, LV_LABEL_LONG_CLIP);
	}
	lv_obj_add_event_cb(list_btn, BluetoothScanList::device_btn_cb, LV_EVENT_CLICKED, this);
	lv_obj_add_event_cb(list_btn, BluetoothScanList::device_btn_cb, LV_EVENT_FOCUSED, this);
	lv_obj_add_event_cb(list_btn, BluetoothScanList::device_btn_cb, LV_EVENT_DEFOCUSED, this);
	lv_group_add_obj(this->group, list_btn);

	return list_btn;
}

lv_obj_t* BluetoothScanList::getDeviceButton(const String& addressString) {
	for (std::unordered_map<lv_obj_t*, String>::iterator it = this->buttonAddressMap.begin(); it != this->buttonAddressMap.end(); it++) {
#ifdef _XSTRING_
		bool match = it->second == addressString;
#else 
		bool match = it->second.equals(addressString);
#endif
		if (match) {
			return it->first;
		}
	}
	return NULL;
}

void BluetoothScanList::refreshCB(lv_event_t* event)
{
	if (this->scanning) {
		if (this->bluetoothScanner && this->bluetoothScanner->getScannedDeviceAvailable()) {
			// This bleDevice should always be true as we've already idenfied scanned device available
			BLEDevice bleDevice = this->bluetoothScanner->getScannedDevice();
			this->bluetoothScanner->continueScan();
			if (bleDevice) {
				if (lv_obj_get_child_cnt(this->list_obj) < 20) {
					this->addDeviceItem(&bleDevice);
				}
				if (lv_obj_get_child_cnt(this->list_obj) == 20) {
					this->scanning = false;
					lv_label_set_long_mode(this->scan_anim_obj, LV_LABEL_LONG_CLIP);
				}
			}
		}
	}
}

void BluetoothScanList::exitButtonCB(lv_event_t* event)
{
	if (this->defocusLvObj) {
		this->defocusLvObj->focusLvObj();
		this->configStore->updateBTAddressesConfig(this->btAddressConfig);
		this->stopBTScan();
	}
}

void BluetoothScanList::deviceButtonCB(lv_event_t* event)
{
	//iterator<std::map<lv_obj_t*, BluetoothMaster::BLEDetails>> 
	if (this->buttonAddressMap.count(event->target)) {
		std::unordered_map<lv_obj_t*, String>::iterator it = this->buttonAddressMap.find(event->target);
		String address = (*it).second;
		String display = lv_label_get_text(lv_obj_get_child(event->target, 1));
		lv_obj_t* btn_img = lv_obj_get_child(event->target, 0);

		if (this->btAddressConfig.containsBTAddress(address)) {
			this->btAddressConfig.removeBTAddress(address);
			lv_obj_add_flag(btn_img, LV_OBJ_FLAG_HIDDEN);
		}
		else
		{
			// TODO: Check that it's a bike fit for registering
			this->btAddressConfig.addBTAddress(address, display);
			lv_obj_clear_flag(btn_img, LV_OBJ_FLAG_HIDDEN);
		}

		if (lv_obj_has_class(btn_img, &lv_img_class)) {
			
		}
	}
}

void BluetoothScanList::removeDeviceItems()
{
	int childCount = lv_obj_get_child_cnt(this->list_obj);
	for (int childId = childCount - 1; childId >= 0; childId--) {
		lv_obj_t* childObj = lv_obj_get_child(this->list_obj, childId);
		if (childObj != this->exit_button_obj) {
			lv_obj_del(childObj);
		}
	}
}

void BluetoothScanList::addKnownDeviceItems()
{
	for (std::unordered_map<String, String>::iterator it = this->btAddressConfig.btAddressMap.begin(); it != this->btAddressConfig.btAddressMap.end(); it++) {
		this->addDeviceItem((*it).first, (*it).second);
	}
}

