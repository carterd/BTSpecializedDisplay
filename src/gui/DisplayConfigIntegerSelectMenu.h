#ifndef _DISPLAY_CONFIG_INTEGER_SELECT_MENU_H
#define _DISPLAY_CONFIG_INTEGER_SELECT_MENU_H

#include "IntegerSelectMenu.h"
#include "..\dev\ConfigStore.h"
#include "..\dev\Display.h"

class DisplayConfigIntegerSelectMenu : public IntegerSelectMenu {    
private:
    ConfigStore& configStore;
    DisplayConfig displayConfig;
    Display& display;
    DisplayConfigAttributeIndex displayConfigAttributeIndex;
public:
    DisplayConfigIntegerSelectMenu(const char* titleText, const char* exitButtonText, ConfigStore& configStore, DisplayConfigAttributeIndex displayConfigAttributeIndex, Display &display, lv_indev_t* indev, ButtonLabel* buttonLabel = NULL) :
        IntegerSelectMenu(titleText, exitButtonText, indev, buttonLabel), configStore(configStore), display(display) {
        this->configStore = configStore;
        this->displayConfigAttributeIndex = displayConfigAttributeIndex;
    }    
    virtual void valueChangeCB(int newValue) {
        this->displayConfig.setIntValue(displayConfigAttributeIndex, newValue);
        if (this->displayConfigAttributeIndex == DisplayConfigAttributeIndex::Contrast) {
            this->display.setContrast(this->displayConfig.contrast);            
        }
    };
    virtual int valueInitCB() {
        this->displayConfig = this->configStore.getDisplayConfig();
        return this->displayConfig.getIntValue(this->displayConfigAttributeIndex);
    };
    virtual void valueFinishCB() { 
        this->configStore.updateDisplayConfig(this->displayConfig);
    };
};

#endif