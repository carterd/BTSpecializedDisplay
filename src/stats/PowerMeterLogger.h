#ifndef _POWER_METER_LOGGER_H
#define _POWER_METER_LOGGER_H

#include "BaseLogger.h"

class PowerMeterLogger : public BaseLogger<uint16_t> {
public:
    PowerMeterLogger(uint32_t startTimeTicks = 0L, uint32_t periodLengthTimeTicks = 60000L, int maxPeriodReadings = 480) : BaseLogger<uint16_t>(startTimeTicks, periodLengthTimeTicks, maxPeriodReadings) {}
};

#endif