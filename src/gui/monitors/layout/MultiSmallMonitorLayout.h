#ifndef _MULTI_SMALL_MONITOR_LAYOUT_H
#define _MULTI_SMALL_MONITOR_LAYOUT_H

#include "BaseMonitorLayout.h"

#define MAX_MULTI_SMALL_MONITOR_OBJECTS 8

class MultiSmallMonitorLayout : public BaseMonitorLayout
{
private:    
    MonitorLvObject* smallMonitorLvObjects[MAX_MULTI_SMALL_MONITOR_OBJECTS];
    int smallMonitorHeights[MAX_MULTI_SMALL_MONITOR_OBJECTS];

public:
    MultiSmallMonitorLayout(MonitorLvObject* smallMonitorLvObject1, MonitorLvObject* smallMonitorLvObject2, MonitorLvObject* smallMonitorLvObject3, MonitorLvObject* smallMonitorLvObject4, MonitorLvObject* smallMonitorLvObject5, MonitorLvObject* smallMonitorLvObject6, MonitorLvObject* smallMonitorLvObject7, MonitorLvObject* smallMonitorLvObject8)
        : BaseMonitorLayout(this->smallMonitorLvObjects, this->smallMonitorHeights, MAX_MULTI_SMALL_MONITOR_OBJECTS)
    {
        this->smallMonitorLvObjects[0] = smallMonitorLvObject1;
        this->smallMonitorLvObjects[1] = smallMonitorLvObject2;
        this->smallMonitorLvObjects[2] = smallMonitorLvObject3;
        this->smallMonitorLvObjects[3] = smallMonitorLvObject4;
        this->smallMonitorLvObjects[4] = smallMonitorLvObject5;
        this->smallMonitorLvObjects[5] = smallMonitorLvObject6;
        this->smallMonitorLvObjects[6] = smallMonitorLvObject7;
        this->smallMonitorLvObjects[7] = smallMonitorLvObject8;
        for (int i = 0; i < MAX_MULTI_SMALL_MONITOR_OBJECTS; i++) {
            this->smallMonitorHeights[i] = SMALL_MONITOR_LV_OBJECT_HEIGHT;
        }
    }
};

#endif