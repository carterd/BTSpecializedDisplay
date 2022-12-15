#ifndef _MOTOR_POWER_MONITOR_SMALL_H
#define _MOTOR_POWER_MONITOR_SMALL_H

#include "BaseNumericMonitorSmall.h"


class MotorPowerMonitorSmall : public BaseNumericMonitorSmall
{
public:
    MotorPowerMonitorSmall(const char* title = "e " LV_SYMBOL_CHARGE, const char* attributeUnits = " W") : BaseNumericMonitorSmall(BikeStateAttributeIndex::MOTOR_POWER, MonitorAttributeType::EVERY_SECOND, title, attributeUnits) {}
};

#endif