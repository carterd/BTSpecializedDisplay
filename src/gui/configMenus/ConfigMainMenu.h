#ifndef _CONFIG_MAIN_MENU_H
#define _CONFIG_MAIN_MENU_H

#include "../../img/Spanner.h"

#include "../NavigationMenu.h"
#include "ConfigBikeMenu.h"
#include "ConfigDisplayMenu.h"

class ConfigMainMenu {
private:
    ConfigBikeMenu configBikeMenu;
    ConfigDisplayMenu configDisplayMenu;
    NavigationMenu mainNavigationMenu;
public:
    ScrollMenuItem configMainMenuItem;
public:
	ConfigMainMenu(ConfigStore& configStore, Display& display, lv_indev_t* indev, ButtonLabelBar* buttonLabel = NULL) :
        mainNavigationMenu("Config", "Exit", indev, buttonLabel),
        configMainMenuItem(&spanner_img_dsc),
        configBikeMenu(configStore, indev, buttonLabel),
        configDisplayMenu(configStore, display, indev, buttonLabel)

	{
        // Settings Root menu of just the display menu and the bike menu
        mainNavigationMenu.addMenuItem(&configDisplayMenu.configDisplayMenuItem);
        mainNavigationMenu.addMenuItem(&configBikeMenu.configBikeMenuItem);

        // Configure the menuItem for th emain menu
        configMainMenuItem.setPopupItem(&mainNavigationMenu);
	}
};

#endif