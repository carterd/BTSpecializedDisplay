#ifndef _BIKE_ASSIST_LEVELS_MENU_H
#define _BIKE_ASSIST_LEVELS_MENU_H

#include "../ScrollMenuItem.h"
#include "../IntegerSelectMenu.h"
#include "../BikeConfigIntegerSelectMenu.h"

#include "../../dev/ConfigStore.h"

class BikeAssistLevelsMenu {
private:
	BikeConfigIntegerSelectMenu integerSelectMenu;
public:
	ScrollMenuItem menuItem;
public:
	BikeAssistLevelsMenu(const char* menuTitle, BikeConfigAttributeIndex bikeConfigAttributeIndex, ConfigStore & configStore, lv_indev_t* indev, ButtonLabel* buttonLabel = NULL) :
		integerSelectMenu(menuTitle, "Back", configStore, bikeConfigAttributeIndex, false, indev, buttonLabel),
		menuItem(menuTitle)
	{
		this->integerSelectMenu.addMenuItem("0 %", 0);
		this->integerSelectMenu.addMenuItem("10 %", 10);
		this->integerSelectMenu.addMenuItem("20 %", 20);
		this->integerSelectMenu.addMenuItem("30 %", 30);
		this->integerSelectMenu.addMenuItem("40 %", 40);
		this->integerSelectMenu.addMenuItem("50 %", 50);
		this->integerSelectMenu.addMenuItem("60 %", 60);
		this->integerSelectMenu.addMenuItem("70 %", 70);
		this->integerSelectMenu.addMenuItem("80 %", 80);
		this->integerSelectMenu.addMenuItem("90 %", 90);
		this->integerSelectMenu.addMenuItem("100 %", 100);
		this->menuItem.setPopupItem(&this->integerSelectMenu);
	}
};

#endif