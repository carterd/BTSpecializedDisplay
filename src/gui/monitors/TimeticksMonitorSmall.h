#ifndef _TIMETICKS_MONITOR_SMALL_H
#define _TIMETICKS_MONITOR_SMALL_H

#include "..\MonitorLvObject.h"


class TimeticksMonitorSmall : public MonitorLvObject
{
private:
    lv_obj_t* label_obj;

private:
    void update();

public:
    TimeticksMonitorSmall();

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