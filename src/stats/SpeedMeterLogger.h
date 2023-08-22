#ifndef _SPEED_METER_LOGGER_H
#define _SPEED_METER_LOGGER_H

#include "BaseLogger.h"

#define FLOAT_TO_UINT16_MULTIPLIER 16.0f

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
        BaseLogger<uint16_t>::addReading(this->convertWheelRotationsPerMinToMultipliedWheelRotations(wheelRotationsPerMin), lastFetchTimeTicks);
    }

    uint16_t convertWheelRotationsPerMinToMultipliedWheelRotations(float wheelRotationsPerMin) {
        return wheelRotationsPerMin * FLOAT_TO_UINT16_MULTIPLIER;
    }

    /// <summary>
    /// This converts a float to the stored reading
    /// </summary>
    /// <param name="reading"></param>
    /// <returns></returns>
    float convertMultipliedWheelRotationsPerMinToKmph(float multipliedWheelRotationsPerMin) {
        return (multipliedWheelRotationsPerMin / FLOAT_TO_UINT16_MULTIPLIER) * this->wheelCircumferenceMm * 60.0f / (1000000.0f);
    }

    float convertWheelRotationsPerMinToKmph(float wheelRotationsPerMin) {
        return wheelRotationsPerMin * this->wheelCircumferenceMm * 60.0f / 1000000.0f;
    }

    float convertKmphToMultipliedWheelRotationsPerMin(float kmph) {
        return  ( kmph * FLOAT_TO_UINT16_MULTIPLIER )  /  (this->wheelCircumferenceMm * 60.0f / (1000000.0f));
    }
};

#endif