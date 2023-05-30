#ifndef _SPEED_METER_LOGGER_H
#define _SPEED_METER_LOGGER_H

#include "BaseLogger.h"

#define FLOAT_TO_UINT16_MULTIPLIER 512.0f

class SpeedMeterLogger : public BaseLogger<uint16_t> {
private:
    uint16_t wheelCircumferenceMm;
public:
    /// <summary>
    /// Constructor for the logger
    /// </summary>
    /// <param name="startTimeTicks"></param>
    /// <param name="periodLengthTimeTicks"></param>
    /// <param name="maxPeriodReadings"></param>
    SpeedMeterLogger(uint32_t startTimeTicks = 0L, uint32_t periodLengthTimeTicks = 60000L, int maxPeriodReadings = 480) : BaseLogger<uint16_t>(startTimeTicks, periodLengthTimeTicks, maxPeriodReadings) { this->wheelCircumferenceMm = 0; }

    /// <summary>
    /// Setter for the wheel circumference in using to convert readings
    /// </summary>
    /// <param name="wheelCircumferenceMm"></param>
    void setWheelCircumference(uint16_t wheelCircumferenceMm) { this->wheelCircumferenceMm = wheelCircumferenceMm; }

    /// <summary>
    /// Speed readings are given as wheel rotations per min
    /// </summary>
    /// <param name="reading"></param>
    /// <param name="lastFetchTimeTicks"></param>
    void addReading(float wheelRotationsPerMin, uint32_t lastFetchTimeTicks) {
        // Multiply the speed from a float to uint .. to save memory but keep enough accuracy
        BaseLogger<uint16_t>::addReading(this->convertWheelRotationsPerMinToMultipliedKmph(wheelRotationsPerMin), lastFetchTimeTicks);
    }

    /// <summary>
    /// This converts a float to the stored reading
    /// </summary>
    /// <param name="reading"></param>
    /// <returns></returns>
    uint16_t convertWheelRotationsPerMinToMultipliedKmph(float wheelRotationsPerMin) {
        float kmph = wheelRotationsPerMin * this->wheelCircumferenceMm * 60.0f / (1000000.0f / FLOAT_TO_UINT16_MULTIPLIER);
        return kmph;
    }

    /// <summary>
    /// Given a value in Kmph convert to uint16_t value using in this logger
    /// </summary>
    uint16_t convertKmphToMultipliedKmph(float kmph) { return FLOAT_TO_UINT16_MULTIPLIER * kmph; }

    /// <summary>
    /// Given a value in Logger stored multiplier Kmph convert to uint16_t value using in this logger
    /// </summary>
    float convertMultipliedKmphToKmph(uint16_t multipliedKmph) { return multipliedKmph / FLOAT_TO_UINT16_MULTIPLIER; }

};

#endif