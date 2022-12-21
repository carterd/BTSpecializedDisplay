#ifndef _BASE_MONITOR_LAYOUT_H
#define _BASE_MONITOR_LAYOUT_H

#include "../../MonitorLvObject.h"

#define SMALL_MONITOR_LV_OBJECT_HEIGHT 16
#define MAIN_MONITOR_LV_OBJECT_HEIGHT 112
#define MEDIUM_MONITOR_LV_OBJECT_HEIGHT 48

class BaseMonitorLayout : public MonitorLvObject
{
protected:
    /// <summary>
    /// these are the monitor objects ossicated with the layout
    /// </summary>
    MonitorLvObject** monitorLvObjects;

    int* monitorHeights;

    int numberOfMonitorObjects;

public:
    BaseMonitorLayout(MonitorLvObject** monitorLvObjects, int* monitorHeights, int numberOfMonitorObjects);

    /// <summary>
    /// Set the bluetoothController which is required by all monitors
    /// </summary>
    virtual void setBluetoothController(BluetoothBikeController* bluetoothBikeController);

    /// <summary>
    /// Returns the LV object instance to represent this class instance
    /// </summary>
    /// <returns>The LV object instance to represent this class instance</returns>
    virtual lv_obj_t* createLvObj(lv_obj_t* parent);

   	/// <summary>
	/// The callback on the list required to be updated, i.e. a bluetooth device detected
	/// </summary>
	/// <param name="event">The lv event that identifies pressing the device entry</param>
	virtual void statusUpdate();

    /// <summary>
    /// This will ensure the stats are initialised and the correct interest for the monitor is assigned to the controller
    /// </summary>
    virtual void initBluetoothStats();

    /// <summary>
    /// Only a monitor in focus should be required to update their gui
    /// </summary>
    virtual void focusLvObj(BaseLvObject* defocusLvObj = NULL);

    /// <summary>
    /// For the layout we don't have any actual lv_objects to update
    /// </summary>
    virtual void updateLvObj() {}

    /// <summary>
    /// Defocus a monitor and hence no more updated on the display
    /// </summary>
    virtual void defocusLvObj();
};

#endif