#ifndef _SPEED_GRAPH_MONITOR_MAIN_H
#define _SPEED_GRAPH_MONITOR_MAIN_H

#include "../GraphMonitorIterator.h"
#include "../GraphPlot.h"
#include "../GraphAxis.h"
#include "../GraphMonitor.h"

#include "BaseMonitorMain.h"
#include "../../../stats/SpeedMeterLogger.h"
#include "../../../dev/ConfigStore.h"

#define GRAPH_MAX_NO_AXIS_TICKS 12
#define GRAPH_WIDTH 128
#define GRAPH_HEIGHT 100
#define GRAPH_BOTTOM GRAPH_HEIGHT - 1

#define KMPH_TO_MPH_MULTIPLIER 0.621371

class SpeedGraphMonitorMain : public BaseMonitorMain
{
private:
    /// <summary>
    /// This is the graph iterator used in drawing speed plot
    /// </summary>
    SpeedGraphMonitorIterator speedGraphMonitorIterator;

    /// <summary>
    /// This is the logger object which stores the stats to be displayed
    /// </summary>
    SpeedMeterLogger* speedMeterLogger;

    /// <summary>
    /// This is used to detect when the logger has moved it's currently reading period and redraw is in order
    /// </summary>
    uint32_t LoggerPeriodStartTimeTicks;

    /// <summary>
    /// This is the current graphed maximum displayed in multipled wheel rotations
    /// </summary>
    float maxGraphPlotYaxis;

    /// <summary>
    /// Selector for display mode either min/max or average
    /// </summary>
    bool minMaxMode;

    /// <summary>
    /// The config store which contains the bluetooth devices to connect to
    /// </summary>
    ConfigStore* configStore;

protected:
    /// <summary>
    /// The current wheel rotation per min reading
    /// </summary>
    float currentWheelRotationsPerMin;

    /// <summary>
    /// The current speed to display
    /// </summary>
    uint16_t currentMultipliedWheelRotationsPerMin;

private:
    /// <summary>
    /// Updated the graph
    /// </summary>
    void updateGraph();

    /// <summary>
    /// Ensure the current graph is updated
    /// </summary>
    void updateCurrent();

    /// <summary>
    /// Update the value of the max show on the graph
    /// </summary>
    void updateGraphMax();

    /// <summary>
    /// Helper function that sets the Y size of the graph so current and average fits
    /// </summary>
    void adjustCurrentMaxMultipliedWheelRotationsPerMin(float compareValue);

    /// <summary>
    /// Helper function that calculates what the size of axis ticks should be
    /// </summary>
    void adjustGraphYTicks();

private:
    /// <summary>
    /// This is the objec for display of graph
    /// </summary>
    GraphMonitor graphMonitor;

public:
    /// <summary>
    /// Constructor for the SpeedGraphMonitorMain
    /// </summary>
    SpeedGraphMonitorMain(ConfigStore* configStore, SpeedMeterLogger* speedMeterLogger, bool minMaxMode = true, int16_t graphMinLimit = 3000, int16_t graphMaxLimit = 20000);

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

    /// <summary>
    /// Accessor function to set the graph mode to min max
    /// </summary>
    void setMinMaxMode() { this->minMaxMode = true; }

    /// <summary>
    /// Accessor function to set the graph mode to average
    /// </summary>
    void setAverageMode() { this->minMaxMode = false; }
};

#endif