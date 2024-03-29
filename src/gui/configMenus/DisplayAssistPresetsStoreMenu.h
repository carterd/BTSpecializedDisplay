#ifndef _DISPLAY_ASSIST_PRESET_STORE_MENU_H
#define _DISPLAY_ASSIST_PRESET_STORE_MENU_H

#include "../ScrollMenuItem.h"
#include "../IntegerSelectMenu.h"
#include "../BikeConfigIntegerSelectMenu.h"

#include "../../dev/ConfigStore.h"

class DisplayAssistPresetsStoreSelectMenu : public IntegerSelectMenu {
public:
	static std::vector<String> BikeAssistPresetNames;
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
	/// <summary>
	/// This Creates the instance of the Lv Objects associated with the instance and sub components associated with it
	/// </summary>
	/// <param name="parent">The LV object parent on which to attach the created lv object for this instance</param>
	/// <returns>The created LV object instance</returns>
	virtual lv_obj_t* createLvObj(lv_obj_t* parent);

	/// <summary>
	/// What to do when something is selected
	/// </summary>
	virtual void valueChangeCB(int newValue);

	/// <summary>
	/// No values are currently selected in the menu
	/// </summary>
	virtual int valueInitCB() { return -1; }

	/// <summary>
	/// Nothing happens if exit button pressed
	/// </summary>
	virtual void valueFinishCB() {};
};

class DisplayAssistPresetsStoreMenu {
private:
	DisplayAssistPresetsStoreSelectMenu displayAssistPresetsStoreSelectMenu;
	ConfigStore& configStore;
public:
	ScrollMenuItem menuItem;
public:
	DisplayAssistPresetsStoreMenu(ConfigStore& configStore, lv_indev_t* indev, ButtonLabelBar* buttonLabelBar = NULL) :
		displayAssistPresetsStoreSelectMenu("Store Presets", "Back", configStore, indev, buttonLabelBar),
		menuItem("Store"),
		configStore(configStore)
	{
		this->menuItem.setPopupItem(&this->displayAssistPresetsStoreSelectMenu);
	}
};

#endif