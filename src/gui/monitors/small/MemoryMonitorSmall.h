#ifndef _MEMORY_MONITOR_SMALL_H
#define _MEMORY_MONITOR_SMALL_H

#include "BaseNumericMonitorSmall.h"


class MemoryMonitorSmall : public MonitorLvObject
{
private:
    /// <summary>
    /// This is the value label object require to be updated to show different values
    /// </summary>
    lv_obj_t* value_obj;

    /// <summary>
    /// This is the title displayed for the value
    /// </summary>
    const char* attributeTitle;
    /// <summary>
    /// This is the optional units of the value
    /// </summary>
    const char* attributeUnits;
public:
    /// <summary>
    /// Constructor for the Display of monitor
    /// </summary>
    /// <param name="title"></param>
    /// <param name="attributeUnits"></param>
    MemoryMonitorSmall(const char* title = "Mem:", const char* attributeUnits = "");

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
    virtual void initBluetoothStats() {}
};

#endif