#ifndef _CONFIG_DISPLAY_MENU_H
#define _CONFIG_DISPLAY_MENU_H

#include "../ScrollMenuItem.h"
#include "../NavigationMenu.h"
#include "../BikeConfigIntegerSelectMenu.h"

#include "../../dev/ConfigStore.h"

#include "DisplayBrightnessMenu.h"
#include "DisplayConnectBatteryOnlyMenu.h"
#include "DisplayConnectOnBootMenu.h"

class ConfigDisplayMenu {
private:
    NavigationMenu bikeNavigationMenu;
    DisplayBrightnessMenu displayBrightnessMenu;
    DisplayConnectBatteryOnlyMenu displayConnectBatteryOnlyMenu;
    DisplayConnectOnBootMenu displayConnectOnBootMenu;
public:
	ScrollMenuItem configDisplayMenuItem;
public:
	ConfigDisplayMenu(ConfigStore& configStore, Display& display, lv_indev_t* indev, ButtonLabelBar* buttonLabel = NULL) :
		bikeNavigationMenu("Display", "Back", indev, buttonLabel),
		configDisplayMenuItem("Display"),
        displayBrightnessMenu(configStore, display, indev, buttonLabel),
        displayConnectOnBootMenu(configStore, display, indev, buttonLabel),
        displayConnectBatteryOnlyMenu(configStore, display, indev, buttonLabel)
	{        
        bikeNavigationMenu.addMenuItem(&displayConnectOnBootMenu.menuItem);
        bikeNavigationMenu.addMenuItem(&displayBrightnessMenu.menuItem);
        bikeNavigationMenu.addMenuItem(&displayConnectBatteryOnlyMenu.menuItem);

        // Configure the menu item
        configDisplayMenuItem.setPopupItem(&bikeNavigationMenu);
	}
};

#endif