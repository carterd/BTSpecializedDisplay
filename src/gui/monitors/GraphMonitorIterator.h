#ifndef _GRAPH_MONITOR_ITERATOR_H
#define _GRAPH_MONITOR_ITERATOR_H

#include "GraphPlot.h"
#include "GraphAxis.h"
#include "../../stats/SpeedMeterLogger.h"
#include "../../stats/PowerMeterLogger.h"

class GraphMonitorPointsIterator {
public:
    /// <summary>
    /// Called before getNext should be called in order to start the iterator at the first point
    /// </summary>
    virtual void init() = 0;
    /// <summary>
    /// This should move the iterator to the next point and if no points are left returns false
    /// </summary>
    /// <returns>false if there are no more points data to be retried</returns>
    virtual bool getNext() = 0;
public:
    /// <summary>
    /// This is the point of the line start
    /// </summary>
    GraphPoint lineStart;
    /// <summary>
    /// This is the point of the line end
    /// </summary>
    GraphPoint lineEnd;
};

class SpeedGraphMonitorIterator : public GraphMonitorPointsIterator {
private:
    SpeedMeterLogger* speedMeterLogger;
public:
    /// <summary>
    /// This is the initial
    /// </summary>
    /// <param name="speedMeterLogger"></param>
    virtual bool getNext();
    virtual void init();
public:
    SpeedGraphMonitorIterator(SpeedMeterLogger* speedMeterLogger) { this->speedMeterLogger = speedMeterLogger; }
    std::reverse_iterator<std::deque<BaseLogger<uint16_t>::PeriodReading>::const_iterator> end;
    std::reverse_iterator<std::deque<BaseLogger<uint16_t>::PeriodReading>::const_iterator> iter;
    uint32_t periodGraphEndTimeTicks;
    int16_t mintuesOffset;
    uint32_t maxLineWidth;
    uint32_t prevTimeTicks;
    float prevAvgCoord;
    float maxGraphPlotYaxis;
};


class RiderPowerMonitorIterator : public GraphMonitorPointsIterator {
private:
    PowerMeterLogger* powerMeterLogger;
public:
    virtual bool getNext();
    virtual void init();
public:
    RiderPowerMonitorIterator(PowerMeterLogger* powerMeterLogger) { this->powerMeterLogger = powerMeterLogger; }
    std::reverse_iterator<std::deque<BaseLogger<uint16_t>::PeriodReading>::const_iterator> end;
    std::reverse_iterator<std::deque<BaseLogger<uint16_t>::PeriodReading>::const_iterator> iter;
    uint32_t periodGraphEndTimeTicks;
    int16_t mintuesOffset;
    uint32_t maxLineWidth;
    uint32_t prevTimeTicks;
    float prevAvgCoord;
    float maxGraphPlotYaxis;
};

#endif