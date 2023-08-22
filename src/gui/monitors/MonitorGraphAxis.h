#ifndef _MONITOR_GRAPH_AXIS_H
#define _MONITOR_GRAPH_AXIS_H

#include <Arduino.h>
#include <lvgl.h>
#include "..\BaseLvObject.h"
#include "MonitorGraph.h"

enum class AxisType { XaxisBottomYaxisLeft, XaxisBottomYaxisMid, XaxisBottomYaxisRight, XaxisMidYaxisLeft, XaxisMidYaxisMid, XaxisMidYaxisRight, XaxisTopYaxisLeft, XaxisTopYaxisMid, XaxisTopYaxisRight };

typedef struct {
    int32_t x;
    int32_t y;
} GraphAxisPoint;

typedef struct {
    int32_t xInc;
    int32_t yInc;
} GraphAxisIncrement;

class MonitorGraphAxis : public BaseLvObject {
private:
    /// <summary>
    /// The parent container objcet
    /// </summary>
    lv_obj_t* parent_obj;

    lv_obj_t* x_ticks_parent_obj;

    lv_obj_t* y_ticks_parent_obj;

    /// <summary>
    /// This is the graph that is to be axised by this class
    /// </summary>
    MonitorGraph* monitorGraph;

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

    GraphAxisIncrement minorTickIncrement;

    GraphAxisIncrement majorTickIncrement;

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

public:
    /// <summary>
    /// Constructor including number of points on each axis
    /// </summary>
    MonitorGraphAxis(MonitorGraph* monitorGraph, uint16_t tickPointsOnXAxis, uint16_t tickPointsOnYAxis, AxisType axisType);

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
    void setGraphAxisLineStyle(lv_style_t* line_style, lv_point_t* largeTicksSize, lv_point_t* smallTicksSize) { this->graph_axis_line_style = line_style; this->largeTicksSize = *largeTicksSize; this->smallTicksSize = *smallTicksSize; }

    void setAxisPos(GraphAxisPoint* axisPos) { this->axisPos = *axisPos; }

    void setAxisYPos(int32_t yPos) { this->axisPos.y = yPos; }

    void setAxisXPos(int32_t xPos) { this->axisPos.x = xPos; }

    void setMinorTickIncrement(GraphAxisIncrement* axisIncrement) { this->minorTickIncrement = *axisIncrement; }

    void setMajorTickIncrement(GraphAxisIncrement* axisIncrement) { this->majorTickIncrement = *axisIncrement; }

    void setXMinorTickIncrement(int32_t xInc) { this->minorTickIncrement.xInc = xInc; }

    void setYMinorTickIncrement(int32_t yInc) { this->minorTickIncrement.yInc = yInc; }

    void setXMajorTickIncrement(int32_t xInc) { this->majorTickIncrement.xInc = xInc; }

    void setYMajorTickIncrement(int32_t yInc) { this->majorTickIncrement.yInc = yInc; }

};

#endif
