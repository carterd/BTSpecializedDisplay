#ifndef _DISPLAY_ASSIST_PRESETS_MENU_H
#define _DISPLAY_ASSIST_PRESETS_MENU_H

#include "DisplayAssistPresetsStoreMenu.h"
#include "../ScrollMenuItem.h"
#include "../NavigationMenu.h"

#include "../../dev/ConfigStore.h"

class DisplayAssistPresetsMenu {
public:
	NavigationMenu assistPresestsNavigationMenu;
	ScrollMenuItem menuItem;
	DisplayAssistPresetsStoreMenu displayAssistPresetsStoreMenu;
	DisplayAssistPresetsDeleteMenu displayAssistPresetsDeleteMenu;
public:
	DisplayAssistPresetsMenu(ConfigStore& configStore, Display& display, lv_indev_t* indev, ButtonLabelBar* buttonLabel = NULL) :
		assistPresestsNavigationMenu("Presets", "Back", indev, buttonLabel),
		menuItem("Assist Presets"),
		displayAssistPresetsStoreMenu(configStore, indev, buttonLabel),
		displayAssistPresetsDeleteMenu(configStore, indev, buttonLabel)
	{
		this->assistPresestsNavigationMenu.addMenuItem(&displayAssistPresetsStoreMenu.menuItem);
		this->assistPresestsNavigationMenu.addMenuItem(&displayAssistPresetsDeleteMenu.menuItem);
        // Configure the menu item
        menuItem.setPopupItem(&assistPresestsNavigationMenu);
	}
};

#endif