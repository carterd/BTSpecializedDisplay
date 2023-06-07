#include "BikeAssistPresetsMenu.h"
#include "..\..\config\AssistPresets.h"

BikeAssistPresetsMenu::BikeAssistPresetsMenu(ConfigStore& configStore, lv_indev_t* indev, ButtonLabelBar* buttonLabelBar) :
	assistPresetIntegerSelectMenu("Presets", "Back", configStore, indev, buttonLabelBar),
	configBikeMenuItem("Presets")
{
	for (int i = 0; i < BIKE_ASSIST_LEVELS; i++) {
		this->assistPresetIntegerSelectMenu.addMenuItem(bikeAssistPresets[i].Name, i);
	}
	this->configBikeMenuItem.setPopupItem(&this->assistPresetIntegerSelectMenu);
}

void AssistPresetSelectMenu::valueChangeCB(int newValue) {
	if (newValue >= 0 && newValue < BIKE_ASSIST_LEVELS) {
		BikeConfig bikeConfig = this->configStore.getBikeConfig();
		bikeConfig.setIntValue(BikeConfigAttributeIndex::SupportAssistEco, bikeAssistPresets[newValue].supportAssistLevels.eco);
		bikeConfig.setIntValue(BikeConfigAttributeIndex::SupportAssistTrail, bikeAssistPresets[newValue].supportAssistLevels.trail);
		bikeConfig.setIntValue(BikeConfigAttributeIndex::SupportAssistTurbo, bikeAssistPresets[newValue].supportAssistLevels.turbo);
		bikeConfig.setManagedValue(BikeConfigAttributeIndex::SupportAssistLevelsManaged, true);

		bikeConfig.setIntValue(BikeConfigAttributeIndex::PeakPowerAssistEco, bikeAssistPresets[newValue].peakPowerAssistLevels.eco);
		bikeConfig.setIntValue(BikeConfigAttributeIndex::PeakPowerAssistTrail, bikeAssistPresets[newValue].peakPowerAssistLevels.trail);
		bikeConfig.setIntValue(BikeConfigAttributeIndex::PeakPowerAssistTurbo, bikeAssistPresets[newValue].peakPowerAssistLevels.turbo);
		bikeConfig.setManagedValue(BikeConfigAttributeIndex::PeakPowerAssistLevelsManaged, true);
		
		this->configStore.updateBikeConfig(bikeConfig);


	}
	this->exitButtonCB(NULL);
}
