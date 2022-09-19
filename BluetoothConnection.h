#ifndef BLUETOOTH_CONNECTION_H
#define BLUETOOTH_CONNECTION_H

#include <lvgl.h>

#include "BaseLvObject.h"
#include "ButtonLabel.h"
#include "BluetoothMaster.h"
#include "ConfigStore.h"

class BluetoothConnection : public BaseLvObject
{
private:
    lv_img_dsc_t* image;

    ButtonLabel* buttonLabel = NULL;
    BluetoothMaster* bluetoothMaster;
    ConfigStore* configStore;

public:
    BluetoothConnection(BluetoothMaster* bluetoothMaster, ConfigStore* configStore, lv_img_dsc_t* image);
    virtual lv_obj_t* createLvObj(lv_obj_t* parent);
    virtual void focusLvObj();
    void setButtonLabel(ButtonLabel* buttonLabel);
    void hideButtonLabels();
};

#endif