#include <lvgl.h>

#include <Fonts/PixelOperator8pt7b.h>
#include <LvglThemes/lv_theme_binary.h>
#include <LvglFonts/lv_font_symbols_8.h>
#include <Utilities/Adafruit_To_LvGL_Font.h>
#include <Adafruit_LvGL_Glue.h>

#include "LvglGui.h"

#include "gui/ScrollMenu.h"
#include "gui/ScrollMenuItem.h"
#include "gui/IntegerSelectMenu.h"
#include "gui/ValueSelectMenu.h"
#include "gui/ValueSelectMenuItem.h"
#include "gui/ButtonLabel.h"
//#include "gui/NavigationMenu.h"
#include "gui/BluetoothScanList.h"
#include "gui/BluetoothConnection.h"
#include "gui/monitors/BatteryMonitorMain.h"
#include "gui/monitors/AssistMonitorSmall.h"
#include "gui/monitors/MainSmallMonitorLayout.h"
#include "gui/monitors/TimeticksMonitorSmall.h"
#include "gui/monitors/CrankRotationsMonitorSmall.h"

#include "gui/configMenus/DisplayBrightnessMenu.h"
#include "gui/configMenus/DisplayConnectOnBootMenu.h"
#include "gui/configMenus/DisplayConnectBatteryOnlyMenu.h"
#include "gui/configMenus/BikeBeeperMenu.h"

#include "gui/configMenus/ConfigMainMenu.h"

#include "img/PressButton.h"
#include "img/Spanner.h"
#include "img/Bluetooth.h"

#include "dev/Display.h"
#include "dev/BluetoothBikeController.h"

#include <Arduino.h>

void setDisplayContrast();

static ConfigStore* configStorePtr;

/*
int displayConfigInt16Init(void* userData) {
    return *((int16_t*)userData);
}

void displayConfigInt16Update(int updateValue, void* userData) {
    *(int16_t*)userData = updateValue;
}

ScrollMenuItem DisplayBrightnessMenu(ConfigStore* configStore, lv_indev_t* indev) {
    static IntegerSelectMenu brightnessConfigMenu("Brightness", "Back", IntegerSelectMenu::defaultInt16Init, IntegerSelectMenu::defaultInt16Update, indev, configStore->getDisplayConfig(). )
    , brightnessConfigMenuItem("Brightness")
{
    this->brightnessConfigMenu.addMenuItem("Low", 0x01);
    this->brightnessConfigMenu.addMenuItem("Med", 0x14);
    this->brightnessConfigMenu.addMenuItem("High", 0x2F);

    this->brightnessConfigMenuItem.setPopupItem(&this->brightnessConfigMenu);
}
*/

/**
 * @brief Set up the components of the LVGL display
 * 
 */
void lvgl_setup(ConfigStore *configStore, BluetoothBikeController *bluetoothBikeController, Adafruit_LvGL_Glue& displayGlue, lv_indev_t* indev) {

    // Configure the Display
    lv_disp_t* lv_display = displayGlue.getLvDisplay();
    static Display display(displayGlue.display);
    display.setContrast(configStore->getDisplayConfig().contrast);

    static ButtonLabel buttonLabel(indev);



    // 
    // Connect Menu Item
    //
    static BluetoothConnection bluetoothConnection(bluetoothBikeController, configStore, &pressbutton, indev);

    static CrankRotationsMonitorSmall crankRotationsMonitorSmall;
    static TimeticksMonitorSmall timeticksMonitor;
    static BatteryMonitorMain batteryMonitor;
    static AssistMonitorSmall assistMonitorSmall;
    static MainSmallMonitorLayout mainSmallMonitorLayout(&batteryMonitor, &assistMonitorSmall);
    // Create the connection
    bluetoothConnection.setMonitor(&mainSmallMonitorLayout);
    bluetoothConnection.setButtonLabel(&buttonLabel);
    // Create the menu item popup being the connection
    static ScrollMenuItem connectMenuItem(&pressbutton, false);
    connectMenuItem.setPopupItem(&bluetoothConnection);


    //
    // Settings Menu Item
    //
    static ConfigMainMenu configMainMenu(*configStore, display, indev, &buttonLabel);
    
    //
    // Bluetooth Menu Item
    //
    static BluetoothScanList bluetoothScanList(bluetoothBikeController, configStore, indev);
    bluetoothScanList.setButtonLabel(&buttonLabel);
    static ScrollMenuItem bluetoothMenuItem(&bluetooth);    
    bluetoothMenuItem.setPopupItem(&bluetoothScanList);    

    //
    // Main Menu
    //
    static ScrollMenu mainMenu(indev, &buttonLabel);
    mainMenu.addMenuItem(&connectMenuItem);
    mainMenu.addMenuItem(&configMainMenu.configMainMenuItem);
    mainMenu.addMenuItem(&bluetoothMenuItem);

    lv_obj_t *screen_obj = lv_scr_act();
    mainMenu.createLvObj(screen_obj);
    mainMenu.focusLvObj();

    //
    // If connect on boot and there is at least one address to connect to
    //
    if (configStore->getDisplayConfig().connectOnBoot && configStore->countBTAddresses() > 0) {
        mainMenu.selectScrollMenuItem(&connectMenuItem);
    }
}