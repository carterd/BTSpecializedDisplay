#include "GraphMonitorIterator.h"

#define MILLI_PER_SECOND 1000
#define SECONDS_PER_X_TICK 600

void SpeedGraphMonitorIterator::init()
{
    uint32_t periodGraphEndTimeTicks = this->speedMeterLogger->getPeriodStartTimeTicks();

    std::deque<SpeedMeterLogger::PeriodReading>* periodReadings = this->speedMeterLogger->getPeriodReadings();
    this->periodGraphEndTimeTicks = periodGraphEndTimeTicks;
    this->maxLineWidth = this->speedMeterLogger->getPeriodLengthTimeTicks() * 1.5;
    this->mintuesOffset = ((periodGraphEndTimeTicks / MILLI_PER_SECOND) % (SECONDS_PER_X_TICK));
    this->iter = periodReadings->crbegin();
    this->end = periodReadings->crend();

    this->maxGraphPlotYaxis = 0.0;
    if (this->iter != end) {
        this->prevTimeTicks = (*this->iter).periodStartTimeTicks;
        this->prevAvgCoord = (*this->iter).average;
    }
}

bool SpeedGraphMonitorIterator::getNext()
{
    uint32_t timeTicks;
    float avgCoord;
    if (this->iter != this->end) {
        this->iter++;
        if (this->iter != end) {
            timeTicks = (*iter).periodStartTimeTicks;
            if (timeTicks > this->prevTimeTicks - this->maxLineWidth) {
                avgCoord = (*this->iter).average;
            }
            else {
                timeTicks = this->prevTimeTicks;
                avgCoord = this->prevAvgCoord;
            }
        }
        else {
            timeTicks = this->prevTimeTicks;
            avgCoord = this->prevAvgCoord;
        }

        this->lineStart.x = -((int16_t)((this->periodGraphEndTimeTicks - timeTicks) / MILLI_PER_SECOND) - this->mintuesOffset);
        this->lineStart.y = (uint16_t)avgCoord;
        this->lineEnd.x = -((int16_t)((this->periodGraphEndTimeTicks - this->prevTimeTicks) / MILLI_PER_SECOND) - this->mintuesOffset);
        this->lineEnd.y = (uint16_t)this->prevAvgCoord;

        if (this->maxGraphPlotYaxis < avgCoord) { this->maxGraphPlotYaxis = avgCoord; }

        this->prevTimeTicks = timeTicks;
        this->prevAvgCoord = avgCoord;

        return true;
    }
    return false;
}

void RiderPowerMonitorIterator::init()
{
    uint32_t periodGraphEndTimeTicks = this->powerMeterLogger->getPeriodStartTimeTicks();

    std::deque<PowerMeterLogger::PeriodReading>* periodReadings = this->powerMeterLogger->getPeriodReadings();
    this->periodGraphEndTimeTicks = periodGraphEndTimeTicks;
    this->maxLineWidth = this->powerMeterLogger->getPeriodLengthTimeTicks() * 1.5;
    this->mintuesOffset = ((periodGraphEndTimeTicks / MILLI_PER_SECOND) % (SECONDS_PER_X_TICK));
    this->iter = periodReadings->crbegin();
    this->end = periodReadings->crend();

    this->maxGraphPlotYaxis = 0.0;
    if (this->iter != end) {
        this->prevTimeTicks = (*this->iter).periodStartTimeTicks;
        this->prevAvgCoord = (*this->iter).average;
    }
}

bool RiderPowerMonitorIterator::getNext()
{
    uint32_t timeTicks;
    float avgCoord;
    if (this->iter != this->end) {
        this->iter++;
        if (this->iter != end) {
            timeTicks = (*iter).periodStartTimeTicks;
            if (timeTicks > this->prevTimeTicks - this->maxLineWidth) {
                avgCoord = (*this->iter).average;
            }
            else {
                timeTicks = this->prevTimeTicks;
                avgCoord = this->prevAvgCoord;
            }
        }
        else {
            timeTicks = this->prevTimeTicks;
            avgCoord = this->prevAvgCoord;
        }

        this->lineStart.x = -((int16_t)((this->periodGraphEndTimeTicks - timeTicks) / MILLI_PER_SECOND) - this->mintuesOffset);
        this->lineStart.y = (uint16_t)avgCoord;
        this->lineEnd.x = -((int16_t)((this->periodGraphEndTimeTicks - this->prevTimeTicks) / MILLI_PER_SECOND) - this->mintuesOffset);
        this->lineEnd.y = (uint16_t)this->prevAvgCoord;

        if (this->maxGraphPlotYaxis < avgCoord) { this->maxGraphPlotYaxis = avgCoord; }

        this->prevTimeTicks = timeTicks;
        this->prevAvgCoord = avgCoord;

        return true;
    }
    return false;

}

