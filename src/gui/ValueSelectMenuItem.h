#ifndef VALUE_SELECT_MENU_ITEM_H
#define VALUE_SELECT_MENU_ITEM_H

#include <lvgl.h>

#include <Arduino.h>
#include "BaseLvObject.h"

class ValueSelectMenuItem
{
protected:
    String valueText;

public:
    ValueSelectMenuItem(const char* valueText) {
        this->valueText = valueText;
    }

    const char* getItemText() {
        return this->valueText.c_str();
    }

    virtual bool isChecked() {
        return false;
    }

    virtual void selectedCB() {
    }
};

#endif