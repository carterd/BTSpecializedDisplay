#ifndef _LVGL_GUI_H_
#define _LVGL_GUI_H_

#include <lvgl.h>

#include "dev/BluetoothBikeController.h"
#include "dev/ConfigStore.h"

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 128

void lvgl_setup(ConfigStore *configStore, BluetoothBikeController *bluetoothBikeController, Adafruit_LvGL_Glue& displayGlue, lv_indev_t* indev);

#endif