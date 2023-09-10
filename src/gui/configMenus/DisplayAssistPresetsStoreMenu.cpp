#include "DisplayAssistPresetsStoreMenu.h"
#include "..\..\config\AssistPresets.h"

lv_obj_t* DisplayAssistPresetsStoreSelectMenu::createLvObj(lv_obj_t* parent)
{
	this->deleteAllMenuItems();
	int i = 0;
	for (auto it = BikeAssistPresetNames.begin(); it != BikeAssistPresetNames.end(); it++) {
		this->addMenuItem(it->c_str(), i);
		i++;
	}
	return ValueSelectMenu::createLvObj(parent);
}

void DisplayAssistPresetsStoreSelectMenu::valueChangeCB(int newValue) {
	BikeConfig bikeConfig = this->configStore.getBikeConfig();	

	if (newValue >= 0 && newValue < BikeAssistPresetNames.size()) {
		BikeAssistPreset bikeAssistPreset(BikeAssistPresetNames[newValue], this->configStore.getBikeConfig().supportAssistLevels.value, this->configStore.getBikeConfig().peakPowerAssistLevels.value);
        DisplayConfig displayConfig = this->configStore.getDisplayConfig();
        for (auto it = displayConfig.bikeAssistPresets.begin(); it != displayConfig.bikeAssistPresets.end(); it++) {
            if (it->Name == BikeAssistPresetNames[newValue]) {
                displayConfig.bikeAssistPresets.erase(it);
                break;
            }
        }
        displayConfig.bikeAssistPresets.push_back(bikeAssistPreset);
        this->configStore.updateDisplayConfig(displayConfig);
	}
	this->exitButtonCB(NULL);
}


std::vector<String> DisplayAssistPresetsStoreSelectMenu::BikeAssistPresetNames = { 
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