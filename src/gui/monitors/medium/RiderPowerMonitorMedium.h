#ifndef _RIDER_POWER_MONITOR_MEDIUM_H
#define _RIDER_POWER_MONITOR_MEDIUM_H

#include "BaseNumericMonitorMedium.h"


class RiderPowerMonitorMedium : public BaseNumericMonitorMedium
{
public:
    RiderPowerMonitorMedium(const char* title = "Watts", const char* attributeUnits = "Watts") : BaseNumericMonitorMedium(BikeStateAttributeIndex::RIDER_POWER, MonitorAttributeType::EVERY_SECOND, title, attributeUnits) {}
};

#endif