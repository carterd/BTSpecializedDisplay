#ifndef _LVGL_GUI_H_
#define _LVGL_GUI_H_

#include <Arduino_LvGL_Glue.h>
#include <lvgl.h>

#include "dev/BluetoothBikeController.h"
#include "dev/ConfigStore.h"

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 128

void lvgl_setup(ConfigStore *configStore, BluetoothBikeController *bluetoothBikeController, Arduino_LvGL_Glue& displayGlue, lv_indev_t* indev);

#endif