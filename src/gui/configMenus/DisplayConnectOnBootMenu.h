#ifndef _DISPLAY_CONNECT_ON_BOOT_MENU_H
#define _DISPLAY_CONNECT_ON_BOOT_MENU_H

#include "../ScrollMenuItem.h"
#include "../IntegerSelectMenu.h"
#include "../DisplayConfigIntegerSelectMenu.h"

#include "../../dev/ConfigStore.h"

class DisplayConnectOnBootMenu {
public:
	DisplayConfigIntegerSelectMenu integerSelectMenu;
	ScrollMenuItem menuItem;
public:
	DisplayConnectOnBootMenu(ConfigStore& configStore, Display& display, lv_indev_t* indev, ButtonLabel* buttonLabel = NULL) :
		integerSelectMenu("On Start", "Back", configStore, DisplayConfigAttributeIndex::ConnectOnBoot, display, indev, buttonLabel),
		menuItem("On Start") 
	{
	    this->integerSelectMenu.addMenuItem("Menu", 0x00);
	    this->integerSelectMenu.addMenuItem("Connect", 0x01);
	    this->menuItem.setPopupItem(&this->integerSelectMenu);
	}
};

#endif