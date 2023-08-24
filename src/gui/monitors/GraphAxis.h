#ifndef _GRAPH_AXIS_H
#define _GRAPH_AXIS_H

#include <Arduino.h>
#include <lvgl.h>
#include "..\BaseLvObject.h"
#include "GraphPlot.h"

enum class AxisType { XaxisBottomYaxisLeft, XaxisBottomYaxisMid, XaxisBottomYaxisRight, XaxisMidYaxisLeft, XaxisMidYaxisMid, XaxisMidYaxisRight, XaxisTopYaxisLeft, XaxisTopYaxisMid, XaxisTopYaxisRight };

typedef struct {
    int16_t x;
    int16_t y;
} GraphAxisPoint;

typedef struct {
    float minorTickSize;
    int16_t minorTicksPerMajorTick;
} GraphTickSize;

class GraphAxis : public BaseLvObject {
private:

    lv_obj_t* x_ticks_parent_obj;

    lv_obj_t* y_ticks_parent_obj;

    /// <summary>
    /// This is the graph that is to be axised by this class
    /// </summary>
    GraphPlot* graphPlot;

    /// <summary>
    /// This is the store for the points for the y axis
    /// </summary>
    lv_point_t graph_y_axis_points[2];

    /// <summary>
    /// This is the store for the points for the x axis
    /// </summary>
    lv_point_t graph_x_axis_points[2];

    lv_style_t* graph_axis_line_style;

    /// <summary>
    /// This is the store for the point that make up the graph axis
    /// </summary>
    lv_point_t* graph_x_axis_tick_points;

    /// <summary>
    /// This is the store for the point that make up the graph axis
    /// </summary>
    lv_point_t* graph_y_axis_tick_points;

private:
    void updateXTicksLvObj();
    void updateYTicksLvObj();
protected:
    /// <summary>
    /// The position of the axis to draw in relationship to linked graph
    /// </summary>
    GraphAxisPoint axisPos;

    GraphTickSize xGraphTickSize;

    GraphTickSize yGraphTickSize;

    /// <summary>
    /// The width of the lv_object area
    /// </summary>
    uint16_t width;

    /// <summary>
    /// The height of the lv_object area
    /// </summary>
    uint16_t height;

    /// <summary>
    /// The count of tickPoints on X axis
    /// </summary>
    uint16_t tickPointsOnXAxis;

    /// <summary>
    /// THe count of tickPoints on Y axis
    /// </summary>
    uint16_t tickPointsOnYAxis;

    /// <summary>
    /// The type of axis
    /// </summary>
    AxisType axisType;

    /// <summary>
    /// This is the size of the large ticks
    /// </summary>
    lv_point_t largeTicksSize;

    /// <summary>
    /// This is the size of the small ticks
    /// </summary>
    lv_point_t smallTicksSize;

    /// <summary>
    /// This is the x y offset the plot is from the axis
    /// </summary>
    lv_point_t graphPlotOffset;

public:
    /// <summary>
    /// Constructor including number of points on each axis
    /// </summary>
    GraphAxis(GraphPlot* graphPlot, lv_point_t graphPlotOffset, uint16_t tickPointsOnXAxis, uint16_t tickPointsOnYAxis, AxisType axisType);

    /// <summary>
    /// This Creates the instance of the Lv Objects associated with the instance and sub components associated with it
    /// </summary>
    /// <param name="parent">The LV object parent on which to attach the created lv object for this instance</param>
    /// <returns>The created LV object instance</returns>
    lv_obj_t* createLvObj(lv_obj_t* parent);

    /// <summary>
    /// This means the object and any sub objects should set any groups to be in focus at this point
    /// </summary>
    void focusLvObj(BaseLvObject* defocusLvObj);

    /// <summary>
    /// Update the points on the display with the values set in the graph points array
    /// </summary>
    void updateLvObj();

    /// <summary>
    /// /// Sets the line style of the graph
    /// </summary>
    /// <param name="graph_line_style"></param>
    void setAxisLineStyle(lv_style_t* line_style, lv_point_t largeTicksSize, lv_point_t smallTicksSize) { this->graph_axis_line_style = line_style; this->largeTicksSize = largeTicksSize; this->smallTicksSize = smallTicksSize; }

    void setAxisPos(GraphAxisPoint* axisPos) { this->axisPos = *axisPos; }

    void setAxisYPos(int16_t yPos) { this->axisPos.y = yPos; }

    void setAxisXPos(int16_t xPos) { this->axisPos.x = xPos; }

    void setYGraphTickSize(GraphTickSize* axisIncrement) { this->yGraphTickSize = *axisIncrement; }

    void setXGraphTickSize(GraphTickSize* axisIncrement) { this->xGraphTickSize = *axisIncrement; }

    void setXMinorTickSize(float minorTickSize) { this->xGraphTickSize.minorTickSize = minorTickSize; }

    void setYMinorTickSize(float minorTickSize) { this->yGraphTickSize.minorTickSize = minorTickSize; }

    void setXMinorTicksPerMajorTick(uint16_t minorTicksPerMajorTick) { this->xGraphTickSize.minorTicksPerMajorTick = minorTicksPerMajorTick; }

    void setYMinorTicksPerMajorTick(uint16_t minorTicksPerMajorTick) { this->yGraphTickSize.minorTicksPerMajorTick = minorTicksPerMajorTick; }

    lv_point_t getGraphPlotOffset() { return this->graphPlotOffset;  }
};

#endif
