#include "DisplayAssistPresetsStoreMenu.h"
#include "..\..\config\AssistPresets.h"

DisplayAssistPresetsDeleteMenu::DisplayAssistPresetsDeleteMenu(ConfigStore& configStore, lv_indev_t* indev, ButtonLabelBar* buttonLabelBar) :
	displayAssistPresetsDeleteSelectMenu("Delete Presets", "Back", configStore, indev, buttonLabelBar),
	menuItem("Delete"),
	configStore(configStore)
{
	int i = 0;
	for (auto it = this->configStore.getDisplayConfig().bikeAssistPresets.begin(); it != this->configStore.getDisplayConfig().bikeAssistPresets.end(); it++) {
		this->displayAssistPresetsDeleteSelectMenu.addMenuItem(it->Name.c_str(), i);
		i++;
	}
	this->menuItem.setPopupItem(&this->displayAssistPresetsDeleteSelectMenu);
}

DisplayAssistPresetsStoreMenu::DisplayAssistPresetsStoreMenu(ConfigStore& configStore, lv_indev_t* indev, ButtonLabelBar* buttonLabelBar) :
	displayAssistPresetsStoreSelectMenu("Store Presets", "Back", configStore, indev, buttonLabelBar),
	menuItem("Store"),
	configStore(configStore)
{
	this->menuItem.setPopupItem(&this->displayAssistPresetsStoreSelectMenu);
}

int DisplayAssistPresetsStoreSelectMenu::valueInitCB() {
	this->deleteAllMenuItems();
	int i = 0;
	// Show all the preset names for storage
	for (auto it = BikeAssistPresetNames.begin(); it != BikeAssistPresetNames.end(); it++) {
		this->addMenuItem(it->c_str(), i);
		i++;
	}
	// Nothing selected
	return -1;
}

void DisplayAssistPresetsStoreSelectMenu::valueChangeCB(int newValue) {
	BikeConfig bikeConfig = this->configStore.getBikeConfig();	
	/*
	if (newValue >= 0 && newValue < this->configStore.getDisplayConfig().bikeAssistPresets.size()) {
		BikeConfig bikeConfig = this->configStore.getBikeConfig();
		bikeConfig.setIntValue(BikeConfigAttributeIndex::SupportAssistEco, this->configStore.getDisplayConfig().bikeAssistPresets[newValue].supportAssistLevels.eco);
		bikeConfig.setIntValue(BikeConfigAttributeIndex::SupportAssistTrail, this->configStore.getDisplayConfig().bikeAssistPresets[newValue].supportAssistLevels.trail);
		bikeConfig.setIntValue(BikeConfigAttributeIndex::SupportAssistTurbo, this->configStore.getDisplayConfig().bikeAssistPresets[newValue].supportAssistLevels.turbo);
		bikeConfig.setManagedValue(BikeConfigAttributeIndex::SupportAssistLevelsManaged, true);

		bikeConfig.setIntValue(BikeConfigAttributeIndex::PeakPowerAssistEco, this->configStore.getDisplayConfig().bikeAssistPresets[newValue].peakPowerAssistLevels.eco);
		bikeConfig.setIntValue(BikeConfigAttributeIndex::PeakPowerAssistTrail, this->configStore.getDisplayConfig().bikeAssistPresets[newValue].peakPowerAssistLevels.trail);
		bikeConfig.setIntValue(BikeConfigAttributeIndex::PeakPowerAssistTurbo, this->configStore.getDisplayConfig().bikeAssistPresets[newValue].peakPowerAssistLevels.turbo);
		bikeConfig.setManagedValue(BikeConfigAttributeIndex::PeakPowerAssistLevelsManaged, true);
		
		this->configStore.updateBikeConfig(bikeConfig);


	}
	*/
	this->exitButtonCB(NULL);
}

int DisplayAssistPresetsDeleteSelectMenu::valueInitCB() {
	this->deleteAllMenuItems();
	int i = 0;	
	for (auto it = this->configStore.getDisplayConfig().bikeAssistPresets.begin(); it != this->configStore.getDisplayConfig().bikeAssistPresets.end(); it++) {
		this->addMenuItem(it->Name.c_str(), i);
		i++;
	}
	// Nothing selected
	return -1;
}

void DisplayAssistPresetsDeleteSelectMenu::valueChangeCB(int newValue) {
	if (newValue >= 0 && newValue < this->configStore.getDisplayConfig().bikeAssistPresets.size()) {
		DisplayConfig displayConfig = this->configStore.getDisplayConfig();
		displayConfig.bikeAssistPresets.erase(displayConfig.bikeAssistPresets.begin() + newValue);
		this->configStore.updateDisplayConfig(displayConfig);
	}
	this->exitButtonCB(NULL);
}

static std::vector<String> BikeAssistPresetNames = { 
    PRESET_NAME_DEFAULT,
    PRESET_NAME_LOW,
    PRESET_NAME_ECO,
    PRESET_NAME_LIMP,
    PRESET_NAME_NORMAL,
    PRESET_NAME_MEDIUM,
    PRESET_NAME_HIGH,
    PRESET_NAME_THRASH,
    PRESET_NAME_FAVOURITE,
    PRESET_NAME_PRESET_1,
    PRESET_NAME_PRESET_2,
    PRESET_NAME_PRESET_3,
    PRESET_NAME_PRESET_4 };