#ifndef _BIKE_BEEPER_MENU_H
#define _BIKE_BEEPER_MENU_H

#include "../ScrollMenuItem.h"
#include "../IntegerSelectMenu.h"
#include "../BikeConfigIntegerSelectMenu.h"

#include "../../dev/ConfigStore.h"

class BikeBeeperMenu {
private:
	BikeConfigIntegerSelectMenu integerSelectMenu;
public:
	ScrollMenuItem menuItem;
public:
	BikeBeeperMenu(ConfigStore& configStore, lv_indev_t* indev, ButtonLabel* buttonLabel = NULL) :
		integerSelectMenu("Beeper", "Back", configStore, BikeConfigAttributeIndex::Beeper, true, indev, buttonLabel),
		menuItem("Beeper") 
	{
		this->integerSelectMenu.addMenuItem("Unmanaged", UNMANAGED_CONFIG);
	    this->integerSelectMenu.addMenuItem("Off", 0x00);
	    this->integerSelectMenu.addMenuItem("On", 0x01);
	    this->menuItem.setPopupItem(&this->integerSelectMenu);
	}
};

#endif