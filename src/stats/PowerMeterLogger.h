#ifndef _POWER_METER_LOGGER_H
#define _POWER_METER_LOGGER_H

#include <Arduino.h>
#include <vector>
#include <deque>

class PowerMeterLogger {
public:
    struct PeriodReading {
        uint32_t periodStartTimeTicks;
        uint16_t min;
        uint16_t max;
        float average;
    };
private:
    /// <summary>
    /// This attribute will limit the number of reading held by the logger
    /// </summary>
    int maxPeriodReadings;

    uint32_t periodStartTimeTicks;
    uint32_t periodLengthTimeTicks;

    struct MeterReading {
        uint16_t value;
        uint32_t lastFetchTimeTicks;
    };

    std::vector<MeterReading> meterReadings;

    std::deque<PeriodReading> periodReadings;

    uint32_t lastFetchTimeTicks;
protected:
    void processPeriodRead();

    void addPeriodReading(PeriodReading& periodReading);

    /// <summary>
    /// Helper function returns true if the window provided by start and end covers any attributes in
    /// </summary>
    /// <param name="windowStart"></param>
    /// <param name="windowEnd"></param>
    bool checkWindowStartEnd(uint32_t windowStart, uint32_t windowEnd);
    /// <summary>
    /// This will either get the max or min between two given timeticks points
    /// </summary>
    /// <param name="windowStart"></param>
    /// <param name="windowEnd"></param>
    /// <param name="getMax"></param>
    /// <returns></returns>
    uint16_t getLimitPeriodReading(uint32_t windowStart, uint32_t windowEnd, bool getMax);
public:
    PowerMeterLogger(uint32_t startTimeTicks = 0, uint32_t periodLengthTimeTicks = 60000, int maxPeriodReadings = 480);

    void addMeterReading(uint16_t powerValue, uint32_t lastFetchTimeTicks);

    void initLogger(uint32_t startTimeTicks);

    uint16_t getMaxPeriodReading(uint32_t windowLength);

    uint16_t getMaxPeriodReading(uint32_t windowStart, uint32_t windowEnd);

    uint16_t getMinPeriodReading(uint32_t windowLength);

    uint16_t getMinPeriodReading(uint32_t windowStart, uint32_t windowEnd);

    uint32_t getPeriodStartTimeTicks() { return this->periodStartTimeTicks; }

    uint32_t getPeriodLengthTimeTicks() { return this->periodLengthTimeTicks; }

    std::deque<PeriodReading>* getPeriodReadings() { return &this->periodReadings; }
};

#endif