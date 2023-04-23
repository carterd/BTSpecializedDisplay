#ifndef BLUETOOTH_CONNECTION_H
#define BLUETOOTH_CONNECTION_H

#include <lvgl.h>

#include "ButtonLabelledLvObject.h"
#include "MonitorLvObject.h"
#include "ButtonLabelBar.h"
#include "../dev/BluetoothController.h"
#include "../dev/BluetoothScanner.h"
#include "../dev/BluetoothBike.h"
#include "../dev/ConfigStore.h"

#define CONNECT_ONLY_WHEN_BATTERY_CONNECTED 0
#define SEARCHING_STRING "Searching"
#define CONNECTING_TIMEOUT_MILLIS 180000

class MonitorSelector;

/// <summary>
/// The connection shows an image with a spinner attempts to make a connection to any connecting 
/// </summary>
class BluetoothConnection : public ButtonLabelledLvObject
{
private:
    /// <summary>
    /// This is the LV background image
    /// </summary>
    lv_img_dsc_t* image;

    /// <summary>
    /// The tile view object as this just an easy way to give scroll down animation on connection
    /// </summary>
    lv_obj_t* tileview_obj;

    /// <summary>
    /// The connection button
    /// </summary>
    lv_obj_t* button_obj;
    /// <summary>
    /// The connection label
    /// </summary>
    lv_obj_t* label_obj;
    /// <summary>
    /// The Spinner graphic to show connecting activity
    /// </summary>
    lv_obj_t* spinner_obj;

    /// <summary>
    /// This is the connection screen tile
    /// </summary>
    lv_obj_t* connection_tile_obj;

    /// <summary>
    /// This is a timer use to abort the connection if it's taking too long
    /// </summary>
    lv_timer_t* connecting_timer;

    /// <summary>
    /// This is the bluetooth controller object
    /// </summary>
    BluetoothController* bluetoothController;

    /// <summary>
    /// This is the bluetooth bike object
    /// </summary>
    BluetoothBike* bluetoothBike;

    /// <summary>
    /// This is the bluetooth scanner object
    /// </summary>
    BluetoothScanner* bluetoothScanner;

    /// <summary>
    /// The config store which contains the bluetooth devices to connect to
    /// </summary>
    ConfigStore* configStore;

    /// <summary>
    /// Not currently used simply the milli seconds since turn on when the connection was attempted
    /// </summary>
    uint32_t connectingStartTime;

    /// <summary>
    /// The state of the connection is in connecting state
    /// </summary>
    bool connecting;

    /// <summary>
    /// The state of the connection is connected state
    /// </summary>
    bool connected;

    /// <summary>
    /// Ture when the monitorSelector is the actively shown on the display
    /// </summary>
    bool monitorSelectorActive;

    /// <summary>
    /// Ture when the exit button has been pressed and hence actively exiting the connection
    /// </summary>
    bool exiting;

    /// <summary>
    /// This is the object given focus on this object loosing focus
    /// </summary>
    BaseLvObject* defocusLvObj;

    MonitorSelector* monitorSelector;

private:

    void switchToMonitorLvObject();

    void switchToConnectionLvObject();

    /// <summary>
    /// Call on hiding the button label if one has been defined for this gui object
    /// </summary>
    void hideButtonLabels();
    void showButtonLabels();
    /// <summary>
    /// This will start the process of connection to any of the BLE devices in the config store
    /// </summary>
    void startBTConnection();
    /// <summary>
    /// This will end the process of connection
    /// </summary>
    void stopBTConnection();
    /// <summary>
    /// This will start the process of connection to any of the BLE devices in the config store
    /// </summary>
    void restartBTConnection();
    /// <summary>
    /// Called when an acceptable ebike had been connected to via BLE
    /// </summary>
    void connectionSuccess();
    /// <summary>
    /// Called on refresh to check if there is a connection when attempting to connect
    /// </summary>
    void checkForConnection();
    /// <summary>
    /// Called on refresh to check if there are any updates to be displayed on connected to bike
    /// </summary>
    void checkForConnectedUpdates();

protected:
    virtual void updateButtonLabelBar();

public:
    /// <summary>
    /// The construct of for the bluetooth connection object
    /// </summary>
    /// <param name="bluetoothMaster">an instance of the bluetooth controller</param>
    /// <param name="configStore">an instance of the config store for devices to connect to</param>
    /// <param name="configStore">an image to be the background image while connecting</param>
    BluetoothConnection(BluetoothController* bluetoothController, BluetoothBike* bluetoothBike, BluetoothScanner* bluetoothScanner, ConfigStore* configStore, lv_img_dsc_t* image, lv_indev_t* indev, ButtonLabelBar* buttonLabelBar = NULL);

    virtual ~BluetoothConnection();

    /// <summary>
    /// Returns the LV object instance to represent this class instance
    /// </summary>
    /// <returns>The LV object instance to represent this class instance</returns>
    virtual lv_obj_t* createLvObj(lv_obj_t* parent);

    /// <summary>
    /// Destroy all the LV objects associated with the connection
    /// </summary>
    virtual void destroyLvObj();

    /// <summary>
    /// This means the object and any sub objects should set any groups to be in focus at this point
    /// </summary>
    virtual void focusLvObj(BaseLvObject* defocusLvObj = NULL);

    /// <summary>
    /// Set the given bluetooth monitor to the connection whereby on connection control passed to the monitor
    /// </summary>
    /// <param name="scrollMenuItem">The menu item to add to the list</param>
    void setMonitorSelector(MonitorSelector* monitorSelector);

    /// <summary>
    /// This is called because on getting focus the refresh callback is registered with the bluetooth controller
    /// </summary>
    /// <param name="event">The event object for refresh being called into by the scanning</param>
    void refreshCB(lv_event_t* event);

    /// <summary>
    /// This is called when the user presses the exit button for the connection
    /// </summary>
    /// <param name="event">The event object for refresh being called into by the scanning</param>
    void exitButtonCB(lv_event_t* event = NULL);

    /// <summary>
    /// This is called once a tile have moved it's position
    /// </summary>
    /// <param name="event"></param>
    void tileChangedCB(lv_event_t* event);

    /// <summary>
    /// This a callback on any kind of activity detected by the encoder
    /// </summary>
    /// <param name="event"></param>
    void encoderActivityCB(lv_event_t* event);

    /// <summary>
    /// This is the timeout call back for the timeout timer
    /// </summary>
    /// <param name="timer"></param>
    void connectingTimerCB(lv_timer_t* timer);    
public:

    static void refresh_cb(lv_event_t* event);

    static void exit_btn_cb(lv_event_t* event);

    static void lv_event_encorder_cb(lv_event_t* event);

    static void tile_changed_cb(lv_event_t* event);

    static void connecting_timer_cb(lv_timer_t * timer);
};

#endif