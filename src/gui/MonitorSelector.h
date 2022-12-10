#ifndef _MONITOR_SELECTOR_H
#define _MONITOR_SELECTOR_H

#include <lvgl.h>
#include <vector>

#include "ButtonLabelledLvObject.h"
#include "MonitorLvObject.h"
#include "ButtonLabelBar.h"

class BluetoothConnection;

class MonitorSelector : public ButtonLabelledLvObject
{
private:
    /// <summary>
    /// The tile view object containing each tile
    /// </summary>
    lv_obj_t* options_tileview_obj;

    lv_obj_t* activity_btn_obj;

    /// <summary>
    /// The menu items store
    /// </summary>
    std::vector <MonitorLvObject*> monitorLvObjects;

    /// <summary>
    /// Flag to define if button labels should be displayed when entering menu
    /// </summary>
    bool buttonLabelStartShown = false;
    /// <summary>
    /// Flag, when true the GUI is in an animation between selecting or deselecting menu item
    /// </summary>
    bool focusAnimation = false;
    /// <summary>
    /// Flag, when true the GUI is in an animation between scrolling through menu items
    /// </summary>
    bool selectAnimation = false;
    /// <summary>
    /// The colomn id of the selected scroll menu item
    /// </summary>
    int selectedItem;
    /// <summary>
    /// Flag, if true the selecting to and from the menu item is animated.
    /// </summary>
    bool selectedItemAnimated;
    /// <summary>
    /// Pointer back to bluetoothConnection object for exiting
    /// </summary>
    BluetoothConnection* bluetoothConnection;
protected:
    virtual void updateButtonLabelBar();

    void selectMonitor(MonitorLvObject* monitorLvObject);

public:
    /// <summary>
    /// Constructor for the scroll menu
    /// </summary>
    /// <param name="indev">The indev used for taking context of the encoder</param>
    MonitorSelector(lv_indev_t* indev, ButtonLabelBar* buttonLabel = NULL);

    virtual ~MonitorSelector();

    /// <summary>
    /// This Creates the instance of the Lv Objects associated with the instance and sub components associated with it
    /// </summary>
    /// <param name="parent">The LV object parent on which to attach the created lv object for this instance</param>
    /// <returns>The created LV object instance</returns>
    virtual lv_obj_t* createLvObj(lv_obj_t* parent);

    virtual void destroyLvObj();

    /// <summary>
    /// This means the object and any sub objects should set any groups to be in focus at this point
    /// </summary>
    virtual void focusLvObj(BaseLvObject* defocusLvObj = NULL);

    virtual void statusUpdate();

    /// <summary>
    /// Add the given menu item objec to this list in the order called with this function
    /// </summary>
    /// <param name="scrollMenuItem">The menu item to add to the list</param>
    void addMonitorLvObject(MonitorLvObject* monitorLvObject);

    /// <summary>
    /// This will if not already selected a menu item and shift the focus to this menu item
    /// </summary>
    /// <param name="scrollMenuItem"></param>
    void selectMonitorLvObject(MonitorLvObject* monitorLvObject);

    void setBluetoothConnection(BluetoothConnection* bluetoothConnection) { this->bluetoothConnection = bluetoothConnection; }

    /// <summary>
    /// This is used to pass the bluetooth controller to it's actual collection of monitors
    /// </summary>
    /// <param name="bluetoothBikeController"></param>
    void setBluetoothController(BluetoothBikeController* bluetoothBikeController);

    void initBluetoothStats();

    /// <summary>
    /// Callback when a tile item button is clicked
    /// </summary>
    /// <param name="event"></param>
    void tileButtonCB(lv_event_t* event);

    /// <summary>
    /// Callback when a tile item button is de-focused
    /// </summary>
    /// <param name="event"></param>
    void tileButtonDefocusCB(lv_event_t* event);

    /// <summary>
    /// Callback when the scroll menu has moved onto a new item
    /// </summary>
    /// <param name="event"></param>
    void valueChangedCB(lv_event_t* event);

    /// <summary>
    /// This is called when the user presses the exit button for the connection
    /// </summary>
    /// <param name="event">The event object for refresh being called into by the scanning</param>
    void exitButtonCB(lv_event_t* event = NULL);

public:
    static void value_changed_cb(lv_event_t* event);
    static void tile_btn_cb(lv_event_t* event);
    static void tile_btn_defocus_cb(lv_event_t* event);
    static void exit_btn_cb(lv_event_t* event);
};

#endif