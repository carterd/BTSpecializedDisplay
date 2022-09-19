#include <lvgl.h>

#include <Fonts/PixelOperator8pt7b.h>
#include <LvglThemes/lv_theme_binary.h>
#include <LvglFonts/lv_font_symbols_8.h>
#include <Utilities/Adafruit_To_LvGL_Font.h>

#include "LvglGui.h"

#include "ScrollMenu.h"
#include "ScrollMenuItem.h"
#include "ButtonLabel.h"
#include "BluetoothScanList.h"
#include "BluetoothConnection.h"

#include "PressButton.h"
#include "Spanner.h"
#include "Bluetooth.h"

#include <Arduino.h>

/**
 * @brief Set up the components of the LVGL display
 * 
 */
void lvgl_setup(ConfigStore *configStore, BluetoothMaster *bluetoothMaster, lv_disp_t* display, lv_indev_t* indev) {
    bluetoothMaster->init();
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
    static BluetoothScanList bluetoothScanList(bluetoothMaster, configStore, indev);
    bluetoothScanList.setButtonLabel(&buttonLabel);
    static BluetoothConnection bluetoothConnection(bluetoothMaster, configStore, &pressbutton);
    bluetoothConnection.setButtonLabel(&buttonLabel);

    //static BluetoothScanList bluetoothScanList(NULL, indev);
    
    
    bluetoothMenuItem.setPopupItem(&bluetoothScanList);
    connectMenuItem.setPopupItem(&bluetoothConnection);

    scrollMenu.setButtonLabel(&buttonLabel);

    scrollMenu.createLvObj(screen_obj);
    scrollMenu.focusLvObj();  
}