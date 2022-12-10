#ifndef _BATTERY_CAPACITY_SMALL_H
#define _BATTERY_CAPACITY_SMALL_H

#include "BaseNumericSmall.h"


class BatteryCapacitySmall : public BaseNumericSmall
{
public:
    BatteryCapacitySmall(const char* title = "Bat:") : BaseNumericSmall(BikeStateAttributeIndex::BATTERY_CHARGE_PERCENT, MonitorAttributeType::EVERY_MINUTE, title) {}
};

#endif