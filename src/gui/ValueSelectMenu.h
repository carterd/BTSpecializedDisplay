#ifndef _VALUE_SELECT_MENU_H
#define _VALUE_SELECT_MENU_H

#include <lvgl.h>

#include "ButtonLabelledLvObject.h"
#include "ValueSelectMenuItem.h"
#include "ButtonLabelBar.h"

#include <vector>
#include <unordered_map>

#define VALUE_SELECT_MENU_LABEL_HEIGHT 16

/// <summary>
/// This is the scanning list of possible bluetooth connections
/// </summary>
class ValueSelectMenu : public ButtonLabelledLvObject
{
protected:
	lv_obj_t* list_obj;
	lv_obj_t* exit_button_obj;

	const char* exitButtonText;
	const char* titleText;
	std::vector <ValueSelectMenuItem*> valueMenuItems;

	/// <summary>
	/// This is the object given focus on this object loosing focus
	/// </summary>
	BaseLvObject* defocusLvObj;

private:
	/// <summary>
	/// Displays the button label if one has been assoicated with the scan list
	/// </summary>
	void showButtonLabels();
	/// <summary>
	/// Add a button onto the list with given display text and a possible known device icon
	/// </summary>
	/// <param name="displayText">Display text in the list</param>
	/// <param name="knownDevice">True if the device is known hence shown with an icon</param>
	/// <returns>The created list item button object</returns>
	lv_obj_t* addMenuItemButton(ValueSelectMenuItem* menuItem);
	
    void updateMenuItemButton(ValueSelectMenuItem* menuItem);

    lv_obj_t* getMenuItemButton(ValueSelectMenuItem* menuItem);

protected:
	/// <summary>
	/// This will set the button labells
	/// </summary>
	virtual void updateButtonLabelBar();

public:
	/// <summary>
	/// Constructor of the Bluetooth scan list hence shows a list of dectected bluetooth devices/
	/// </summary>
	/// <param name="exitButtonText">The text to display for exit menu option of the menu</param>
	/// <param name="indev">The indev used for taking context of the encoder</param>
	ValueSelectMenu(const char* titleText, const char* exitButtonText, lv_indev_t* indev, ButtonLabelBar* buttonLabelBar = NULL);

	virtual ~ValueSelectMenu();

	/// <summary>
	/// This Creates the instance of the Lv Objects associated with the instance and sub components associated with it
	/// </summary>
	/// <param name="parent">The LV object parent on which to attach the created lv object for this instance</param>
	/// <returns>The created LV object instance</returns>
	virtual lv_obj_t* createLvObj(lv_obj_t* parent);

	/// <summary>
	/// Clean up the Lv objects for this instance, including any groups
	/// </summary>
	virtual void destroyLvObj();

	/// <summary>
	/// This means the object and any sub objects should set any groups to be in focus at this point
	/// </summary>
	virtual void focusLvObj(BaseLvObject* defocusLvObj = NULL);

	/// <summary>
	/// Ensure the LvObject menu is updated with all the required menu items have the correct ticks et-al.
	/// </summary>
    void updateLvObj();

    /// <summary>
    /// Add the given menu item objec to this list in the order called with this function
    /// </summary>
    /// <param name="scrollMenuItem">The menu item to add to the list</param>
    void addMenuItem(ValueSelectMenuItem* menuItem);

	/// <summary>
	/// The callback on the exit button clicked
	/// </summary>
	/// <param name="event">The lv event that identifies pressing the exit button</param>
	virtual void exitButtonCB(lv_event_t* event);

	/// <summary>
	/// The callback on a specific detected device clicked
	/// </summary>
	/// <param name="event">The lv event that identifies pressing the device entry</param>
	void valueItemButtonCB(lv_event_t* event);

public:
	static void exit_btn_cb(lv_event_t* event);
	static void menu_item_btn_cb(lv_event_t* event);
};

#endif