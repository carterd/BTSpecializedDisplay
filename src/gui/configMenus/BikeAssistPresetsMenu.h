#ifndef _BIKE_ASSIST_PRESET_MENU_H
#define _BIKE_ASSIST_PRESET_MENU_H

#include "../ScrollMenuItem.h"
#include "../IntegerSelectMenu.h"
#include "../BikeConfigIntegerSelectMenu.h"

#include "../../dev/ConfigStore.h"

class AssistPresetSelectMenu : public IntegerSelectMenu {
private:
    ConfigStore& configStore;
    BikeConfig bikeConfig;
    BikeConfigAttributeIndex bikeConfigAttributeIndex;
    /// <summary>
    /// This identifies if manged
    /// </summary>
    bool configureAttributeManaged;
public:
	AssistPresetSelectMenu(const char* titleText, const char* exitButtonText, ConfigStore& configStore, lv_indev_t* indev, ButtonLabelBar* buttonLabelBar = NULL) :
		IntegerSelectMenu(titleText, exitButtonText, indev, buttonLabelBar), configStore(configStore) {
        this->configStore = configStore;
    }
	virtual void valueChangeCB(int newValue);
	virtual int valueInitCB() { return -1; };
	virtual void valueFinishCB() {
		// Dont do anything on normal exit
	};
};

class BikeAssistPresetsMenu {
private:
	AssistPresetSelectMenu assistPresetIntegerSelectMenu;
public:
	ScrollMenuItem configBikeMenuItem;
public:
	BikeAssistPresetsMenu(ConfigStore& configStore, lv_indev_t* indev, ButtonLabelBar* buttonLabelBar = NULL);
};

#endif