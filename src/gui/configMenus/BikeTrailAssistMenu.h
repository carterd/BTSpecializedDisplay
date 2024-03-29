#ifndef _BIKE_TRAIL_ASSIST_MENU_H
#define _BIKE_TRAIL_ASSIST_MENU_H

#include "../NavigationMenu.h"
#include "../BikeConfigIntegerSelectMenu.h"

#include "../../dev/ConfigStore.h"

#include "BikeBeeperMenu.h"
#include "BikeWheelCircumferenceMenu.h"
#include "BikeAssistMenu.h"
#include "BikeAssistLevelsMenu.h"
#include "BikePeakAssistLevelsMenu.h"

class BikeTrailAssistMenu {
private:
	NavigationMenu navigationMenu;
	BikeAssistLevelsMenu bikeAssistLevelsMenu;
	BikePeakAssistLevelsMenu bikePeakAssitLevelsMenu;
public:
	ScrollMenuItem configBikeMenuItem;
public:
	BikeTrailAssistMenu(ConfigStore& configStore, lv_indev_t* indev, ButtonLabelBar* buttonLabelBar = NULL) :
		navigationMenu("Trail", "Back", indev, buttonLabelBar),
		configBikeMenuItem("Trail Assist"),
		bikeAssistLevelsMenu("Support", BikeConfigAttributeIndex::SupportAssistTrail, configStore, indev, buttonLabelBar),
		bikePeakAssitLevelsMenu("Peak Power", BikeConfigAttributeIndex::PeakPowerAssistTrail, configStore, indev, buttonLabelBar)
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