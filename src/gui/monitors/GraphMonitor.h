#ifndef _GRAPH_MONITOR_MAIN_H
#define _GRAPH_MONITOR_MAIN_H

#include "GraphMonitorIterator.h"
#include "GraphPlot.h"
#include "GraphAxis.h"
#include "..\BaseLvObject.h"

#define GRAPH_MAX_NO_AXIS_TICKS 12
#define GRAPH_WIDTH 128
#define GRAPH_HEIGHT 100
#define GRAPH_BOTTOM GRAPH_HEIGHT - 1

#define KMPH_TO_MPH_MULTIPLIER 0.621371

class GraphMonitor : BaseLvObject
{
private:
    /// <summary>
    /// This is the style of the graph plot
    /// </summary>
    lv_style_t* graph_plot_style;
    /// <summary>
    /// This is the style of the curret plot
    /// </summary>
    lv_style_t* graph_current_style;
    /// <summary>
    /// This is the style of the axis
    /// </summary>
    lv_style_t* graph_axis_style;
    /// <summary>
    /// This is the style of the large ticks x and y
    /// </summary>
    lv_point_t graph_axis_large_ticks;
    /// <summary>
    /// This is the style of the small ticks x and y
    /// </summary>

    lv_point_t graph_axis_small_ticks;

protected:
    /// <summary>
    /// The parent to all the axis lines to allow them to be easily managed and give the size through styles
    /// </summary>
    lv_obj_t* graphAxisParent;

    /// <summary>
    /// This is the parent of the line plot that make the parent and give the size through styles
    /// </summary>
    lv_obj_t* graphPlotParent;

    /// <summary>
    /// This is the parent of the current line bar and give the size through styles
    /// </summary>
    lv_obj_t* graphCurrentLineParent;

    /// <summary>
    /// The total number of points to be drawn in the graph
    /// </summary>
    uint16_t plotNumberOfPoints;

    /// <summary>
    /// These are the display limits for the graph it should not be able to scale any smaller than this in Y axis
    /// </summary>
    int16_t graphYAxisMinLimit;

    /// <summary>
    /// These are the display limits for the graph it should not be able to scale any larger than this in Y axis
    /// </summary>
    int16_t graphYAxisMaxLimit;
    
    /// <summary>
    /// The X axis size is fixed in terms of tick sizes
    /// </summary>
    uint16_t xAxisTickStep;

    /// <summary>
    /// The X axis measure between X points on the graph
    /// </summary>
    uint16_t xAxisPointStep;

    /// <summary>
    /// This is the current reading multiplied by the wheel rotations per min multiplier
    /// </summary>
    float currentMaxYAxisValue;

    /// <summary>
    /// This is the current graphed maximum displayed in multipled wheel rotations
    /// </summary>
    float loggerMaxYAxisValue;

    /// <summary>
    /// This is required to detect that the axis is required to be redrawn from scratch
    /// </summary>
    bool updateAxis;

    /// <summary>
    /// This is required to detect that the plot graph is required to be redrawn from scratch
    /// </summary>
    bool updatePlotGraph;

    /// <summary>
    /// This is required to detect that the current line is required to be redrawn from scratch
    /// </summary>
    bool updateCurrentGraph;

protected:
    /// <summary>
    /// The current wheel rotation per min reading
    /// </summary>
    float currentValue;

    /// <summary>
    /// The current speed to display
    /// </summary>
    uint16_t currentMultipliedWheelRotationsPerMin;

protected:
    /// <summary>
    /// This will attempt to destroy all the Lv Objects associated with the instance and sub components associcated with it
    /// </summary>
    virtual void destroyLvObj();

private:
    /// <summary>
    /// This is the graph of the current level of speed
    /// </summary>
    GraphPlot currentGraph;

    /// <summary>
    /// This is the graph of the values over time
    /// </summary>
    GraphPlot plotGraph;

    /// <summary>
    /// This is the graph axis for both current and plot
    /// </summary>
    GraphAxis graphAxis;

public:
    /// <summary>
    /// Constructor for the SpeedGraphMonitorMain
    /// </summary>
    GraphMonitor(lv_point_t graphPlotOffset, uint16_t plotNumberOfPoints, AxisType axisType, uint16_t xAxisPointStep, uint16_t xAxisTickStep, uint16_t maxXAxisTicks, uint16_t maxAxisTicksY, int16_t graphYAxisMinLimit, int16_t graphYAxisMaxLimit);

    /// <summary>
    /// Returns the LV object instance to represent this class instance
    /// </summary>
    /// <returns>The LV object instance to represent this class instance</returns>
    virtual lv_obj_t* createLvObj(lv_obj_t* parent);

