#ifndef _BATTERY_CAPACITY_MONITOR_MAIN_H
#define _BATTERY_CAPACITY_MONITOR_MAIN_H

#include "BaseMonitorMain.h"

#include "..\..\..\themes\display_theme.h"

class BatteryCapacityMonitorMain : public BaseMonitorMain
{
public:
    const static lv_point_t battery_line_points_sh1107[];
    const static lv_point_t battery_line_points_tdisplay[];
    const static lv_point_t battery_line_points_tdisplay_s3[];    
    const static lv_coord_t battery_line_point_count = 14;
    const static charge_coords_vert_t charge_coords_sh1107;
    const static charge_coords_vert_t charge_coords_tdisplay;
    const static charge_coords_vert_t charge_coords_tdisplay_s3;

private:
    lv_obj_t* levels[10];
    lv_point_t charge_line_points[10][2];

    uint8_t displayedPercent;
    const charge_coords_vert_t* main_battery_coords;

public:
    BatteryCapacityMonitorMain();

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