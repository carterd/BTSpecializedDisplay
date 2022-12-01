#ifndef _BIKE_PEAK_ASSIST_LEVELS_MENU_H
#define _BIKE_PEAK_ASSIST_LEVELS_MENU_H

#include "../ScrollMenuItem.h"
#include "../IntegerSelectMenu.h"
#include "../BikeConfigIntegerSelectMenu.h"

#include "../../dev/ConfigStore.h"

class BikePeakAssistLevelsMenu {
private:
	BikeConfigIntegerSelectMenu integerSelectMenu;
public:
	ScrollMenuItem menuItem;
public:
	BikePeakAssistLevelsMenu(const char* menuTitle, BikeConfigAttributeIndex bikeConfigAttributeIndex, ConfigStore& configStore, lv_indev_t* indev, ButtonLabel* buttonLabel = NULL) :
		integerSelectMenu(menuTitle, "Back", configStore, bikeConfigAttributeIndex, false, indev, buttonLabel),
		menuItem(menuTitle)
	{
		this->integerSelectMenu.addMenuItem("10 %", 10);
		this->integerSelectMenu.addMenuItem("15 %", 15);
		this->integerSelectMenu.addMenuItem("20 %", 20);
		this->integerSelectMenu.addMenuItem("25 %", 25);
		this->integerSelectMenu.addMenuItem("30 %", 30);
		this->integerSelectMenu.addMenuItem("35 %", 35);
		this->integerSelectMenu.addMenuItem("40 %", 40);
		this->integerSelectMenu.addMenuItem("45 %", 45);
		this->integerSelectMenu.addMenuItem("50 %", 50);
		this->integerSelectMenu.addMenuItem("55 %", 55);
		this->integerSelectMenu.addMenuItem("60 %", 60);
		this->integerSelectMenu.addMenuItem("65 %", 65);
		this->integerSelectMenu.addMenuItem("70 %", 70);
		this->integerSelectMenu.addMenuItem("75 %", 75);
		this->integerSelectMenu.addMenuItem("80 %", 80);
		this->integerSelectMenu.addMenuItem("85 %", 85);
		this->integerSelectMenu.addMenuItem("90 %", 90);
		this->integerSelectMenu.addMenuItem("95 %", 95);
		this->integerSelectMenu.addMenuItem("100 %", 100);
		this->menuItem.setPopupItem(&this->integerSelectMenu);
	}
};

#endif