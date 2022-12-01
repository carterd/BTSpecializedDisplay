#ifndef _BIKE_ECO_ASSIST_MENU_H
#define _BIKE_ECO_ASSIST_MENU_H

#include "../NavigationMenu.h"
#include "../BikeConfigIntegerSelectMenu.h"

#include "../../dev/ConfigStore.h"

#include "BikeBeeperMenu.h"
#include "BikeWheelCircumferenceMenu.h"
#include "BikeAssistMenu.h"
#include "BikeAssistLevelsMenu.h"
#include "BikePeakAssistLevelsMenu.h"

class BikeEcoAssistMenu {
private:
	NavigationMenu navigationMenu;	
	BikeAssistLevelsMenu bikeAssistLevelsMenu;
	BikePeakAssistLevelsMenu bikePeakAssitLevelsMenu;
public:
	ScrollMenuItem configBikeMenuItem;
public:
	BikeEcoAssistMenu(ConfigStore& configStore, lv_indev_t* indev, ButtonLabel* buttonLabel = NULL) :
		navigationMenu("Eco", "Back", indev, buttonLabel),
		configBikeMenuItem("Eco Assist"),
		bikeAssistLevelsMenu("Support", BikeConfigAttributeIndex::SupportAssistEco, configStore, indev, buttonLabel),
		bikePeakAssitLevelsMenu("Peak Power", BikeConfigAttributeIndex::PeakPowerAssistEco, configStore, indev, buttonLabel)
	{
		// Bike Menus
		this->navigationMenu.addMenuItem(&bikeAssistLevelsMenu.menuItem);
		this->navigationMenu.addMenuItem(&bikePeakAssitLevelsMenu.menuItem);
		//this->bikeNavigationMenu.addMenuItem(&bikeWheelCircumferenceMenu.menuItem);		

		// configure the menu item
		this->configBikeMenuItem.setPopupItem(&navigationMenu);
	}
};

#endif