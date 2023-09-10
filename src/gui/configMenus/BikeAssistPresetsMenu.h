#ifndef _BIKE_ASSIST_PRESET_MENU_H
#define _BIKE_ASSIST_PRESET_MENU_H

#include "../ScrollMenuItem.h"
#include "../IntegerSelectMenu.h"
#include "../BikeConfigIntegerSelectMenu.h"

#include "../../dev/ConfigStore.h"

class BikeAssistPresetsSelectMenu : public IntegerSelectMenu {
private:
    ConfigStore& configStore;
    BikeConfig bikeConfig;
    BikeConfigAttributeIndex bikeConfigAttributeIndex;
    /// <summary>
    /// This identifies if manged
    /// </summary>
    bool configureAttributeManaged;
public:
	BikeAssistPresetsSelectMenu(const char* titleText, const char* exitButtonText, ConfigStore& configStore, lv_indev_t* indev, ButtonLabelBar* buttonLabelBar = NULL) :
		IntegerSelectMenu(titleText, exitButtonText, indev, buttonLabelBar), configStore(configStore) {
        this->configStore = configStore;
    }

	/// <summary>
	/// This Creates the instance of the Lv Objects associated with the instance and sub components associated with it
	/// </summary>
	/// <param name="parent">The LV object parent on which to attach the created lv object for this instance</param>
	/// <returns>The created LV object instance</returns>
	virtual lv_obj_t* createLvObj(lv_obj_t* parent);

	virtual void valueChangeCB(int newValue);
	virtual int valueInitCB() { return -1; }
	virtual void valueFinishCB() {
		// Dont do anything on normal exit
	};
};

class BikeAssistPresetsMenu {
private:
	BikeAssistPresetsSelectMenu bikeAssistPresetsSelectMenu;
	ConfigStore& configStore;
public:
	ScrollMenuItem configBikeMenuItem;
public:
	BikeAssistPresetsMenu(ConfigStore& configStore, lv_indev_t* indev, ButtonLabelBar* buttonLabelBar = NULL) :
		bikeAssistPresetsSelectMenu("Presets", "Back", configStore, indev, buttonLabelBar),
		configBikeMenuItem("Presets"),
		configStore(configStore)
	{
		this->configBikeMenuItem.setPopupItem(&this->bikeAssistPresetsSelectMenu);
	}
};

#endif