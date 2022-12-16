#ifndef _BASE_LOGGER_H
#define _BASE_LOGGER_H

#include <Arduino.h>
#include <vector>
#include <deque>

template <typename T>
class BaseLogger {
public:
    struct PeriodReading {
        uint32_t periodStartTimeTicks;
        T min;
        T max;
        float average;
    };
private:
    struct MeterReading {
        T value;
        uint32_t lastFetchTimeTicks;
    };
    /// <summary>
    /// This attribute will limit the number of reading held by the logger
    /// </summary>
    int maxPeriodReadings;
    /// <summary>
    /// This is effectively the time stamp for the current values being recorded in meterReadings
    /// </summary>
    uint32_t periodStartTimeTicks;
    /// <summary>
    /// This is the period length for colleting reading in meter readings
    /// </summary>
    uint32_t periodLengthTimeTicks;

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
    T getLimitPeriodReading(uint32_t windowStart, uint32_t windowEnd, bool getMax);
public:
    BaseLogger(uint32_t startTimeTicks, uint32_t periodLengthTimeTicks, int maxPeriodReadings);

    void addReading(T reading, uint32_t lastFetchTimeTicks);

    void initLogger(uint32_t startTimeTicks);

    T getMaxPeriodReading(uint32_t windowLength);

    T getMaxPeriodReading(uint32_t windowStart, uint32_t windowEnd);

    T getMinPeriodReading(uint32_t windowLength);

    T getMinPeriodReading(uint32_t windowStart, uint32_t windowEnd);

    uint32_t getPeriodStartTimeTicks() { return this->periodStartTimeTicks; }

    uint32_t getPeriodLengthTimeTicks() { return this->periodLengthTimeTicks; }

    std::deque<PeriodReading>* getPeriodReadings() { return &this->periodReadings; }
};

#endif