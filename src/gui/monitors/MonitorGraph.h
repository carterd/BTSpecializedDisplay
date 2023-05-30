#ifndef _MONITOR_GRAPH_H
#define _MONITOR_GRAPH_H

#include <Arduino.h>
#include "..\BaseLvObject.h"
#include <lvgl.h>

typedef struct {
    float x;
    float y;
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

    uint16_t pointsInLine;

    uint16_t numberOfLines;

    uint16_t numberOfPoints;

    /// <summary>
    /// The graph Y Maximum value
    /// </summary>
    float graphYMax;
    /// <summary>
    /// The graph Y Minimum value
    /// </summary>
    float graphYMin;
    /// <summary>
    /// The graph X Maximum value
    /// </summary>
    float graphXMax;
    /// <summary>
    /// The graph X Minimum value
    /// </summary>
    float graphXMin;

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

    void updateLvObj();

    void setLimits(float xMin, float xMax, float yMin, float yMax) { this->graphXMin = xMin; this->graphXMax = yMax; this->graphYMin = yMin; this->graphYMax = yMax; }

    void setXmin(float xMin) { this->graphXMin = xMin; }

    void setYmin(float yMin) { this->graphYMin = yMin; }
    
    void setXmax(float xMax) { this->graphXMax = xMax; }

    void setYmax(float yMax) { this->graphYMax = yMax; }

    void setGraphPoint(uint16_t pointIndex, GraphPoint* graphPoint) { if (pointIndex >= 0 && pointIndex < this->numberOfPoints) { this->graphPoints[pointIndex] = *graphPoint; } }

    void setGraphPoint(uint16_t lineIndex, uint16_t pointIndex, GraphPoint* graphPoint) { setGraphPoint(lineIndex*this->pointsInLine + pointIndex, graphPoint); }

    void setGraphPoint(uint16_t pointIndex, float x, float y) { if (pointIndex >= 0 && pointIndex < this->numberOfPoints) { this->graphPoints[pointIndex].x = x; this->graphPoints[pointIndex].y = y; } }

    void setGraphPoint(uint16_t lineIndex, uint16_t pointIndex, float x, float y) { setGraphPoint(lineIndex*this->pointsInLine + pointIndex, x, y); }

    void setGraphPoints(uint16_t pointIndex, uint16_t numberOfPoints, GraphPoint* graphPoints) { memcpy(&this->graphPoints[pointIndex], graphPoints, numberOfPoints*sizeof(GraphPoint)); }

    void setGraphPoints(uint16_t lineIndex, uint16_t pointIndex, uint16_t numberOfPoints, GraphPoint* graphPoints) { setGraphPoints(lineIndex*this->pointsInLine + pointIndex, numberOfPoints, graphPoints); }

    void setLineVisible(uint16_t lineIndex);

    void setLineHidden(uint16_t lineIndex);

    void setGraphLineStyle(lv_style_t* graph_line_style) { this->graph_line_style = graph_line_style; }
};

#endif