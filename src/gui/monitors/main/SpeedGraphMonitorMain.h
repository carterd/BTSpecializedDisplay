#ifndef _SPEED_GRAPH_MONITOR_MAIN_H
#define _SPEED_GRAPH_MONITOR_MAIN_H

#include "../MonitorGraph.h"
#include "../MonitorGraphAxis.h"

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
    /// The parent to all the axis lines to allow them to be easily managed
    /// </summary>
    lv_obj_t* graphAxisParent;
    /// <summary>
    /// This is the parent of the lines that make the parent
    /// </summary>
    lv_obj_t* graphLinesParent;

    lv_obj_t* graphCurrentLineParent;

    /// <summary>
    /// This is the store for the point that make up the graph axis
    /// </summary>
    lv_point_t graph_axis_line_points[GRAPH_MAX_NO_AXIS_TICKS][2];
    /// <summary>
    /// The points for the current power line on the graph
    /// </summary>
    lv_point_t graph_current_line_points[2];
    /// <summary>
    /// The axis line style to be used by axis lines
    /// </summary>
    lv_style_t graph_axis_line_style;
    /// <summary>
    /// The actual graph lines
    /// </summary>
    lv_style_t graph_line_style;
    /// <summary>
    /// The dashed line style for current power
    /// </summary>
    lv_style_t graph_dash_line_style;
    /// <summary>
    /// This is a black line style to allow dotted line to be seen on white background
    /// </summary>
    lv_style_t graph_dash_line_background_style;

    /// <summary>
    /// This is the logger object which stores the stats to be displayed
    /// </summary>
    SpeedMeterLogger* speedMeterLogger;

    /// <summary>
    /// This is used to detect when the logger has moved it's currently reading period
    /// </summary>
    uint32_t speedMeterLoggerPeriodStartTimeTicks;

    /// <summary>
    /// This is the max power that can be shown on the graph
    /// </summary>
    uint16_t graphDisplayMaxSpeed;
    /// <summary>
    /// These are the display limits for the graph
    /// </summary>
    float graphMinLimit;
    /// <summary>
    /// These are the display limits for the graph
    /// </summary>
    float graphMaxLimit;
    
    /// <summary>
    /// This is the current reading multiplied by the wheel rotations per min multiplier
    /// </summary>
    float currentMaxMultipliedWheelRotationsPerMin;

    /// <summary>
    /// This is the current graphed maximum displayed in multipled wheel rotations
    /// </summary>
    float currentMaxAverageMultipliedWheelRotations;

    /// <summary>
    /// The current wheel rotation per min reading
    /// </summary>
    float currentWheelRotationsPerMin;
    /// <summary>
    /// The current speed to display
    /// </summary>
    uint16_t currentMultipliedWheelRotationsPerMin;
    /// <summary>
    /// Selector for display mode either min/max or average
    /// </summary>
    bool minMaxMode;
    /// <summary>
    /// The config store which contains the bluetooth devices to connect to
    /// </summary>
    ConfigStore* configStore;
    /// <summary>
    /// The wheel size for calculation from rotations to speed
    /// </summary>
    uint16_t wheelCircumferenceMm;

    bool redraw;

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

    void adjustCurrentMaxMultipliedWheelRotationsPerMin(float compareValue);

    void adjustGraphYTicks();

private:
    MonitorGraph currentGraph;

    MonitorGraph monitorGraph;

    MonitorGraphAxis monitorGraphAxis;

public:
    SpeedGraphMonitorMain(ConfigStore* configStore, SpeedMeterLogger* speedMeterLogger, bool minMaxMode = true, float graphMinLimit = 24.0F, float graphMaxLimit = 100.0F);

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

    void setMinMaxMode() { this->minMaxMode = true; }

    void setAverageMode() { this->minMaxMode = false; }
};

#endif