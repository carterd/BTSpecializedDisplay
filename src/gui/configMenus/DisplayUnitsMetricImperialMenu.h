#ifndef _DISPLAY_UNITS_METRIC_IMPERIAL_MENU_H
#define _DISPLAY_UNITS_METRIC_IMPERIAL_MENU_H

#include "../ScrollMenuItem.h"
#include "../IntegerSelectMenu.h"
#include "../DisplayConfigIntegerSelectMenu.h"

#include "../../dev/ConfigStore.h"

class DisplayUnitsMetricImperialMenu {
public:
	DisplayConfigIntegerSelectMenu integerSelectMenu;
	ScrollMenuItem menuItem;
public:
	DisplayUnitsMetricImperialMenu(ConfigStore& configStore, Display& display, lv_indev_t* indev, ButtonLabelBar* buttonLabel = NULL) :
		integerSelectMenu("Units", "Back", configStore, DisplayConfigAttributeIndex::UnitsMetricImperial, display, indev, buttonLabel),
		menuItem("Units") 
	{
	    this->integerSelectMenu.addMenuItem("Metric", 0x01);
	    this->integerSelectMenu.addMenuItem("Imperial", 0x00);
	    this->menuItem.setPopupItem(&this->integerSelectMenu);
	}
};

#endif