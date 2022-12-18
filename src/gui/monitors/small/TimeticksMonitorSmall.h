#ifndef _TIMETICKS_MONITOR_SMALL_H
#define _TIMETICKS_MONITOR_SMALL_H

#include "../../MonitorLvObject.h"


class TimeticksMonitorSmall : public MonitorLvObject
{
private:
    lv_obj_t* value_obj;

    const char* attributeTitle;

public:
    TimeticksMonitorSmall(const char* attributeTitle = "Clk:");

    /// <summary>
    /// Returns the LV object instance to represent this class instance
    /// </summary>
    /// <returns>The LV object instance to represent this class instance</returns>
    virtual lv_obj_t* createLvObj(lv_obj_t* parent);

   	/// <summary>
	/// The callback on the list required to be updated, i.e. a bluetooth device detected
	/// </summary>
	/// <param name="event">The lv event that identifies pressing the device entry</param>
	virtual void updateLvObj();

    /// <summary>
    /// This will ensure the stats are initialised and the correct interest for the monitor is assigned to the controller
    /// </summary>
    virtual void initBluetoothStats();
};

#endif