#ifndef _MOTOR_POWER_SMALL_H
#define _MOTOR_POWER_SMALL_H

#include "BaseNumericSmall.h"


class MotorPowerSmall : public BaseNumericSmall
{
public:
    MotorPowerSmall(const char* title = "eWatts:") : BaseNumericSmall(BikeStateAttributeIndex::MOTOR_POWER, MonitorAttributeType::EVERY_SECOND, title) {}
};

#endif