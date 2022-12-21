#ifndef _MAIN_SMALL_MONITOR_LAYOUT_H
#define _MAIN_SMALL_MONITOR_LAYOUT_H

#include "BaseMonitorLayout.h"

#define MAX_MAIN_SMALL_MONITOR_OBJECTS 2

class MainSmallMonitorLayout : public BaseMonitorLayout
{
private:
    MonitorLvObject* mainSmallMonitorLvObjects[MAX_MAIN_SMALL_MONITOR_OBJECTS];
    int mainSmallMonitorHeights[MAX_MAIN_SMALL_MONITOR_OBJECTS];

public:
    MainSmallMonitorLayout(MonitorLvObject* mainMonitorLvObject, MonitorLvObject* smallMonitorLvObject)
        : BaseMonitorLayout(this->mainSmallMonitorLvObjects, this->mainSmallMonitorHeights, MAX_MAIN_SMALL_MONITOR_OBJECTS) {
        this->mainSmallMonitorLvObjects[0] = smallMonitorLvObject;
        this->mainSmallMonitorHeights[0] = SMALL_MONITOR_LV_OBJECT_HEIGHT;
        this->mainSmallMonitorLvObjects[1] = mainMonitorLvObject;
        this->mainSmallMonitorHeights[1] = MAIN_MONITOR_LV_OBJECT_HEIGHT;
    }
};

#endif