#include <lvgl.h>
#include <Arduino.h>

#include "LvglGui.h"

#include "gui/MainView.h"
#include "gui/ScrollMenu.h"
#include "gui/ScrollMenuItem.h"
#include "gui/IntegerSelectMenu.h"
#include "gui/ValueSelectMenu.h"
#include "gui/ValueSelectMenuItem.h"
#include "gui/ButtonLabelBar.h"
#include "gui/SavesAppender.h"
#include "gui/BluetoothScanList.h"
#include "gui/BluetoothConnection.h"
#include "gui/MonitorSelector.h"

#include "gui/monitors/layout/MainSmallMonitorLayout.h"
#include "gui/monitors/layout/MultiSmallMonitorLayout.h"
#include "gui/monitors/layout/MediumSmallmonitorLayout.h"

#include "gui/monitors/main/BatteryCapacityMonitorMain.h"
#include "gui/monitors/main/RiderPowerGraphMonitorMain.h"
#include "gui/monitors/main/SpeedGraphMonitorMain.h"

#include "gui/monitors/medium/RiderPowerMonitorMedium.h"
#include "gui/monitors/medium/SpeedMonitorMedium.h"
#include "gui/monitors/medium/CadenceMonitorMedium.h"

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
#include "gui/monitors/small/MotorOdometerMonitorSmall.h"
#include "gui/monitors/small/BatteryCapacityImageMonitorSmall.h"

#include "gui/configMenus/DisplayBrightnessMenu.h"
#include "gui/configMenus/DisplayConnectOnBootMenu.h"
#include "gui/configMenus/DisplayConnectBatteryOnlyMenu.h"
#include "gui/configMenus/BikeBeeperMenu.h"
#include "gui/configMenus/ConfigMainMenu.h"

#include "img/Ride.h"
#include "img/PressButton.h"
#include "img/Spanner.h"
#include "img/Bluetooth.h"
#include "img/Phone.h"

#include "dev/Display.h"
#include "dev/BluetoothBikeController.h"

#include "stats/PowerMeterLogger.h"
#include "stats/SpeedMeterLogger.h"

#define RIDER_POWER_GRAPH_MAXMIN false
#define SPEED_GRAPH_MAXMIN true

/**
 * @brief Set up the components of the LVGL display
 * 
 */
