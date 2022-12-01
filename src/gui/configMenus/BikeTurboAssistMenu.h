#ifndef _BIKE_TURBO_ASSIST_MENU_H
#define _BIKE_TURBO_ASSIST_MENU_H

#include "../NavigationMenu.h"
#include "../BikeConfigIntegerSelectMenu.h"

#include "../../dev/ConfigStore.h"

#include "BikeBeeperMenu.h"
#include "BikeWheelCircumferenceMenu.h"
#include "BikeAssistMenu.h"
#include "BikeAssistLevelsMenu.h"
#include "BikePeakAssistLevelsMenu.h"

class BikeTurboAssistMenu {
private:
	NavigationMenu navigationMenu;
	BikeAssistLevelsMenu bikeAssistLevelsMenu;
	BikePeakAssistLevelsMenu bikePeakAssitLevelsMenu;
public:
	ScrollMenuItem configBikeMenuItem;
public:
	BikeTurboAssistMenu(ConfigStore& configStore, lv_indev_t* indev, ButtonLabel* buttonLabel = NULL) :
		navigationMenu("Turbo", "Back", indev, buttonLabel),
		configBikeMenuItem("Turbo Assist"),
		bikeAssistLevelsMenu("Support", BikeConfigAttributeIndex::SupportAssistTurbo, configStore, indev, buttonLabel),
		bikePeakAssitLevelsMenu("Peak Power", BikeConfigAttributeIndex::PeakPowerAssistTurbo, configStore, indev, buttonLabel)
	{
		// Bike Menus
		this->navigationMenu.addMenuItem(&bikeAssistLevelsMenu.menuItem);
		this->navigationMenu.addMenuItem(&bikePeakAssitLevelsMenu.menuItem);

		// configure the menu item
		this->configBikeMenuItem.setPopupItem(&navigationMenu);
	}
};

#endif