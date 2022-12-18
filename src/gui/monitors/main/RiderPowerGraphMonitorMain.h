#ifndef _RIDER_POWER_GRAPH_MONITOR_MAIN_H
#define _RIDER_POWER_GRAPH_MONITOR_MAIN_H

#include "../../MonitorLvObject.h"
#include "../../../stats/PowerMeterLogger.h"

#define GRAPH_TICKS_SMALL 50
#define GRAPH_TICKS_LARGE 100

#define GRAPH_MAX_NO_AXIS_TICKS 12
#define GRAPH_WIDTH 60
#define GRAPH_HEIGHT 100
#define GRAPH_BOTTOM GRAPH_HEIGHT - 1

class RiderPowerGraphMonitorMain : public MonitorLvObject
{
private:
    /// <summary>
    /// The parent to all the axis lines to allow them to be easily managed
    /// </summary>
    lv_obj_t* leftAxisLabelsParent;
    /// <summary>
    /// This is the parent of the lines that make the parent
    /// </summary>
    lv_obj_t* graphLinesParent;
    /// <summary>
    /// Current line that shows your power
    /// </summary>
    lv_obj_t* currentLine;
    /// <summary>
    /// Current background line
    /// </summary>
    lv_obj_t* currentBackgroundLine;

    /// <summary>
    /// This is the store for the point that make up the graph axis
    /// </summary>
    lv_point_t graph_axis_line_points[GRAPH_MAX_NO_AXIS_TICKS][2];
    /// <summary>
    /// This is for bar graphs of max and min
    /// </summary>
    lv_point_t graph_bar_line_points[GRAPH_WIDTH][2];
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
    PowerMeterLogger* powerMeterLogger;

    /// <summary>
    /// This is used to detect when the logger has moved it's currently reading period
    /// </summary>
    uint32_t powerMeterLoggerPeriodStartTimeTicks;

    /// <summary>
    /// This is the wattage multiplier for the display
    /// </summary>
    float graphDisplayMultiplier;

    /// <summary>
    /// This is the max power that can be shown on the graph
    /// </summary>
    uint16_t graphDisplayMaxPower;
    /// <summary>
    /// These are the display limits for the graph
    /// </summary>
    uint16_t graphPowerMinLimit;
    /// <summary>
    /// These are the display limits for the graph
    /// </summary>
    uint16_t graphPowerMaxLimit;
    /// <summary>
    /// The current power to display
    /// </summary>
    uint16_t currentPower;
    /// <summary>
    /// Selector for display mode either min/max or average
    /// </summary>
    bool minMaxMode;
private:

    void updateAxisLabels();

    void updateGraph();

    void updateCurrent();

    void updateMultipler(uint16_t maxReading);

public:
    RiderPowerGraphMonitorMain(PowerMeterLogger* powerMeterLogger, bool minMaxMode = true, uint16_t graphPowerMinLimit = 225, uint16_t graphPowerMaxLimit = 1025);

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