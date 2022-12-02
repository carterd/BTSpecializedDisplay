#ifndef _BLANK_SMALL_H
#define _BLANK_SMALL_H

#include "..\..\MonitorLvObject.h"


#define DOT_SIZE 3
#define DOT_ROUNDED true

#define ASSIST_LEVEL_TEXT_0 "-"
#define ASSIST_LEVEL_TEXT_1 "Eco"
#define ASSIST_LEVEL_TEXT_2 "Trail"
#define ASSIST_LEVEL_TEXT_3 "Turbo"

class BlankSmall : public MonitorLvObject
{
private:

public:

    /// <summary>
    /// Returns the LV object instance to represent this class instance
    /// </summary>
    /// <returns>The LV object instance to represent this class instance</returns>
    virtual lv_obj_t* createLvObj(lv_obj_t* parent) { this->this_obj = lv_obj_create(parent); return this->this_obj;  }

    /// <summary>
    /// This means the object and any sub objects should set any groups to be in focus at this point
    /// </summary>
    virtual void focusLvObj(BaseLvObject* defocusLvObj = NULL) {}

   	/// <summary>
	/// The callback on the list required to be updated, i.e. a bluetooth device detected
	/// </summary>
	/// <param name="event">The lv event that identifies pressing the device entry</param>
	virtual void statusUpdate() {}
};

#endif