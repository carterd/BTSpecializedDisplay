#include "BikeAssistPresetsMenu.h"
#include "..\..\config\AssistPresets.h"

BikeAssistPresetsMenu::BikeAssistPresetsMenu(ConfigStore& configStore, lv_indev_t* indev, ButtonLabelBar* buttonLabelBar) :
	assistPresetIntegerSelectMenu("Presets", "Back", configStore, indev, buttonLabelBar),
	configBikeMenuItem("Presets"),
	configStore(configStore)
{
	this->configBikeMenuItem.setPopupItem(&this->assistPresetIntegerSelectMenu);
}

int AssistPresetSelectMenu::valueInitCB() {
	this->deleteAllMenuItems();
	int i = 0;
	for (auto it = this->configStore.getDisplayConfig().bikeAssistPresets.begin(); it != this->configStore.getDisplayConfig().bikeAssistPresets.end(); it++) {
		this->addMenuItem(it->Name.c_str(), i);
		i++;
	}

	// Back button is pre selected
	return -1;
}

void AssistPresetSelectMenu::valueChangeCB(int newValue) {
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
	this->exitButtonCB(NULL);
}
