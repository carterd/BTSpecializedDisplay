#ifndef _RIDER_POWER_SMALL_H
#define _RIDER_POWER_SMALL_H

#include "BaseNumericSmall.h"


class RiderPowerSmall : public BaseNumericSmall
{
public:
    RiderPowerSmall(const char* title = "Watts:") : BaseNumericSmall(BikeStateAttributeIndex::RIDER_POWER, MonitorAttributeType::EVERY_SECOND, title) {}
};

#endif