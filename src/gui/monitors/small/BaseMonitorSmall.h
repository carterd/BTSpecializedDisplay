#ifndef _BASE_MONITOR_SMALL_H
#define _BASE_MONITOR_SMALL_H

#include "../../MonitorLvObject.h"

/// <summary>
/// This is the base object for monitor objects
/// </summary>
class BaseMonitorSmall : public MonitorLvObject
{
public:
    /// <summary>
    /// This should return the height of the monitor object
    /// </summary>
    virtual lv_style_t* getMonitorPanelStyle(display_theme_styles_t* display_theme_styles);
};

#endif
