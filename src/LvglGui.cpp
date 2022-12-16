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
#include "gui/monitors/main/SpeedGraphMonitorMain.h"
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
#include "stats/SpeedMeterLogger.h"

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

    //
    // Configure the Display
    //
    lv_disp_t* lv_display = displayGlue.getLvDisplay();
    static Display display(displayGlue.display);
    display.setContrast(configStore->getDisplayConfig().contrast);

    //
    // Monitor Configurations
    //
    static MainView mainView(indev);
    ButtonLabelBar* buttonLabelBar = mainView.getButtonLabelBar();    

    // Monitor Screen 1 (Battery and Assist)
    static MotorAssistLevelDotMonitorSmall motorAssistLevelDotSmall;
    static BatteryCapacityMonitorMain batteryMonitor;
    static MainSmallMonitorLayout mainSmallMonitorLayout(&batteryMonitor, &motorAssistLevelDotSmall);

    // Monitor Screen 2 (Power Average/Min Max)
    bool minMaxPower = false;
    static PowerMeterLogger powerMeterLogger;
    static RiderPowerMonitorSmall riderPowerSmallTop;
    static RiderPowerGraphMonitorMain riderPowerGraphMain(&powerMeterLogger, minMaxPower);
    static MainSmallMonitorLayout mainPower(&riderPowerGraphMain, &riderPowerSmallTop);

    // Monitor Screen 3 (Speed Average/Min Max)
    bool minMaxSpeed = true;
    static SpeedMeterLogger speedMeterLogger;
    static SpeedMonitorSmall speedSmallTop(configStore);
    static SpeedGraphMonitorMain speedGraphMonitorMain(configStore, &speedMeterLogger, minMaxSpeed);
    static MainSmallMonitorLayout mainSpeed(&speedGraphMonitorMain, &speedSmallTop);

    // Monitor Screen 4 (All the odds and ends
    static TimeticksMonitorSmall timeticksMonitorSmall(NULL);        
    static CadenceMonitorSmall cadenceMonitorSmall;
    static SpeedMonitorSmall speedMonitorSmall(configStore);
    static MotorPowerMonitorSmall motorPowerMonitorSmall;
    static BatteryVoltageMonitorSmall batteryVoltageMonitorSmall;
    static BatteryCurrentMonitorSmall batteryCurrentMonitorSmall;    
    static MotorAssistLevelMonitorSmall motorAssistLevelMonitorSmall;
    static BlankMonitorSmall blankSmall;
    //static RiderPowerMonitorSmall riderPowerMonitorSmall;    
    //static SpeedMonitorSmall speedMonitorSmall(configStore);
    //static BatteryCapacityMonitorSmall batteryCapacityMonitorSmall;
    //static MemoryMonitorSmall memoryMonitorSmall;

    static MultiSmallMonitorLayout multiSmallMonitorLayout(&timeticksMonitorSmall, &blankSmall, &cadenceMonitorSmall, &speedMonitorSmall, &motorPowerMonitorSmall, &batteryVoltageMonitorSmall, &batteryCurrentMonitorSmall, &motorAssistLevelMonitorSmall);

    //
    // Connection Menu Item
    //

    static MonitorSelector monitorSelector(indev, buttonLabelBar);
    monitorSelector.addMonitorLvObject(&mainSmallMonitorLayout);
    monitorSelector.addMonitorLvObject(&mainPower);
    monitorSelector.addMonitorLvObject(&mainSpeed);
    monitorSelector.addMonitorLvObject(&multiSmallMonitorLayout);

    static BluetoothConnection bluetoothConnection(bluetoothBikeController, configStore, &pressbutton, indev, buttonLabelBar);
    bluetoothConnection.setMonitorSelector(&monitorSelector);
    static ScrollMenuItem connectMenuItem(&pressbutton, false);
    connectMenuItem.setPopupItem(&bluetoothConnection);

    //
    // Settings Menu Item
    //
    static ConfigMainMenu configMainMenu(*configStore, display, indev, buttonLabelBar);
    
    //
    // Bluetooth Menu Item
    //
    static BluetoothScanList bluetoothScanList(bluetoothBikeController, configStore, indev, buttonLabelBar);
    static ScrollMenuItem bluetoothMenuItem(&bluetooth);    
    bluetoothMenuItem.setPopupItem(&bluetoothScanList);

    //
    // Main Menu Creations
    //
    
    lv_obj_t* screen_obj = lv_scr_act();
    static ScrollMenu mainScrollMenu(indev, buttonLabelBar);
    mainScrollMenu.addMenuItem(&connectMenuItem);
    mainScrollMenu.addMenuItem(&configMainMenu.configMainMenuItem);
    mainScrollMenu.addMenuItem(&bluetoothMenuItem);

    //
    // Complete configuration of mainView
    //

    mainView.setBaseLvObject(&mainScrollMenu);
    mainView.createLvObj(screen_obj);
    mainView.focusLvObj();


    //
    // If connect on boot and there is at least one address to connect to
    //
    if (configStore->getDisplayConfig().connectOnBoot && configStore->getBTAddressesConfig().countBTAddresses() > 0) {
        mainScrollMenu.selectScrollMenuItem(&connectMenuItem);
    }
}