#ifndef _MAIN_SMALL_MONITOR_LAYOUT_H
#define _MAIN_SMALL_MONITOR_LAYOUT_H

#include "BaseMonitorLayout.h"

#define MAX_MAIN_SMALL_MONITOR_OBJECTS 2

class MainSmallMonitorLayout : public BaseMonitorLayout
{
private:
    MonitorLvObject* mainSmallMonitorLvObjects[MAX_MAIN_SMALL_MONITOR_OBJECTS];

public:
    MainSmallMonitorLayout(MonitorLvObject* mainMonitorLvObject, MonitorLvObject* smallMonitorLvObject)
        : BaseMonitorLayout(this->mainSmallMonitorLvObjects, MAX_MAIN_SMALL_MONITOR_OBJECTS) {
        this->mainSmallMonitorLvObjects[0] = smallMonitorLvObject;
        this->mainSmallMonitorLvObjects[1] = mainMonitorLvObject;
    }
};

#endif