void lvgl_setup(ConfigStore *configStore, BluetoothBikeController *bluetoothBikeController, Arduino_LvGL_Glue& displayGlue, lv_indev_t* indev) {

    //
    // Configure the Display
    // =====================
    //
    lv_disp_t* lv_display = displayGlue.getLvDisplay();
    static Display display(displayGlue.display);
    display.setContrast(configStore->getDisplayConfig().contrast);

    static ButtonLabelBar buttonLabelBar;
    static MainView mainView(indev, &buttonLabelBar);
    static MonitorSelector monitorSelector(indev, &buttonLabelBar);

    static PowerMeterLogger powerMeterLogger;
    static SpeedMeterLogger speedMeterLogger;

    lv_obj_t* screen_obj = lv_scr_act();

    //
    // Monitor Configurations
    // ======================
    //

    // Monitor Screen 1 (Battery and Assist)
    static MotorAssistLevelDotMonitorSmall motorAssistLevelDotSmall_Screen1;
    static BatteryCapacityMonitorMain batteryMonitor_Screen1;
    static MainSmallMonitorLayout mainSmallMonitorLayout_Screen1(&batteryMonitor_Screen1, &motorAssistLevelDotSmall_Screen1);
    monitorSelector.addMonitorLvObject(&mainSmallMonitorLayout_Screen1);

    // Monitor Screen 2 (Assist, Rider Power, Speed Monitor, Battery)
    static BatteryCapacityImageMonitorSmall batteryCapacityImageMonitorSmall_Screen2;
    static RiderPowerMonitorMedium riderPowerMonitorMedium_Screen2;
    static SpeedMonitorMedium speedMonitorMedium_Screen2(configStore);
    static MotorAssistLevelDotMonitorSmall motorAssistLevelDotSmall_Screen2;
    static MediumSmallMonitorLayout mediumSmallmonitorLayout_Screen2(&motorAssistLevelDotSmall_Screen2, &riderPowerMonitorMedium_Screen2, &speedMonitorMedium_Screen2, &batteryCapacityImageMonitorSmall_Screen2);
    monitorSelector.addMonitorLvObject(&mediumSmallmonitorLayout_Screen2);

    // Monitor Screen 3 (Assist, Rider Power, Speed Monitor, Battery)
    static BatteryCapacityImageMonitorSmall batteryCapacityImageMonitorSmall_Screen3;
    static CadenceMonitorMedium cadenceMonitorMedium;
    static SpeedMonitorMedium speedMonitorMedium_Screen3(configStore);
    static MotorAssistLevelDotMonitorSmall motorAssistLevelDotSmall_Screen3;
    static MediumSmallMonitorLayout mediumSmallmonitorLayout_Screen3(&motorAssistLevelDotSmall_Screen3, &cadenceMonitorMedium, &speedMonitorMedium_Screen3, &batteryCapacityImageMonitorSmall_Screen3);
    monitorSelector.addMonitorLvObject(&mediumSmallmonitorLayout_Screen3);

    // Monitor Screen 2 (Power Average/Min Max)
    static RiderPowerMonitorSmall riderPowerSmall_Screen4;
    static RiderPowerGraphMonitorMain riderPowerGraphMain_Screen4(&powerMeterLogger, RIDER_POWER_GRAPH_MAXMIN);
    static MainSmallMonitorLayout mainSmallMonitorLayout_Screen4(&riderPowerGraphMain_Screen4, &riderPowerSmall_Screen4);
    monitorSelector.addMonitorLvObject(&mainSmallMonitorLayout_Screen4);

    // Monitor Screen 3 (Speed Average/Min Max)
    static SpeedMonitorSmall speedMonitorSmall_Screen5(configStore);
    static SpeedGraphMonitorMain speedGraphMonitorMain_Screen5(configStore, &speedMeterLogger, SPEED_GRAPH_MAXMIN);
    static MainSmallMonitorLayout mainSmallMonitorLayout_Screen5(&speedGraphMonitorMain_Screen5, &speedMonitorSmall_Screen5);
    monitorSelector.addMonitorLvObject(&mainSmallMonitorLayout_Screen5);

    // Monitor Screen 4 (All the odds and ends
    static TimeticksMonitorSmall timeticksMonitorSmall_Screen6(NULL);
    static CadenceMonitorSmall cadenceMonitorSmall_Screen6;
    static SpeedMonitorSmall speedMonitorSmall_Screen6(configStore);
    static MotorPowerMonitorSmall motorPowerMonitorSmall_Screen6;
    static BatteryVoltageMonitorSmall batteryVoltageMonitorSmall_Screen6;
    static BatteryCurrentMonitorSmall batteryCurrentMonitorSmall_Screen6;
    static MotorAssistLevelMonitorSmall motorAssistLevelMonitorSmall_Screen6;
    static BlankMonitorSmall blankSmall_Screen6;
    static MotorOdometerMonitorSmall motorOdometerMonitorSmall_Screen6(configStore);
    //static BatteryCapacityImageMonitorSmall batteryCapacityImageMonitorSmall;
    //static RiderPowerMonitorSmall riderPowerMonitorSmall;    
    //static SpeedMonitorSmall speedMonitorSmall(configStore);
    //static BatteryCapacityMonitorSmall batteryCapacityMonitorSmall;
    static MemoryMonitorSmall memoryMonitorSmall("mem");
    static MultiSmallMonitorLayout multiSmallMonitorLayout_Screen6(&timeticksMonitorSmall_Screen6, &memoryMonitorSmall, &cadenceMonitorSmall_Screen6, &speedMonitorSmall_Screen6, &motorPowerMonitorSmall_Screen6, &batteryVoltageMonitorSmall_Screen6, &batteryCurrentMonitorSmall_Screen6, &motorOdometerMonitorSmall_Screen6);
    monitorSelector.addMonitorLvObject(&multiSmallMonitorLayout_Screen6);

    display_theme_styles_t* display_theme_styles = (display_theme_styles_t*)lv_disp_get_theme(lv_obj_get_disp(screen_obj))->user_data;

    //
    // Connection Menu Item
    // ====================
    //

    static BluetoothConnection bluetoothConnection(bluetoothBikeController, configStore, display_theme_styles->connecting_button_img, indev, &buttonLabelBar);
    bluetoothConnection.setMonitorSelector(&monitorSelector);
    static ScrollMenuItem connectMenuItem(display_theme_styles->connect_button_img, false);
    //static ScrollMenuItem connectMenuItem(&ride_img_dsc, false);
    connectMenuItem.setPopupItem(&bluetoothConnection);

    //
    // Settings Menu Item
    // ==================
    //
    static ConfigMainMenu configMainMenu(*configStore, display, indev, &buttonLabelBar);
    static ScrollMenuItem configMenuItem(display_theme_styles->configure_button_img);
    // Configure the menuItem for th emain menu
    configMenuItem.setPopupItem(&configMainMenu.mainNavigationMenu);

    //
    // Bluetooth Menu Item
    // ===================
    //
    static BluetoothScanList bluetoothScanList(bluetoothBikeController, configStore, indev, &buttonLabelBar);
    static ScrollMenuItem bluetoothMenuItem(display_theme_styles->bluetooth_button_img);
    bluetoothMenuItem.setPopupItem(&bluetoothScanList);

    //
    // Saves Appender
    // ==============
    //
    //static SavesAppender savesAppender(configStore, &phone_img_dsc, indev, &buttonLabelBar);
    //static ScrollMenuItem savesAppenderMenuItem(&phone_img_dsc);
    //savesAppenderMenuItem.setPopupItem(&savesAppender);

    //
    // Main Menu Creations
    // ===================
    //

    static ScrollMenu mainScrollMenu(indev, &buttonLabelBar);
    mainScrollMenu.addMenuItem(&connectMenuItem);
    mainScrollMenu.addMenuItem(&configMenuItem);
    mainScrollMenu.addMenuItem(&bluetoothMenuItem);
    //mainScrollMenu.addMenuItem(&savesAppenderMenuItem);

    //
    // Complete configuration of mainView
    // ==================================
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