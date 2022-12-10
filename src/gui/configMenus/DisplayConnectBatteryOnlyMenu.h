#ifndef _DISPLAY_CONNECT_BATTERY_ONLY_MENU_H
#define _DISPLAY_CONNECT_BATTERY_ONLY_MENU_H

#include "../ScrollMenuItem.h"
#include "../IntegerSelectMenu.h"
#include "../DisplayConfigIntegerSelectMenu.h"

#include "../../dev/ConfigStore.h"

class DisplayConnectBatteryOnlyMenu {
public:
	DisplayConfigIntegerSelectMenu integerSelectMenu;
	ScrollMenuItem menuItem;
public:
	DisplayConnectBatteryOnlyMenu(ConfigStore& configStore, Display& display, lv_indev_t* indev, ButtonLabelBar* buttonLabel = NULL) :
		integerSelectMenu("Connection", "Back", configStore, DisplayConfigAttributeIndex::ConnectBatteryOnly, display, indev, buttonLabel),
		menuItem("Connection") 
	{
	    this->integerSelectMenu.addMenuItem("Battery Connected to Bike", 0x00);
	    this->integerSelectMenu.addMenuItem("Any Battery", 0x01);
	    this->menuItem.setPopupItem(&this->integerSelectMenu);
	}
};

#endif