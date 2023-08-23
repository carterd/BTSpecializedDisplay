#ifndef _MONITOR_GRAPH_H
#define _MONITOR_GRAPH_H

#include <Arduino.h>
#include "..\BaseLvObject.h"
#include <lvgl.h>

typedef struct {
    int32_t x;
    int32_t y;
} GraphPoint;

class GraphPlot : public BaseLvObject {
private:
    /// <summary>
    /// The parent container objcet
    /// </summary>
    lv_obj_t* parent_obj;
    /// <summary>
    /// This is the parent of the lines that make the parent
    /// </summary>
    lv_obj_t* graphLinesParent;
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
    /// <summary>
    /// This is how much each pixel is in terms of xMin and xMax
    /// </summary>
    float xMultiplier;
    /// <summary>
    /// This is how much each pixel is in terms of yMin and yMax
    /// </summary>
    float yMultiplier;

protected:
    /// <summary>
    /// This is used to update the X multipler on changes to any xMin xMax values
    /// </summary>
    void updateXMultiplier() { this->xMultiplier = ((float)(this->graphMax.x - this->graphMin.x)) / this->width; }

    /// <summary>
    /// This is used to update the Y multipler on changes to any yMin yMax values
    /// </summary>
    void updateYMultiplier() { this->yMultiplier = ((float)(this->graphMin.y - this->graphMax.y)) / this->height; }

public:
    /// <summary>
    /// </summary>
    GraphPlot(uint16_t numberOfLines, uint16_t pointsInLine);

    /// <summary>
    /// This will attempt to destroy all the Lv Objects associated with the instance and sub components associcated with it
    /// </summary>
    virtual ~GraphPlot();

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
    void setLimits(GraphPoint *min, GraphPoint *max) { this->graphMax = *max; this->graphMin = *min; this->updateXMultiplier(); this->updateYMultiplier(); }
    /// <summary>
    /// Set a new set of graph limits
    /// </summary>
    void setLimits(int32_t xMin, int32_t xMax, int32_t yMin, int32_t yMax) { this->graphMin.x = xMin; this->graphMax.x = xMax; this->updateXMultiplier(); this->graphMin.y = yMin; this->graphMax.y = yMax; this->updateYMultiplier();
    }
    /// <summary>
    /// Set a new set of graph limits minimums
    /// </summary>
    void setLimitsMin(GraphPoint *min) { this->graphMin = *min; this->updateXMultiplier(); this->updateYMultiplier(); }
    /// <summary>
    /// Set a new set of graph limits maximums
    /// </summary>
    void setLimitsMax(GraphPoint *max) { this->graphMax = *max; this->updateXMultiplier(); this->updateYMultiplier(); }
    /// <summary>
    /// Set a new set of x minimum limit
    /// </summary>
    void setXmin(int32_t xMin) { this->graphMin.x = xMin; this->updateXMultiplier(); }
    /// <summary>
    /// Set a new set of y minimum limit
    /// </summary>
    void setYmin(int32_t yMin) { this->graphMin.y = yMin; this->updateYMultiplier(); }
    /// <summary>
    /// Set a new set of x maximum limit
    /// </summary>    
    void setXmax(int32_t xMax) { this->graphMax.x = xMax; this->updateXMultiplier(); }
    /// <summary>
    /// Set a new set of y maximum limit
    /// </summary>
    void setYmax(int32_t yMax) { this->graphMax.y = yMax; this->updateYMultiplier(); }
    /// <summary>
    /// Get the given graph point with a new x,y value, required to calculate which line point blongs to.
    /// </summary>
    /// <param name="pointIndex">index of the point in the array of all points</param>
    /// <param name="graphPoint">new value</param>
    void setGraphPoint(uint16_t pointIndex, GraphPoint* graphPoint) { if (pointIndex >= 0 && pointIndex < this->numberOfPoints) { this->graphPoints[pointIndex] = *graphPoint; } }
    /// <summary>
    /// This sets the graph point of a given line with new x, y value,
    /// </summary>
    /// <param name="lineIndex">Which line does the point belong to</param>
    /// <param name="pointIndex">The index of the line point</param>
    /// <param name="graphPoint">The line point to set the x, y value to</param>
    void setGraphPoint(uint16_t lineIndex, uint16_t pointIndex, GraphPoint* graphPoint) { setGraphPoint(lineIndex*this->pointsInLine + pointIndex, graphPoint); }
    /// <summary>
    /// Sets the graph point of a given line with new x, y value
    /// </summary>
    /// <param name="pointIndex">index of the point in the array of all points</param>
    /// <param name="x">new x position of point</param>
    /// <param name="y">new y position of point</param>
    void setGraphPoint(uint16_t pointIndex, uint32_t x, uint32_t y) { if (pointIndex >= 0 && pointIndex < this->numberOfPoints) { this->graphPoints[pointIndex].x = x; this->graphPoints[pointIndex].y = y; } }
    void setGraphPoint(uint16_t lineIndex, uint16_t pointIndex, uint32_t x, uint32_t y) { setGraphPoint(lineIndex * this->pointsInLine + pointIndex, x, y); }
    /// <summary>
    /// This sets the graph point of a given line with new x, y value,
    /// </summary>
    /// <param name="lineIndex">Which line does the point belong to</param>
    /// <param name="pointIndex">The index of the line point</param>
    /// <param name="x">The points new x value</param>
    /// <param name="y">The points new x value</param>
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

    int16_t getXpixelOffset(int32_t xPos) { return (xPos - this->graphMin.x) / xMultiplier; }

    int16_t getYpixelOffset(int32_t yPos) { return (yPos - this->graphMax.y) / yMultiplier; }

    uint16_t getWidth() { return this->width; }

    uint16_t getHeight() { return this->height; }

    int32_t getXMin() { return this->graphMin.x; }

    int32_t getYMin() { return this->graphMin.y; }

    int32_t getXMax() { return this->graphMax.x; }

    int32_t getYMax() { return this->graphMax.y; }
};

#endif