#include <lvgl.h>

#include <Fonts/PixelOperator8pt7b.h>
#include <LvglThemes/lv_theme_binary.h>
#include <LvglFonts/lv_font_symbols_8.h>
#include <Utilities/Adafruit_To_LvGL_Font.h>
#include <Adafruit_LvGL_Glue.h>

#include "LvglGui.h"

#include "gui/MainView.h"
#include "gui/ScrollMenu.h"
#include "gui/ScrollMenuItem.h"
#include "gui/IntegerSelectMenu.h"
#include "gui/ValueSelectMenu.h"
#include "gui/ValueSelectMenuItem.h"
#include "gui/ButtonLabelBar.h"

#include "gui/BluetoothScanList.h"
#include "gui/BluetoothConnection.h"
#include "gui/MonitorSelector.h"
#include "gui/monitors/main/BatteryCapacityMonitorMain.h"
#include "gui/monitors/main/RiderPowerGraphMonitorMain.h"
#include "gui/monitors/layout/MainSmallMonitorLayout.h"
#include "gui/monitors/layout/MultiSmallMonitorLayout.h"
#include "gui/monitors/small/MotorAssistLevelDotsMonitorSmall.h"
#include "gui/monitors/small/MotorAssistLevelMonitorSmall.h"
#include "gui/monitors/small/TimeticksMonitorSmall.h"
#include "gui/monitors/small/CadenceMonitorSmall.h"
#include "gui/monitors/small/SpeedMonitorSmall.h"
#include "gui/monitors/small/BatteryCapacityMonitorSmall.h"
#include "gui/monitors/small/RiderPowerMonitorSmall.h"
#include "gui/monitors/small/MotorPowerMonitorSmall.h"
#include "gui/monitors/small/BlankMonitorSmall.h"
#include "gui/monitors/small/BatteryVoltageMonitorSmall.h"
#include "gui/monitors/small/BatteryCurrentMonitorSmall.h"
#include "gui/monitors/small/MemoryMonitorSmall.h"

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

#include "stats/PowerMeterLogger.h"

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

    
    static MainView mainView(indev);
    ButtonLabelBar* buttonLabelBar = mainView.getButtonLabelBar();

    // 
    // Connect Menu Item
    //
    static BluetoothConnection bluetoothConnection(bluetoothBikeController, configStore, &pressbutton, indev, buttonLabelBar);

    static CadenceMonitorSmall cadenceMonitorSmall;
    static SpeedMonitorSmall speedMonitorSmall(configStore);
    static TimeticksMonitorSmall timeticksMonitor;
    static BatteryCapacityMonitorSmall batteryCapacitySmall;
    static RiderPowerMonitorSmall riderPowerSmall;
    static MotorPowerMonitorSmall motorPowerSmall;
    static BatteryVoltageMonitorSmall batteryVoltageSmall;
    static BatteryCurrentMonitorSmall batteryCurrentSmall;
    static MemoryMonitorSmall memoryMonitorSmall;

    static BatteryCapacityMonitorMain batteryMonitor;
    static PowerMeterLogger powerMeterLogger;
    static RiderPowerGraphMonitorMain riderPowerGraphMain(&powerMeterLogger);
    riderPowerGraphMain.setAverageMode();
    static RiderPowerMonitorSmall riderPowerSmallTop;
    static MainSmallMonitorLayout mainPower(&riderPowerGraphMain, &riderPowerSmallTop);

    static MotorAssistLevelDotMonitorSmall motorAssistLevelDotSmall;
    static MotorAssistLevelMonitorSmall motorAssistLevelSmall;
    static BlankMonitorSmall blankSmall;

    static MainSmallMonitorLayout mainSmallMonitorLayout(&batteryMonitor, &motorAssistLevelDotSmall);
    //static MultiSmallMonitorLayout multiSmallMonitorLayout(&wheelRotationsPerMinMonitorSmall, &crankRotationsPerMinMonitorSmall, &batteryCapacitySmall, &timeticksMonitor, &motorAssistLevelSmall, &riderPowerSmall, &motorPowerSmall, &blankSmall);
    static MultiSmallMonitorLayout multiSmallMonitorLayout(&timeticksMonitor, &memoryMonitorSmall, &batteryCapacitySmall, &riderPowerSmall, &batteryCurrentSmall, &batteryVoltageSmall, &cadenceMonitorSmall, &speedMonitorSmall);

    static MonitorSelector monitorSelector(indev, buttonLabelBar);
    monitorSelector.addMonitorLvObject(&mainSmallMonitorLayout);
    monitorSelector.addMonitorLvObject(&multiSmallMonitorLayout);
    monitorSelector.addMonitorLvObject(&mainPower);

    // Create the connection
    bluetoothConnection.setMonitorSelector(&monitorSelector);
    // Create the menu item popup being the connection
    static ScrollMenuItem connectMenuItem(&pressbutton, false);
    connectMenuItem.setPopupItem(&bluetoothConnection);

    //
    // Settings Menu Item
    //
    //static ConfigMainMenu configMainMenu(*configStore, display, indev, NULL);
    static ConfigMainMenu configMainMenu(*configStore, display, indev, buttonLabelBar);
    
    //
    // Bluetooth Menu Item
    //
    static BluetoothScanList bluetoothScanList(bluetoothBikeController, configStore, indev, buttonLabelBar);
    static ScrollMenuItem bluetoothMenuItem(&bluetooth);    
    bluetoothMenuItem.setPopupItem(&bluetoothScanList);

    //
    // Main Menu
    //
    
    lv_obj_t* screen_obj = lv_scr_act();
    static ScrollMenu mainScrollMenu(indev, buttonLabelBar);
    mainScrollMenu.addMenuItem(&connectMenuItem);
    mainScrollMenu.addMenuItem(&configMainMenu.configMainMenuItem);
    mainScrollMenu.addMenuItem(&bluetoothMenuItem);

    mainView.setBaseLvObject(&mainScrollMenu);
//    mainView.setBaseLvObject(&monitorSelector);
    mainView.createLvObj(screen_obj);
    mainView.focusLvObj();

//    monitorSelector.createLvObj(screen_obj);
//    monitorSelector.focusLvObj();

    //mainScrollMenu.createLvObj(screen_obj);
    //mainScrollMenu.focusLvObj();

    //
    // If connect on boot and there is at least one address to connect to
    //
    if (configStore->getDisplayConfig().connectOnBoot && configStore->getBTAddressesConfig().countBTAddresses() > 0) {
        mainScrollMenu.selectScrollMenuItem(&connectMenuItem);
    }
}