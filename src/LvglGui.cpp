#include <lvgl.h>

#include <Fonts/PixelOperator8pt7b.h>
#include <LvglThemes/lv_theme_binary.h>
#include <LvglFonts/lv_font_symbols_8.h>
#include <Utilities/Adafruit_To_LvGL_Font.h>

#include "LvglGui.h"

#include "gui/ScrollMenu.h"
#include "gui/ScrollMenuItem.h"
#include "gui/IntegerSelectMenu.h"
#include "gui/ValueSelectMenu.h"
#include "gui/ValueSelectMenuItem.h"
#include "gui/ButtonLabel.h"
#include "gui/NavigationMenu.h"
#include "gui/BluetoothScanList.h"
#include "gui/BluetoothConnection.h"
#include "gui/monitors/BatteryMonitorMain.h"
#include "gui/monitors/AssistMonitorSmall.h"
#include "gui/monitors/MainSmallMonitorLayout.h"
#include "gui/monitors/TimeticksMonitorSmall.h"
#include "gui/monitors/CrankRotationsMonitorSmall.h"

#include "img/PressButton.h"
#include "img/Spanner.h"
#include "img/Bluetooth.h"

#include "dev/BluetoothBikeController.h"

#include <Arduino.h>

void setDisplayContrast();

static ConfigStore::DisplayConfig displayConfig;
static ConfigStore* configStorePtr;

int brightnessInit() {
    displayConfig = configStorePtr->getDisplayConfig();
    return displayConfig.contrast;
}

void brightnessUpdate(int contrast) {
    LV_LOG_USER("brightnessUpdate:start");
    displayConfig.contrast = contrast;
    LV_LOG_USER("brightnessUpdate:update");
    configStorePtr->updateDisplayConfig(displayConfig);
    LV_LOG_USER("brightnessUpdate:done update");
    LV_LOG_USER("brightnessUpdate:setting done");
    setDisplayContrast();
    LV_LOG_USER("brightnessUpdate:contrast done");
}

IntegerSelectMenu& createBrightnessMenu(ConfigStore *configStore, lv_indev_t* indev) {
    configStorePtr = configStore;
    static IntegerSelectMenu brightnessConfigMenu("Brightness", "Back", brightnessInit, brightnessUpdate, indev);
    brightnessConfigMenu.addMenuItem("Low", 0x01);
    brightnessConfigMenu.addMenuItem("Med", 0x14);
    brightnessConfigMenu.addMenuItem("High", 0x2F);
    return brightnessConfigMenu;
}

/**
 * @brief Set up the components of the LVGL display
 * 
 */
void lvgl_setup(ConfigStore *configStore, BluetoothBikeController *bluetoothBikeController, lv_disp_t* display, lv_indev_t* indev) {
    
    // Brightness
    static ScrollMenuItem brightnessConfigMenuItem("Brightness");
    IntegerSelectMenu& brightnessConfigMenu = createBrightnessMenu(configStore, indev);

    brightnessConfigMenuItem.setPopupItem(&brightnessConfigMenu);

    lv_obj_t *screen_obj = lv_scr_act();
    static ScrollMenu scrollMenu(indev);
    static ButtonLabel buttonLabel(indev);
    static ScrollMenuItem connectMenuItem(&pressbutton, false);
    static ScrollMenuItem settingsMenuItem(&spanner);
    static ScrollMenuItem bluetoothMenuItem(&bluetooth);
    scrollMenu.addMenuItem(&connectMenuItem);
    scrollMenu.addMenuItem(&settingsMenuItem);
    scrollMenu.addMenuItem(&bluetoothMenuItem);

    // display config menu
    static NavigationMenu displayConfigMenu("Display", "Back", indev);
    displayConfigMenu.setButtonLabel(&buttonLabel);
    displayConfigMenu.addMenuItem(&brightnessConfigMenuItem);
    static ScrollMenuItem displayConfigMenuItem("Display");
    displayConfigMenuItem.setPopupItem(&displayConfigMenu);
    // bike config menu
    static NavigationMenu bikeConfigMenu("Bike", "Back", indev);
    bikeConfigMenu.setButtonLabel(&buttonLabel);
    static ScrollMenuItem bikeConfigMenuItem("Bike");
    bikeConfigMenuItem.setPopupItem(&bikeConfigMenu);
    // Config menu
    static NavigationMenu configMenu("Config", "Exit", indev);
    configMenu.setButtonLabel(&buttonLabel);
    configMenu.addMenuItem(&displayConfigMenuItem);
    configMenu.addMenuItem(&bikeConfigMenuItem);

    static BluetoothScanList bluetoothScanList(bluetoothBikeController, configStore, indev);
    bluetoothScanList.setButtonLabel(&buttonLabel);

    static CrankRotationsMonitorSmall crankRotationsMonitorSmall;
    static TimeticksMonitorSmall timeticksMonitor;
    static BatteryMonitorMain batteryMonitor;
    static AssistMonitorSmall assistMonitor;
    static MainSmallMonitorLayout mainSmallMonitorLayout(&batteryMonitor, &crankRotationsMonitorSmall);
    //static MainSmallMonitorLayout mainSmallMonitorLayout(&batteryMonitor, &timeticksMonitor);
    static BluetoothConnection bluetoothConnection(bluetoothBikeController, configStore, &pressbutton, indev);
    bluetoothConnection.setMonitor(&mainSmallMonitorLayout);
    bluetoothConnection.setButtonLabel(&buttonLabel);

    //static BluetoothScanList bluetoothScanList(NULL, indev);
    
    
    
    bluetoothMenuItem.setPopupItem(&bluetoothScanList);
    connectMenuItem.setPopupItem(&bluetoothConnection);
    settingsMenuItem.setPopupItem(&configMenu);

    scrollMenu.setButtonLabel(&buttonLabel);

    scrollMenu.createLvObj(screen_obj);
    scrollMenu.focusLvObj();  
}