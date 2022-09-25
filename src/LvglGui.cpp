#include <lvgl.h>

#include <Fonts/PixelOperator8pt7b.h>
#include <LvglThemes/lv_theme_binary.h>
#include <LvglFonts/lv_font_symbols_8.h>
#include <Utilities/Adafruit_To_LvGL_Font.h>

#include "LvglGui.h"

#include "gui/ScrollMenu.h"
#include "gui/ScrollMenuItem.h"
#include "gui/ButtonLabel.h"
#include "gui/BluetoothScanList.h"
#include "gui/BluetoothConnection.h"

#include "img/PressButton.h"
#include "img/Spanner.h"
#include "img/Bluetooth.h"

#include "dev/BluetoothBikeController.h"

#include <Arduino.h>

/**
 * @brief Set up the components of the LVGL display
 * 
 */
void lvgl_setup(ConfigStore *configStore, BluetoothBikeController *bluetoothBikeController, lv_disp_t* display, lv_indev_t* indev) {
    bluetoothBikeController->init();
    configStore->init();
    
    lv_obj_t *screen_obj = lv_scr_act();
    static ScrollMenu scrollMenu(indev);
    static ButtonLabel buttonLabel(indev);
    static ScrollMenuItem connectMenuItem(&pressbutton, false);
    static ScrollMenuItem settingsMenuItem(&spanner);
    static ScrollMenuItem bluetoothMenuItem(&bluetooth);
    scrollMenu.addMenuItem(&connectMenuItem);
    scrollMenu.addMenuItem(&settingsMenuItem);
    scrollMenu.addMenuItem(&bluetoothMenuItem);
    static BluetoothScanList bluetoothScanList(bluetoothBikeController, configStore, indev);
    bluetoothScanList.setButtonLabel(&buttonLabel);
    static BluetoothConnection bluetoothConnection(bluetoothBikeController, configStore, &pressbutton, indev);
    bluetoothConnection.setButtonLabel(&buttonLabel);

    //static BluetoothScanList bluetoothScanList(NULL, indev);
    
    
    bluetoothMenuItem.setPopupItem(&bluetoothScanList);
    connectMenuItem.setPopupItem(&bluetoothConnection);

    scrollMenu.setButtonLabel(&buttonLabel);

    scrollMenu.createLvObj(screen_obj);
    scrollMenu.focusLvObj();  
}