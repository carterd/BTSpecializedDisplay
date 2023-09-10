#include "DisplayAssistPresetsDeleteMenu.h"
#include "..\..\config\AssistPresets.h"

lv_obj_t* DisplayAssistPresetsDeleteSelectMenu::createLvObj(lv_obj_t* parent)
{
	this->deleteAllMenuItems();
	int i = 0;
	for (auto it = this->configStore.getDisplayConfig().bikeAssistPresets.begin(); it != this->configStore.getDisplayConfig().bikeAssistPresets.end(); it++) {
		this->addMenuItem(it->Name.c_str(), i);
		i++;
	}
	return ValueSelectMenu::createLvObj(parent);
}

void DisplayAssistPresetsDeleteSelectMenu::valueChangeCB(int newValue) {
	if (newValue >= 0 && newValue < this->configStore.getDisplayConfig().bikeAssistPresets.size()) {
		DisplayConfig displayConfig = this->configStore.getDisplayConfig();
		displayConfig.bikeAssistPresets.erase(displayConfig.bikeAssistPresets.begin() + newValue);
		this->configStore.updateDisplayConfig(displayConfig);
	}
	this->exitButtonCB(NULL);
}
