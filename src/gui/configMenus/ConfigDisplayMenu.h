#ifndef _CONFIG_DISPLAY_MENU_H
#define _CONFIG_DISPLAY_MENU_H

#include "../ScrollMenuItem.h"
#include "../NavigationMenu.h"
#include "../BikeConfigIntegerSelectMenu.h"

#include "../../dev/ConfigStore.h"

#include "DisplayBrightnessMenu.h"
#include "DisplayConnectBatteryOnlyMenu.h"
#include "DisplayConnectOnBootMenu.h"
#include "DisplayUnitsMetricImperialMenu.h"
#include "DisplayAssistPresetsMenu.h"

class ConfigDisplayMenu {
private:
    NavigationMenu bikeNavigationMenu;
    DisplayBrightnessMenu displayBrightnessMenu;
    DisplayConnectBatteryOnlyMenu displayConnectBatteryOnlyMenu;
    DisplayConnectOnBootMenu displayConnectOnBootMenu;
    DisplayUnitsMetricImperialMenu displayUnitsMetricImperialMenu;
    DisplayAssistPresetsMenu displayAssistPresetsMenu;
public:
	ScrollMenuItem configDisplayMenuItem;
public:
	ConfigDisplayMenu(ConfigStore& configStore, Display& display, lv_indev_t* indev, ButtonLabelBar* buttonLabel = NULL) :
		bikeNavigationMenu("Display", "Back", indev, buttonLabel),
		configDisplayMenuItem("Display"),
        displayBrightnessMenu(configStore, display, indev, buttonLabel),
        displayConnectOnBootMenu(configStore, display, indev, buttonLabel),
        displayConnectBatteryOnlyMenu(configStore, display, indev, buttonLabel),
        displayUnitsMetricImperialMenu(configStore, display, indev, buttonLabel),
        displayAssistPresetsMenu(configStore, display, indev, buttonLabel)
	{
        bikeNavigationMenu.addMenuItem(&displayBrightnessMenu.menuItem);
        bikeNavigationMenu.addMenuItem(&displayUnitsMetricImperialMenu.menuItem);
        bikeNavigationMenu.addMenuItem(&displayConnectOnBootMenu.menuItem);
        bikeNavigationMenu.addMenuItem(&displayConnectBatteryOnlyMenu.menuItem);
        bikeNavigationMenu.addMenuItem(&displayAssistPresetsMenu.menuItem);

        // Configure the menu item
        configDisplayMenuItem.setPopupItem(&bikeNavigationMenu);
	}
};

#endif