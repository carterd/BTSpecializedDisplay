#ifndef _BATTERY_CAPACITY_IMAGE_MONITOR_SMALL_H
#define _BATTERY_CAPACITY_IMAGE_MONITOR_SMALL_H

#include "BaseNumericMonitorSmall.h"


class BatteryCapacityImageMonitorSmall : public MonitorLvObject
{
    const static lv_point_t battery_line_points[];
    const static lv_coord_t battery_line_point_count = 14;
    lv_obj_t* level;
    lv_point_t charge_line_points[2];

    lv_style_t battery_line_style;
    lv_style_t charge_line_style;

    uint8_t displayedPercent;
public:
    BatteryCapacityImageMonitorSmall();
    
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
    /// For the layout we don't have any actual lv_objects to update
    /// </summary>
    virtual void updateLvObj();
};

#endif