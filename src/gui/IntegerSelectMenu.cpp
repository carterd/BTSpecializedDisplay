#include "IntegerSelectMenu.h"

IntegerSelectMenu::IntegerSelectMenu(const char* titleText, const char* exitButtonText, int (*value_init_cb)(void), void (*value_change_cb)(int), lv_indev_t* indev)
    : ValueSelectMenu(titleText, exitButtonText, indev)
{
    this->value_change_cb = value_change_cb;
    this->value_init_cb = value_init_cb;
}

IntegerSelectMenu::~IntegerSelectMenu() {   
}

void IntegerSelectMenu::addMenuItem(const char* text, int value) {
    IntegerSelectMenuItem* item = new IntegerSelectMenuItem(text, value, this);
    this->menuItems.push_back(item);
    ValueSelectMenu::addMenuItem(item);
}

bool IntegerSelectMenu::isCheckedCB(int value) {
        return value == this->selectedValue;
}

void IntegerSelectMenu::selectedCB(int value) {
    this->selectedValue = value;
    this->value_change_cb(this->selectedValue);
}

void IntegerSelectMenu::focusLvObj(BaseLvObject* defocusLvObj) {
    this->selectedValue = this->value_init_cb();
    ValueSelectMenu::focusLvObj(defocusLvObj);
}
