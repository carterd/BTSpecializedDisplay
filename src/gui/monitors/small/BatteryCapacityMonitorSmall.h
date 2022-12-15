#ifndef _BATTERY_CAPACITY_MONITOR_SMALL_H
#define _BATTERY_CAPACITY_MONITOR_SMALL_H

#include "BaseNumericMonitorSmall.h"


class BatteryCapacityMonitorSmall : public BaseNumericMonitorSmall
{
public:
    BatteryCapacityMonitorSmall(const char* title = " " LV_SYMBOL_BATTERY_FULL, const char* attributeUnits = " %") : BaseNumericMonitorSmall(BikeStateAttributeIndex::BATTERY_CHARGE_PERCENT, MonitorAttributeType::EVERY_MINUTE, title, attributeUnits) {}
};

#endif