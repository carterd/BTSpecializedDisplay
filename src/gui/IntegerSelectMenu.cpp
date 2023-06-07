#include "IntegerSelectMenu.h"

IntegerSelectMenu::IntegerSelectMenu(const char* titleText, const char* exitButtonText, lv_indev_t* indev, ButtonLabelBar* buttonLabel)
    : ValueSelectMenu(titleText, exitButtonText, indev, buttonLabel)
{
}

IntegerSelectMenu::~IntegerSelectMenu() {
}

void IntegerSelectMenu::deleteAllMenuItems() {    
    for (std::vector<ValueSelectMenuItem*>::iterator it = std::begin(*this->getValueMenuItems()); it != std::end(this->valueMenuItems); ++it) {
        IntegerSelectMenuItem* item = (IntegerSelectMenuItem*) (*it);
        delete(item);
    }
}

void IntegerSelectMenu::addMenuItem(const char* text, int value) {
    IntegerSelectMenuItem* item = new IntegerSelectMenuItem(text, value, this);
    ValueSelectMenu::addMenuItem(item);
}

bool IntegerSelectMenu::isCheckedCB(int value) {
        return value == this->selectedValue;
}

void IntegerSelectMenu::selectedCB(int value) {
    this->selectedValue = value;
    // Implement the change to the back-end storage and possible show effects of change
    this->valueChangeCB(this->selectedValue);
}

void IntegerSelectMenu::focusLvObj(BaseLvObject* defocusLvObj) {
    // Use the valueInitCB to initialise the data for the menu and return the current value
    this->selectedValue = this->valueInitCB();
    ValueSelectMenu::focusLvObj(defocusLvObj);
}

void IntegerSelectMenu::exitButtonCB(lv_event_t* event) {
    // Use whatever backend storage has been changed tobe updated
    this->valueFinishCB();
    ValueSelectMenu::exitButtonCB(event);
}