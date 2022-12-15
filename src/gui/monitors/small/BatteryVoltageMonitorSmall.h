#ifndef _BATTERY_VOLTAGE_MONITOR_SMALL_H
#define _BATTERY_VOLTAGE_MONITOR_SMALL_H

#include "BaseNumericMonitorSmall.h"


class BatteryVoltageMonitorSmall : public BaseNumericMonitorSmall
{
public:
    BatteryVoltageMonitorSmall(const char* title = LV_SYMBOL_BATTERY_FULL " V" , const char* attributeUnits = " V") : BaseNumericMonitorSmall(BikeStateAttributeIndex::BATTERY_VOLTAGE, MonitorAttributeType::EVERY_SECOND, title, attributeUnits) {}
};

#endif