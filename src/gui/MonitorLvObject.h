#ifndef _MONITOR_LV_OBJECT_H
#define _MONITOR_LV_OBJECT_H

#include <lvgl.h>
#include <ArduinoBLE.h>

#include "BaseLvObject.h"
#include "../dev/BluetoothBikeController.h"
#include "../dev/BluetoothBikeDefinitions.h"
#include "../dev/BluetoothBikeState.h"

class MonitorLvObject : public BaseLvObject
{
protected:
    BluetoothBikeController* bluetoothBikeController;
public:
    MonitorLvObject();

    virtual void setBluetoothController(BluetoothBikeController* bluetoothBikeController);

    virtual void statusUpdate() = 0;
};

#endif