#ifndef _BASE_LV_OBJECT_H
#define _BASE_LV_OBJECT_H

#include <lvgl.h>

/// <summary>
/// This is the base of all our LvObjects which should be built frist as an abstarct
/// collection then a call to createLvObj should instanciate the required lv objects.
/// </summary>

class BaseLvObject
{
protected:
    /// <summary>
    /// This is the LV instance of the encapulated functionality of this classes instance
    /// </summary>
    lv_obj_t* this_obj = NULL;
public:
    /// <summary>
    /// This Creates the instance of the Lv Objects associated with the instance and sub components associated with it
    /// </summary>
    /// <param name="parent">The LV object parent on which to attach the created lv object for this instance</param>
    /// <returns>The created LV object instance</returns>
    virtual lv_obj_t* createLvObj(lv_obj_t* parent) = 0;

    /// <summary>
    /// This means the object and any sub objects should set any groups to be in focus at this point
    /// </summary>
    virtual void focusLvObj() = 0;

    /// <summary>
    /// Returns the LV object instance to represent this class instance
    /// </summary>
    /// <returns>The LV object instance to represent this class instance</returns>
    virtual lv_obj_t* getLvObj();
};

#endif