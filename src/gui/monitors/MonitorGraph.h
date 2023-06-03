#ifndef _MONITOR_GRAPH_H
#define _MONITOR_GRAPH_H

#include <Arduino.h>
#include "..\BaseLvObject.h"
#include <lvgl.h>

typedef struct {
    int32_t x;
    int32_t y;
} GraphPoint;

class MonitorGraph : public BaseLvObject {
private:
    lv_obj_t* parent_obj;
    /// <summary>
    /// This is the parent of the lines that make the parent
    /// </summary>
    lv_obj_t* graphLinesParent;
    /// <summary>
    /// The graph ticks will depend upon mph or kmph
    /// </summary>
    uint16_t graphTicksLarge;
    /// <summary>
    /// The style of the graph line
    /// </summary>
    lv_style_t *graph_line_style;
    
    /// <summary>
    /// This is the store for the point that make up the graph axis
    /// </summary>
    lv_point_t* graph_line_points;
protected:
    /// <summary>
    /// The width of the lv_object area
    /// </summary>
    uint16_t width;
    /// <summary>
    /// The height of the lv_object area
    /// </summary>
    uint16_t height;
    /// <summary>
    /// The number of points in each of the lines
    /// </summary>
    uint16_t pointsInLine;
    /// <summary>
    /// The number of lines in the graph
    /// </summary>
    uint16_t numberOfLines;
    /// <summary>
    /// The number of lines in the graph basically pointsInLine*numberOfLines
    /// </summary>
    uint16_t numberOfPoints;
    /// <summary>
    /// The graph Maximum values
    /// </summary>
    GraphPoint graphMax;
    /// <summary>
    /// The graph Minimum values
    /// </summary>
    GraphPoint graphMin;
    /// <summary>
    /// Pointer to all the stored points of the graph
    /// </summary>
    GraphPoint* graphPoints;

public:
    /// <summary>
    /// </summary>
    MonitorGraph(uint16_t numberOfLines, uint16_t pointsInLine);

    /// <summary>
    /// This will attempt to destroy all the Lv Objects associated with the instance and sub components associcated with it
    /// </summary>
    virtual ~MonitorGraph();

    /// <summary>
    /// This Creates the instance of the Lv Objects associated with the instance and sub components associated with it
    /// </summary>
    /// <param name="parent">The LV object parent on which to attach the created lv object for this instance</param>
    /// <returns>The created LV object instance</returns>
    virtual lv_obj_t* createLvObj(lv_obj_t* parent);

    /// <summary>
    /// This means the object and any sub objects should set any groups to be in focus at this point
    /// </summary>
    virtual void focusLvObj(BaseLvObject* defocusLvObj = NULL);

    /// <summary>
    /// Update the points on the display with the values set in the graph points array
    /// </summary>
    void updateLvObj();

    /// <summary>
    /// Set a new set of graph limits
    /// </summary>
    /// <param name="min"></param>
    /// <param name="max"></param>
    void setLimits(GraphPoint min, GraphPoint max) { this->graphMax = max; this->graphMin = min; }
    /// <summary>
    /// Set a new set of graph limits
    /// </summary>
    void setLimits(int32_t xMin, int32_t xMax, int32_t yMin, int32_t yMax) { this->graphMin.x = xMin; this->graphMax.x = xMax; this->graphMin.y = yMin; this->graphMax.y = yMax; }
    /// <summary>
    /// Set a new set of graph limits
    /// </summary>
    void setLimitsMin(GraphPoint min) { this->graphMin = min; }

    void setLimitsMax(GraphPoint max) { this->graphMax = max; }

    void setXmin(int32_t xMin) { this->graphMin.x = xMin; }

    void setYmin(int32_t yMin) { this->graphMin.y = yMin; }
    
    void setXmax(int32_t xMax) { this->graphMax.x = xMax; }

    void setYmax(int32_t yMax) { this->graphMax.y = yMax; }

    void setGraphPoint(uint16_t pointIndex, GraphPoint* graphPoint) { if (pointIndex >= 0 && pointIndex < this->numberOfPoints) { this->graphPoints[pointIndex] = *graphPoint; } }

    void setGraphPoint(uint16_t lineIndex, uint16_t pointIndex, GraphPoint* graphPoint) { setGraphPoint(lineIndex*this->pointsInLine + pointIndex, graphPoint); }

    void setGraphPoint(uint16_t pointIndex, float x, float y) { if (pointIndex >= 0 && pointIndex < this->numberOfPoints) { this->graphPoints[pointIndex].x = x; this->graphPoints[pointIndex].y = y; } }

    void setGraphPoint(uint16_t lineIndex, uint16_t pointIndex, float x, float y) { setGraphPoint(lineIndex*this->pointsInLine + pointIndex, x, y); }

    void setGraphPoints(uint16_t pointIndex, uint16_t numberOfPoints, GraphPoint* graphPoints) { memcpy(&this->graphPoints[pointIndex], graphPoints, numberOfPoints*sizeof(GraphPoint)); }

    void setGraphPoints(uint16_t lineIndex, uint16_t pointIndex, uint16_t numberOfPoints, GraphPoint* graphPoints) { setGraphPoints(lineIndex*this->pointsInLine + pointIndex, numberOfPoints, graphPoints); }

    void setLineVisible(uint16_t lineIndex);

    void setLineHidden(uint16_t lineIndex);
    /// <summary>
    /// Sets the line style of the graph
    /// </summary>
    /// <param name="graph_line_style"></param>
    void setGraphLineStyle(lv_style_t* graph_line_style) { this->graph_line_style = graph_line_style; }
};

#endif