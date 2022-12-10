#ifndef _BIKE_ASSIST_MENU_H
#define _BIKE_ASSIST_MENU_H

#include "../NavigationMenu.h"
#include "../BikeConfigIntegerSelectMenu.h"
#include "../ScrollMenuItem.h"

#include "../../dev/ConfigStore.h"

#include "BikeBeeperMenu.h"
#include "BikeWheelCircumferenceMenu.h"
#include "BikeEcoAssistMenu.h"
#include "BikeTrailAssistMenu.h"
#include "BikeTurboAssistMenu.h"


class ManagedBikeConfigMenuItem : public ScrollMenuItem {
private:
	ConfigStore& configStore;
	BikeConfig bikeConfig;
	BikeConfigAttributeIndex bikeConfigAttributeIndex;
public:
	ManagedBikeConfigMenuItem(const char* itemText, ConfigStore& configStore, BikeConfigAttributeIndex bikeConfigAttributeIndex) :
		ScrollMenuItem(itemText),
		configStore(configStore) {
		this->bikeConfigAttributeIndex = bikeConfigAttributeIndex;
	}
	/// <summary>
	/// It maybe possible for text menu items to be checkable this returns the current checked state of the menuItem
	/// </summary>
	virtual bool isChecked() {
		return this->bikeConfig.getManagedValue(this->bikeConfigAttributeIndex);
	}
	/// <summary>
	/// This function will be called by navigation menu if there is no popup item for this menu item to perform an action rather navigation.
	/// </summary>
	virtual void noPopupAction() {
		this->bikeConfig.setManagedValue(this->bikeConfigAttributeIndex, !this->bikeConfig.getManagedValue(this->bikeConfigAttributeIndex));
	}
	/// <summary>
	/// On the menu which contains this item coming into focus lets obtain the latest bike config so management status is upto date
	/// </summary>
	virtual void menuItemInitCB() {
		this->bikeConfig = this->configStore.getBikeConfig();
	}
	/// <summary>
	/// When this menu is closed we need to store the management settings contained in the navigation menu into the configstore
	/// </summary>
	virtual void menuItemFinishedCB() {
		bool managed = this->bikeConfig.getManagedValue(this->bikeConfigAttributeIndex);
		// Only update the managed state
		this->bikeConfig = this->configStore.getBikeConfig();
		switch (this->bikeConfigAttributeIndex) {
			// For the moment tie the Peak and Assit management together
		case BikeConfigAttributeIndex::SupportAssistLevelsManaged:
		case BikeConfigAttributeIndex::PeakPowerAssistLevelsManaged:
			this->bikeConfig.setManagedValue(BikeConfigAttributeIndex::SupportAssistLevelsManaged, managed);
			this->bikeConfig.setManagedValue(BikeConfigAttributeIndex::PeakPowerAssistLevelsManaged, managed);
		default:
			this->bikeConfig.setManagedValue(this->bikeConfigAttributeIndex, managed);
		}		
		this->configStore.updateBikeConfig(this->bikeConfig);
	}
};

class ConfigNavigationMenu : public NavigationMenu {
public:
	/// <summary>
	/// The callback on the exit button clicked
	/// </summary>
	/// <param name="event">The lv event that identifies pressing the exit button</param>
	virtual void exitButtonCB(lv_event_t* event) {
		// Need to at this point ensure the menu items are written into config
		NavigationMenu::exitButtonCB(event);
	}
};

class BikeAssistMenu {
private:
	NavigationMenu assistNavigationMenu;
	ManagedBikeConfigMenuItem managedBikeConfigMenuItem;
	BikeEcoAssistMenu bikeEcoAssitMenu;
	BikeTrailAssistMenu bikeTrailAssitMenu;
	BikeTurboAssistMenu bikeTurboAssitMenu;
public:
	ScrollMenuItem menuItem;
public:
	BikeAssistMenu(ConfigStore& configStore, lv_indev_t* indev, ButtonLabelBar* buttonLabelBar = NULL) :
		assistNavigationMenu("Assit Levels", "Back", indev, buttonLabelBar),
		menuItem("Assit Levels"),
		managedBikeConfigMenuItem("Managed", configStore, BikeConfigAttributeIndex::SupportAssistLevelsManaged),
		bikeEcoAssitMenu(configStore, indev, buttonLabelBar),
		bikeTrailAssitMenu(configStore, indev, buttonLabelBar),
		bikeTurboAssitMenu(configStore, indev, buttonLabelBar)
	{
        // Bike Menus
		this->assistNavigationMenu.addMenuItem(&this->managedBikeConfigMenuItem);
		this->assistNavigationMenu.addMenuItem(&this->bikeEcoAssitMenu.configBikeMenuItem);
		this->assistNavigationMenu.addMenuItem(&this->bikeTrailAssitMenu.configBikeMenuItem);
		this->assistNavigationMenu.addMenuItem(&this->bikeTurboAssitMenu.configBikeMenuItem);

		// configure the menu item
        this->menuItem.setPopupItem(&assistNavigationMenu);
	} 
};

#endif