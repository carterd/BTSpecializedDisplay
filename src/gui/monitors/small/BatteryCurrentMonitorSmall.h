#ifndef _BATTERY_CURRENT_MONITOR_SMALL_H
#define _BATTERY_CURRENT_MONITOR_SMALL_H

#include "BaseNumericMonitorSmall.h"


class BatteryCurrentMonitorSmall : public BaseNumericMonitorSmall
{
public:
    BatteryCurrentMonitorSmall(const char* title = LV_SYMBOL_BATTERY_FULL " I" , const char* attributeUnits = " A") : BaseNumericMonitorSmall(BikeStateAttributeIndex::BATTERY_CURRENT, MonitorAttributeType::EVERY_SECOND, title, attributeUnits) {}
};

#endif