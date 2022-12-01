#ifndef _BIKE_CONFIG_INTEGER_SELECT_MENU_H
#define _BIKE_CONFIG_INTEGER_SELECT_MENU_H

#include "IntegerSelectMenu.h"
#include "..\dev\ConfigStore.h"
#include "..\dev\Display.h"

class BikeConfigIntegerSelectMenu : public IntegerSelectMenu {    
private:
    ConfigStore& configStore;
    BikeConfig bikeConfig;
    BikeConfigAttributeIndex bikeConfigAttributeIndex;
    /// <summary>
    /// This identifies if manged
    /// </summary>
    bool configureAttributeManaged;
public:
    BikeConfigIntegerSelectMenu(const char* titleText, const char* exitButtonText, ConfigStore& configStore, BikeConfigAttributeIndex bikeConfigAttributeIndex, bool configureAttributeManaged, lv_indev_t* indev, ButtonLabel* buttonLabel = NULL) :
        IntegerSelectMenu(titleText, exitButtonText, indev, buttonLabel), configStore(configStore) {
        this->configStore = configStore;
        this->bikeConfigAttributeIndex = bikeConfigAttributeIndex;
        this->configureAttributeManaged = configureAttributeManaged;
    }    
    virtual void valueChangeCB(int newValue) {
        if (newValue < 0) {
            if (this->configureAttributeManaged) this->bikeConfig.setManagedValue(this->bikeConfigAttributeIndex, false);
        }
        else {
            if (this->configureAttributeManaged) this->bikeConfig.setManagedValue(this->bikeConfigAttributeIndex, true);
            this->bikeConfig.setIntValue(this->bikeConfigAttributeIndex, newValue);            
        }
    };
    virtual int valueInitCB() {
        this->bikeConfig = this->configStore.getBikeConfig();
        
        if (this->configureAttributeManaged && !this->bikeConfig.getManagedValue(this->bikeConfigAttributeIndex)) {
            return -1;
        }
        
        return this->bikeConfig.getIntValue(this->bikeConfigAttributeIndex);
    };
    virtual void valueFinishCB() { 
        this->configStore.updateBikeConfig(this->bikeConfig);
    };
};

#endif