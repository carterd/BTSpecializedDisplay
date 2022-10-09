#ifndef BLUETOOTH_CONNECTION_H
#define BLUETOOTH_CONNECTION_H

#include <lvgl.h>

#include "BaseLvObject.h"
#include "MonitorLvObject.h"
#include "ButtonLabel.h"
#include "..\dev\BluetoothBikeController.h"
#include "..\dev\ConfigStore.h"

#define CONNECT_ONLY_WHEN_BATTERY_CONNECTED 0
#define SEARCHING_STRING "Searching"

/// <summary>
/// The connection shows an image with a spinner attempts to make a connection to any connecting 
/// </summary>
class BluetoothConnection : public BaseLvObject
{
private:
    /// <summary>
    /// This is the LV background image
    /// </summary>
    lv_img_dsc_t* image;
    /// <summary>
    /// The indev for the encoder to give buttons context for exit
    /// </summary>
    lv_indev_t* indev;
    /// <summary>
    /// The group for the encoder to give buttons context for exit
    /// </summary>
    lv_group_t* group;

    lv_obj_t* tileview_obj;
    /// <summary>
    /// The connection button
    /// </summary>
    lv_obj_t* button_obj;
    /// <summary>
    /// The connection label
    /// </summary>
    lv_obj_t* label_obj;

    lv_obj_t* spinner_obj;

    /// <summary>
    /// This is the connection screen tile
    /// </summary>
    lv_obj_t* connection_tile_obj;

    /// <summary>
    /// This is the button label or NULL for not button label
    /// </summary>
    ButtonLabel* buttonLabel = NULL;
    /// <summary>
    /// This is the bluetooth controller object
    /// </summary>
    BluetoothBikeController* bluetoothController;
    /// <summary>
    /// The config store which contains the bluetooth devices to connect to
    /// </summary>
    ConfigStore* configStore;


    bool connecting;

    bool connected;

    bool monitorLvObjActive;

    bool exiting;

    /// <summary>
    /// This is the object given focus on this object loosing focus
    /// </summary>
    BaseLvObject* defocusLvObj;

    MonitorLvObject* monitorLvObj;

private:

    void switchToMonitorLvObject();

    void switchToConnectionLvObject();

    /// <summary>
    /// Call on hiding the button label if one has been defined for this gui object
    /// </summary>
    void hideButtonLabels();
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

public:
    /// <summary>
    /// The construct of for the bluetooth connection object
    /// </summary>
    /// <param name="bluetoothMaster">an instance of the bluetooth controller</param>
    /// <param name="configStore">an instance of the config store for devices to connect to</param>
    /// <param name="configStore">an image to be the background image while connecting</param>
    BluetoothConnection(BluetoothBikeController* bluetoothMaster, ConfigStore* configStore, lv_img_dsc_t* image, lv_indev_t* indev);

    /// <summary>
    /// Returns the LV object instance to represent this class instance
    /// </summary>
    /// <returns>The LV object instance to represent this class instance</returns>
    virtual lv_obj_t* createLvObj(lv_obj_t* parent);

    /// <summary>
    /// This means the object and any sub objects should set any groups to be in focus at this point
    /// </summary>
    virtual void focusLvObj(BaseLvObject* defocusLvObj = NULL);

    /// <summary>
    /// Used to set the button label for this object or NULL for no button label
    /// </summary>
    /// <param name="buttonLabel">The button label object for this gui object</param>
    void setButtonLabel(ButtonLabel* buttonLabel) { this->buttonLabel = buttonLabel; };

    /// <summary>
    /// Set the given bluetooth monitor to the connection whereby on connection control passed to the monitor
    /// </summary>
    /// <param name="scrollMenuItem">The menu item to add to the list</param>
    void setMonitor(MonitorLvObject* monitorLvObj) { this->monitorLvObj = monitorLvObj; };

    /// <summary>
    /// This is called because on getting focus the refresh callback is registered with the bluetooth controller
    /// </summary>
    /// <param name="event">The event object for refresh being called into by the scanning</param>
    void refreshCB(lv_event_t* event);

    /// <summary>
    /// This is called when the user presses the exit button for the connection
    /// </summary>
    /// <param name="event">The event object for refresh being called into by the scanning</param>
    void exitButtonCB(lv_event_t* event);

    /// <summary>
    /// This is called once a tile have moved it's position
    /// </summary>
    /// <param name="event"></param>
    void tileChangedhCB(lv_event_t* event);
public:

    static void refresh_cb(lv_event_t* event);

    static void exit_btn_cb(lv_event_t* event);

    static void tile_changed_cb(lv_event_t* event);
};

#endif