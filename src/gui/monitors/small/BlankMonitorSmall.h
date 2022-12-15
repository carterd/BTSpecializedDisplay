#ifndef _BLANK_MONITOR_SMALL_H
#define _BLANK_MONITOR_SMALL_H

#include "..\..\MonitorLvObject.h"

class BlankMonitorSmall : public MonitorLvObject
{
private:

public:

    /// <summary>
    /// Returns the LV object instance to represent this class instance
    /// </summary>
    /// <returns>The LV object instance to represent this class instance</returns>
    virtual lv_obj_t* createLvObj(lv_obj_t* parent) { this->this_obj = lv_obj_create(parent); return this->this_obj;  }

   	/// <summary>
	/// The callback on the list required to be updated, i.e. a bluetooth device detected
	/// </summary>
	/// <param name="event">The lv event that identifies pressing the device entry</param>
	virtual void statusUpdate() {}

    /// <summary>
    /// This will ensure the stats are initialised and the correct interest for the monitor is assigned to the controller
    /// </summary>
    virtual void initBluetoothStats() {}

    /// <summary>
    /// No need to perform and update of the LV object as this object is static
    /// </summary>
    virtual void updateLvObj() {}
};

#endif