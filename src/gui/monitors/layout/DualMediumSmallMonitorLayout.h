#ifndef _MEDIUM_SMALL_MONITOR_LAYOUT_H
#define _MEDIUM_SMALL_MONITOR_LAYOUT_H

#include "BaseMonitorLayout.h"

#define MEDIUM_SMALL_MONITOR_OBJECTS 4

class DualMediumSmallMonitorLayout : public BaseMonitorLayout
{
private:
    MonitorLvObject* mediumSmallMonitorLvObjects[MEDIUM_SMALL_MONITOR_OBJECTS];

public:
    DualMediumSmallMonitorLayout(MonitorLvObject* smallTopMonitorLvObject, MonitorLvObject* mediumTopMonitorLvObject, MonitorLvObject* mediumBottomMonitorLvObject, MonitorLvObject* smallBottomMonitorLvObject) 
        : BaseMonitorLayout(mediumSmallMonitorLvObjects, MEDIUM_SMALL_MONITOR_OBJECTS)
    {
        this->mediumSmallMonitorLvObjects[0] = smallTopMonitorLvObject;
        this->mediumSmallMonitorLvObjects[1] = mediumTopMonitorLvObject;
        this->mediumSmallMonitorLvObjects[2] = mediumBottomMonitorLvObject;
        this->mediumSmallMonitorLvObjects[3] = smallBottomMonitorLvObject;
    }
};

#endif