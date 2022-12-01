#ifndef _DISPLAY_BRIGHTNESS_MENU_H
#define _DISPLAY_BRIGHTNESS_MENU_H

#include "../ScrollMenuItem.h"
#include "../IntegerSelectMenu.h"
#include "../DisplayConfigIntegerSelectMenu.h"

#include "../../dev/ConfigStore.h"

class DisplayBrightnessMenu {
public:
	DisplayConfigIntegerSelectMenu integerSelectMenu;
	ScrollMenuItem menuItem;
public:
	DisplayBrightnessMenu(ConfigStore& configStore, Display& display, lv_indev_t* indev, ButtonLabel* buttonLabel = NULL) :
		integerSelectMenu("Brightness", "Back", configStore, DisplayConfigAttributeIndex::Contrast, display, indev, buttonLabel),
		menuItem("Brightness") 
	{
		
	    this->integerSelectMenu.addMenuItem("Low", 0x01);
	    this->integerSelectMenu.addMenuItem("Med", 0x14);
    	this->integerSelectMenu.addMenuItem("High", 0x2F);
	    this->menuItem.setPopupItem(&this->integerSelectMenu);
	}
};

#endif