#ifndef _RIDER_POWER_GRAPH_MONITOR_MAIN_H
#define _RIDER_POWER_GRAPH_MONITOR_MAIN_H

#include "../GraphMonitorIterator.h"
#include "BaseMonitorMain.h"
#include "../../../stats/PowerMeterLogger.h"
#include "../GraphMonitor.h"

#define GRAPH_TICKS_SMALL 50
#define GRAPH_TICKS_LARGE 100

#define GRAPH_MAX_NO_AXIS_TICKS 12
#define GRAPH_WIDTH 60
#define GRAPH_HEIGHT 100
#define GRAPH_BOTTOM GRAPH_HEIGHT - 1

class RiderPowerGraphMonitorMain : public BaseMonitorMain
{
private:
    /// <summary>
    /// This is the graph iterator used in drawing power plot
    /// </summary>
    RiderPowerMonitorIterator riderPowerMonitorIterator;

    /// <summary>
    /// This is the logger object which stores the stats to be displayed
    /// </summary>
    PowerMeterLogger* powerMeterLogger;

    /// <summary>
    /// This is used to detect when the logger has moved it's currently reading period and redraw is in order
    /// </summary>
    uint32_t LoggerPeriodStartTimeTicks;

    /// <summary>
    /// The current power to display
    /// </summary>
    uint16_t currentPower;

    /// <summary>
    /// This is the current graphed maximum displayed in multipled wheel rotations
    /// </summary>
    float maxGraphPlotYaxis;

    /// <summary>
    /// Selector for display mode either min/max or average
    /// </summary>
    bool minMaxMode;
private:
    /// <summary>
    /// This is the objec for display of graph
    /// </summary>
    GraphMonitor graphMonitor;

    void updateAxisLabels();

    void updateGraph();

    void updateCurrent();

    void updateGraphMax();

    void updateMultipler(uint16_t maxReading);

public:
    RiderPowerGraphMonitorMain(PowerMeterLogger* powerMeterLogger, bool minMaxMode = true, int16_t graphPowerMinLimit = 225, int16_t graphPowerMaxLimit = 1025);

    /// <summary>
    /// Returns the LV object instance to represent this class instance
    /// </summary>
    /// <returns>The LV object instance to represent this class instance</returns>
    virtual lv_obj_t* createLvObj(lv_obj_t* parent);

   	/// <summary>
	/// The callback on the list required to be updated, i.e. a bluetooth device detected
	/// </summary>
	/// <param name="event">The lv event that identifies pressing the device entry</param>
	virtual void statusUpdate();

    /// <summary>
    /// For the layout we don't have any actual lv_objects to update
    /// </summary>
    virtual void updateLvObj();

    /// <summary>
    /// This will ensure the stats are initialised and the correct interest for the monitor is assigned to the controller
    /// </summary>
    virtual void initBluetoothStats();

    void adjustGraphYTicks();

    void adjustCurrentMaxPower(float compareValue);

    void setMinMaxMode() { this->minMaxMode = true; }

    void setAverageMode() { this->minMaxMode = false; }
};

#endif