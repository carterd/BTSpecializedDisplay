#ifndef _MAIN_SMALL_MONITOR_LAYOUT_H
#define _MAIN_SMALL_MONITOR_LAYOUT_H

#include "..\..\MonitorLvObject.h"

class MainSmallMonitorLayout : public MonitorLvObject
{
private:
    MonitorLvObject* mainMonitorLvObject;
    MonitorLvObject* smallMonitorLvObject;

public:
    MainSmallMonitorLayout(MonitorLvObject* mainMonitorLvObject, MonitorLvObject* smallMonitorLvObject);

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
    virtual void initBluetoothStats() { this->mainMonitorLvObject->initBluetoothStats(); this->smallMonitorLvObject->initBluetoothStats(); }

    /// <summary>
    /// Only a monitor in focus should be required to update their gui
    /// </summary>
    virtual void focusLvObj(BaseLvObject* defocusLvObj = NULL);

    /// <summary>
    /// Defocus a monitor and hence no more updated on the display
    /// </summary>
    virtual void defocusLvObj();

    /// <summary>
    /// For the layout we don't have any actual lv_objects to update
    /// </summary>
    virtual void updateLvObj() {}
};

#endif