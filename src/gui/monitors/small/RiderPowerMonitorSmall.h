#ifndef _RIDER_POWER_MONITOR_SMALL_H
#define _RIDER_POWER_MONITOR_SMALL_H

#include "BaseNumericMonitorSmall.h"


class RiderPowerMonitorSmall : public BaseNumericMonitorSmall
{
public:
    RiderPowerMonitorSmall(const char* title = LV_SYMBOL_REFRESH " " LV_SYMBOL_CHARGE, const char* attributeUnits = " W") : BaseNumericMonitorSmall(BikeStateAttributeIndex::RIDER_POWER, MonitorAttributeType::EVERY_SECOND, title, attributeUnits) {}
};

#endif