    /// <summary>
    /// For the layout we don't have any actual lv_objects to update
    /// </summary>
    virtual void updateLvObj();

    /// <summary>
    /// Has to be defined but not used as the object should never get focus
    /// </summary>
    /// <param name="defocusLvObj"></param>
    virtual void focusLvObj(BaseLvObject* defocusLvObj) {}

    /// <summary>
    /// This will ensure the stats are initialised and the correct interest for the monitor is assigned to the controller
    /// </summary>
    virtual void initGraphs();

    /// <summary>
    /// Updated the graph
    /// </summary>
    void updatePlot(GraphMonitorPointsIterator* it);

    /// <summary>
    /// The limits of the graph from x min which is the left of the graph and x max to the right
    /// </summary>
    /// <param name="min">min x</param>
    /// <param name="max">max x</param>
    void setXLimits(int16_t min, int16_t max);

    /// <summary>
    /// The limits of the graph from y min which is the bottom of the graph and y max to the top
    /// </summary>
    /// <param name="min"></param>
    /// <param name="max"></param>
    void setYLimits(int16_t min, int16_t max);

    /// <summary>
    /// Returns the graph current maxima values
    /// </summary>
    /// <returns></returns>
    GraphPoint* getLmitsMax() { return this->plotGraph.getLimitsMax(); }

    /// <summary>
    /// Returns the current maxima display for the plot
    /// </summary>
    /// <returns></returns>
    int16_t getXMax() { return this->plotGraph.getXMax(); }

    /// <summary>
    /// Returns the current maxima display for the plot
    /// </summary>
    /// <returns></returns>
    int16_t getYMax() { return this->plotGraph.getYMax(); }

    /// <summary>
    /// Returns the current maxima display for the plot
    /// </summary>
    /// <returns></returns>
    int16_t getXMin() { return this->plotGraph.getXMin(); }

    /// <summary>
    /// Returns the current maxima display for the plot
    /// </summary>
    /// <returns></returns>
    int16_t getYMin() { return this->plotGraph.getYMin(); }

    /// <summary>
    /// Returns the graph current minima values
    /// </summary>
    /// <returns></returns>
    GraphPoint* getLmitsMin() { return this->plotGraph.getLimitsMax(); }

    /// <summary>
    /// The position of the Y axis position in along the XAxis
    /// </summary>
    /// <param name="x">the position of the X axis</param>
    void setAxisXPos(int16_t x) { this->graphAxis.setAxisXPos(x); this->updateAxis = true; };
    
    /// <summary>
    /// The position of the Y axis
    /// </summary>
    /// <param name="x">the position of the Y axis</param>
    void setAxisYPos(int16_t x) { this->graphAxis.setAxisYPos(x); this->updateAxis = true; };

    /// <summary>
    /// Setting the position of both the axis X and Y
    /// </summary>
    /// <param name="pos">The position of the axis</param>
    void setAxisPos(GraphAxisPoint* axisPos) { this->graphAxis.setAxisPos(axisPos); this->updateAxis = true; };

    /// <summary>
    /// Setting the sizes of the ticks Y ticks
    /// </summary>
    /// <param name="minorTickSize"></param>
    /// <param name="minorTicksPerMajorTick"></param>
    void setYTickSizes(float minorTickSize, uint16_t minorTicksPerMajorTick);

    /// <summary>
    /// Setting the sizes of the ticks in X ticks
    /// </summary>
    /// <param name="minorTickSize"></param>
    /// <param name="minorTicksPerMajorTick"></param>
    void setXTickSizes(float minorTickSize, uint16_t minorTicksPerMajorTick);

    /// <summary>
    /// Ensure the current graph is updated
    /// </summary>
    void updateCurrent(int16_t currentValue);

    /// <summary>
    /// Controls if the current level line is visable
    /// </summary>
    /// <param name="visible"></param>
    void setCurrentVisiblitiy(bool visible);

    /// <summary>
    /// Sets the line styles
    /// </summary>
    /// <param name="graph_line_style"></param>
    void setGraphLineStyles(lv_style_t* graph_plot_style, lv_style_t* graph_current_style, lv_style_t* graph_axis_style, lv_point_t* graph_axis_large_ticks, lv_point_t* graph_axis_small_ticks) {
        this->graph_plot_style = graph_plot_style; this->graph_current_style = graph_current_style; this->graph_axis_style = graph_axis_style; 
        this->graph_axis_large_ticks = *graph_axis_large_ticks;
        this->graph_axis_small_ticks = *graph_axis_small_ticks;
    }
};

#endif