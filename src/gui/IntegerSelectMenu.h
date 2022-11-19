#ifndef _INTEGER_SELECT_MENU_H
#define _INTEGER_SELECT_MENU_H

#include <vector>
#include "ValueSelectMenu.h"
#include "ValueSelectMenuItem.h"

class IntegerSelectMenu : public ValueSelectMenu 
{
private:
    std::vector<ValueSelectMenuItem*> menuItems;
    void (*value_change_cb)(int);
    int (*value_init_cb)(void);
    int selectedValue;
public:
	/// <summary>
	/// Constructor of the Bluetooth scan list hence shows a list of dectected bluetooth devices/
	/// </summary>
	/// <param name="exitButtonText">The text to display for exit menu option of the menu</param>
	/// <param name="indev">The indev used for taking context of the encoder</param>
	IntegerSelectMenu(const char* titleText, const char* exitButtonText, int (*value_init_cb)(void), void (*value_change_cb)(int), lv_indev_t* indev);

    virtual ~IntegerSelectMenu();

    virtual void focusLvObj(BaseLvObject* defocusLvObj = NULL);

    void addMenuItem(const char* text, int value);

    bool isCheckedCB(int value);

    void selectedCB(int value);
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