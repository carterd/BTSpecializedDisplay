#ifndef _LVGL_GUI_H_
#define _LVGL_GUI_H_

#include <Arduino_LvGL_Glue.h>
#include <lvgl.h>

#include "dev/BluetoothController.h"
#include "dev/BluetoothScanner.h"
#include "dev/BluetoothBike.h"
#include "dev/ConfigStore.h"

void lvgl_setup(ConfigStore *configStore, BluetoothController *bluetoothController, BluetoothBike* bluetoothBike, BluetoothScanner *bluetoothScanner, Arduino_LvGL_Glue& displayGlue, lv_indev_t* indev);

#endif