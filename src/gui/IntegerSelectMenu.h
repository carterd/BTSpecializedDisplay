#ifndef _INTEGER_SELECT_MENU_H
#define _INTEGER_SELECT_MENU_H

#include <vector>
#include "ValueSelectMenu.h"
#include "ValueSelectMenuItem.h"

class IntegerSelectMenu : public ValueSelectMenu 
{
private:
    std::vector<ValueSelectMenuItem*> menuItems;
    int selectedValue;

public:
	/// <summary>
	/// Constructor of the Bluetooth scan list hence shows a list of dectected bluetooth devices/
	/// </summary>
	/// <param name="exitButtonText">The text to display for exit menu option of the menu</param>
	/// <param name="indev">The indev used for taking context of the encoder</param>
	IntegerSelectMenu(const char* titleText, const char* exitButtonText, lv_indev_t* indev, ButtonLabel* buttonLabel = NULL);

    virtual ~IntegerSelectMenu();

	/// <summary>
	/// This means the object and any sub objects should set any groups to be in focus at this point
	/// </summary>
    virtual void focusLvObj(BaseLvObject* defocusLvObj = NULL);

	/// <summary>
	/// The callback on the exit button clicked, overloaded for possible callbacks
	/// </summary>
	/// <param name="event">The lv event that identifies pressing the exit button</param>
	virtual void exitButtonCB(lv_event_t* event);

    void addMenuItem(const char* text, int value);

    bool isCheckedCB(int value);

    void selectedCB(int value);

    virtual void valueChangeCB(int newValue) = 0;
    virtual int valueInitCB() = 0;
    virtual void valueFinishCB() = 0;
};

class IntegerSelectMenuItem : public ValueSelectMenuItem {
private:
    int value;
    IntegerSelectMenu* integerSelectMenu;
    //void (*callback)(void);
public:
    IntegerSelectMenuItem(const char* valueText, int value, IntegerSelectMenu* integerSelectMenu) : ValueSelectMenuItem(valueText) {
        this->value = value;
        this->integerSelectMenu = integerSelectMenu;
    }

    virtual bool isChecked() {
        return this->integerSelectMenu->isCheckedCB(this->value);
    }

    virtual void selectedCB() {
        this->integerSelectMenu->selectedCB(this->value);
    }
};

#endif