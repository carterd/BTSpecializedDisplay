#ifndef _MULTI_SMALL_MONITOR_LAYOUT_H
#define _MULTI_SMALL_MONITOR_LAYOUT_H

#include "..\..\MonitorLvObject.h"

#define MAX_MULTI_SMALL_MONITOR_OBJECTS 8
#define SMALL_MONITOR_LV_OBJECT_HEIGHT 16

class MultiSmallMonitorLayout : public MonitorLvObject
{
private:    
    MonitorLvObject* smallMonitorLvObjects[MAX_MULTI_SMALL_MONITOR_OBJECTS];

public:
    MultiSmallMonitorLayout(MonitorLvObject* smallMonitorLvObject1, MonitorLvObject* smallMonitorLvObject2, MonitorLvObject* smallMonitorLvObject3, MonitorLvObject* smallMonitorLvObject4, MonitorLvObject* smallMonitorLvObject5, MonitorLvObject* smallMonitorLvObject6, MonitorLvObject* smallMonitorLvObject7, MonitorLvObject* smallMonitorLvObject8);

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
};

#endif