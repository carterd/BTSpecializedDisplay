#ifndef _MOTOR_ASSIST_LEVEL_DOTS_SMALL_H
#define _MOTOR_ASSIST_LEVEL_DOTS_SMALL_H

#include "..\..\MonitorLvObject.h"


#define DOT_SIZE 3
#define DOT_ROUNDED true

class MotorAssistLevelDotSmall : public MonitorLvObject
{
private:
    lv_style_t assist_line_style;
    lv_point_t assist_line_points[4][2];
    lv_obj_t* levels[4];

    uint16_t displayedMotorAssistLevel;
    uint8_t displayedBikeOnOffState;

private:
    void update();

public:
    MotorAssistLevelDotSmall();

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