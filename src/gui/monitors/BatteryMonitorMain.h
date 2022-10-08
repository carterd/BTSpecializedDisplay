#ifndef _BATTERY_MONITOR_MAIN_H
#define _BATTERY_MONITOR_MAIN_H

#include "..\MonitorLvObject.h"

class BatteryMonitorMain : public MonitorLvObject
{
private:

    const static lv_point_t battery_line_points[];
    const static lv_coord_t battery_line_point_count = 14;

    lv_obj_t* levels[10];
    lv_point_t charge_line_points[10][2];
    lv_style_t battery_line_style;
    lv_style_t charge_line_style;

    uint8_t displayedPercent;
private:
    void update();

public:
    BatteryMonitorMain();

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
	/// The callback on the list required to be updated, i.e. a bluetooth device detected
	/// </summary>
	/// <param name="event">The lv event that identifies pressing the device entry</param>
	virtual void statusUpdate();
};

#endif