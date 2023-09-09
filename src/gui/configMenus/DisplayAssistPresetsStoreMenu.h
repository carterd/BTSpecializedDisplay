#ifndef _DISPLAY_ASSIST_PRESET_STORE_MENU_H
#define _DISPLAY_ASSIST_PRESET_STORE_MENU_H

#include "../ScrollMenuItem.h"
#include "../IntegerSelectMenu.h"
#include "../BikeConfigIntegerSelectMenu.h"

#include "../../dev/ConfigStore.h"

class DisplayAssistPresetsStoreSelectMenu : public IntegerSelectMenu {
private:
    ConfigStore& configStore;
    BikeConfig bikeConfig;
    BikeConfigAttributeIndex bikeConfigAttributeIndex;
    /// <summary>
    /// This identifies if manged
    /// </summary>
    bool configureAttributeManaged;
public:
	DisplayAssistPresetsStoreSelectMenu(const char* titleText, const char* exitButtonText, ConfigStore& configStore, lv_indev_t* indev, ButtonLabelBar* buttonLabelBar = NULL) :
		IntegerSelectMenu(titleText, exitButtonText, indev, buttonLabelBar), configStore(configStore) {
        this->configStore = configStore;
    }
	virtual void valueChangeCB(int newValue);
	virtual int valueInitCB();
	virtual void valueFinishCB() {
		// Dont do anything on normal exit
	};
};

class DisplayAssistPresetsDeleteSelectMenu : public IntegerSelectMenu {
private:
    ConfigStore& configStore;
    BikeConfig bikeConfig;
    BikeConfigAttributeIndex bikeConfigAttributeIndex;
public:
	DisplayAssistPresetsDeleteSelectMenu(const char* titleText, const char* exitButtonText, ConfigStore& configStore, lv_indev_t* indev, ButtonLabelBar* buttonLabelBar = NULL) :
		IntegerSelectMenu(titleText, exitButtonText, indev, buttonLabelBar), configStore(configStore) {
        this->configStore = configStore;
    }
	virtual void valueChangeCB(int newValue);
	virtual int valueInitCB();
	virtual void valueFinishCB() {
		// Dont do anything on normal exit
	};
};

class DisplayAssistPresetsStoreMenu {
private:
	DisplayAssistPresetsStoreSelectMenu displayAssistPresetsStoreSelectMenu;
	ConfigStore& configStore;
public:
	ScrollMenuItem menuItem;
public:
	DisplayAssistPresetsStoreMenu(ConfigStore& configStore, lv_indev_t* indev, ButtonLabelBar* buttonLabelBar = NULL);
};

class DisplayAssistPresetsDeleteMenu {
private:
	DisplayAssistPresetsDeleteSelectMenu displayAssistPresetsDeleteSelectMenu;
	ConfigStore& configStore;
public:
	ScrollMenuItem menuItem;
public:
	DisplayAssistPresetsDeleteMenu(ConfigStore& configStore, lv_indev_t* indev, ButtonLabelBar* buttonLabelBar = NULL);
};

#endif