#ifndef _BIKE_WHEEL_CIRCUMFERENCE_MENU_H
#define _BIKE_WHEEL_CIRCUMFERENCE_MENU_H

#include "../ScrollMenuItem.h"
#include "../IntegerSelectMenu.h"
#include "../BikeConfigIntegerSelectMenu.h"

#include "../../dev/ConfigStore.h"

class BikeWheelCircumferenceMenu {
private:
	BikeConfigIntegerSelectMenu integerSelectMenu;
public:
	ScrollMenuItem menuItem;
public:
	BikeWheelCircumferenceMenu(ConfigStore& configStore, lv_indev_t* indev, ButtonLabel* buttonLabel = NULL) :
		integerSelectMenu("Wheel Circumference", "Back", configStore, BikeConfigAttributeIndex::WheelCircumference, true, indev, buttonLabel),
		menuItem("Wheel Circumference") 
	{
		this->integerSelectMenu.addMenuItem("Unmanaged", UNMANAGED_CONFIG);
        this->integerSelectMenu.addMenuItem("Default\" (2293mm)", 2293);
        this->integerSelectMenu.addMenuItem("29 x 3.2\" (2462mm)",   2462);
        this->integerSelectMenu.addMenuItem("29 x 3.0\" (2430mm)",   2430);
        this->integerSelectMenu.addMenuItem("29 x 2.8\" (2398mm)",   2398);
        this->integerSelectMenu.addMenuItem("29 x 2.7\" (2382mm)",   2382);
        this->integerSelectMenu.addMenuItem("29 x 2.6\" (2366mm)",   2366);
        this->integerSelectMenu.addMenuItem("29 x 2.5\" (2350mm)",   2350);
        this->integerSelectMenu.addMenuItem("27.5 x 3.2\" (2342mm)", 2342);
        this->integerSelectMenu.addMenuItem("29 x 2.4\" (2333mm)",   2333);
        this->integerSelectMenu.addMenuItem("29 x 2.35\" (2326mm)",  2326);
        this->integerSelectMenu.addMenuItem("29 x 2.3\" (2318mm)",   2318);
        this->integerSelectMenu.addMenuItem("27.5 x 3.0\" (2311mm)", 2311);
        this->integerSelectMenu.addMenuItem("29 x 2.25\" (2310mm)",  2310);
        this->integerSelectMenu.addMenuItem("29 x 2.2\" (2302mm)",   2302);
        this->integerSelectMenu.addMenuItem("27.5 x 2.8\" (2279mm)", 2279);
        this->integerSelectMenu.addMenuItem("27.5 x 2.7\" (2263mm)", 2263);
        this->integerSelectMenu.addMenuItem("27.5 x 2.6\" (2247mm)", 2247);
        this->integerSelectMenu.addMenuItem("27.5 x 2.5\" (2231mm)", 2231);
        this->integerSelectMenu.addMenuItem("26 x 3.0\" (2216mm)",   2216);
        this->integerSelectMenu.addMenuItem("27.5 x 2.4\" (2213mm)", 2213);
        this->integerSelectMenu.addMenuItem("27.5 x 2.35\" (2207mm)",2207);
        this->integerSelectMenu.addMenuItem("26 x 2.8\" (2200mm)",   2200);
        this->integerSelectMenu.addMenuItem("27.5 x 2.3\" (2199mm)", 2199);
        this->integerSelectMenu.addMenuItem("27.5 x 2.25\" (2191mm)",2191);
        this->integerSelectMenu.addMenuItem("26 x 2.7\" (2184mm)",   2184);
        this->integerSelectMenu.addMenuItem("27.5 x 2.2\" (2183mm)", 2183);        
        this->integerSelectMenu.addMenuItem("26 x 2.6\" (2168mm)",   2168);
        this->integerSelectMenu.addMenuItem("26 x 2.5\" (2152mm)",   2152);
        this->integerSelectMenu.addMenuItem("26 x 2.4\" (2135mm)",   2135);
        this->integerSelectMenu.addMenuItem("26 x 2.35\" (2129mm)",  2192);
        this->integerSelectMenu.addMenuItem("26 x 2.3\" (2121mm)",   2121);
        this->integerSelectMenu.addMenuItem("26 x 2.25\" (2113mm)",  2113);
        this->integerSelectMenu.addMenuItem("26 x 2.2\" (2105mm)",   2105);
        this->integerSelectMenu.addMenuItem("26 x 2.0\" (2073mm)",   2073);
	    this->menuItem.setPopupItem(&this->integerSelectMenu);
	}
};

#endif