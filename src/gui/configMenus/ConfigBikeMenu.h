#ifndef _CONFIG_BIKE_MENU_H
#define _CONFIG_BIKE_MENU_H

#include "../NavigationMenu.h"
#include "../BikeConfigIntegerSelectMenu.h"

#include "../../dev/ConfigStore.h"

#include "BikeBeeperMenu.h"
#include "BikeWheelCircumferenceMenu.h"
#include "BikeAssistMenu.h"

class ConfigBikeMenu {
private:
	NavigationMenu bikeNavigationMenu;
    BikeBeeperMenu bikeBeeperMenu;
	//BikeWheelCircumferenceMenu bikeWheelCircumferenceMenu;
	BikeAssistMenu bikeAssistMenu;
public:
	ScrollMenuItem configBikeMenuItem;
public:
	ConfigBikeMenu(ConfigStore& configStore, lv_indev_t* indev, ButtonLabelBar* buttonLabel = NULL) :
		bikeNavigationMenu("Bike", "Back", indev, buttonLabel),
		configBikeMenuItem("Bike"),
        bikeBeeperMenu(configStore, indev, buttonLabel),
		//bikeWheelCircumferenceMenu(configStore, indev, buttonLabel),
		bikeAssistMenu(configStore, indev, buttonLabel)
	{
        // Bike Menus
        this->bikeNavigationMenu.addMenuItem(&bikeBeeperMenu.menuItem);    
		this->bikeNavigationMenu.addMenuItem(&bikeAssistMenu.menuItem);
		//this->bikeNavigationMenu.addMenuItem(&bikeWheelCircumferenceMenu.menuItem);		

		// configure the menu item
        this->configBikeMenuItem.setPopupItem(&bikeNavigationMenu);
	}
};

#endif