#include <math.h>
#include <deque>
#include "GraphMonitor.h"

#define MAX_CURRENT_GRAPH_LINES 1
#define POINTS_PER_LINE 2

GraphMonitor::GraphMonitor(lv_point_t graphPlotOffset, uint16_t plotNumberOfPoints, AxisType axisType, uint16_t xAxisPointStep, uint16_t xAxisTickStep, uint16_t maxXAxisTicks, uint16_t maxYAxisTicks, int16_t graphYAxisMinLimit, int16_t graphYAxisMaxLimit) :
    plotGraph(plotNumberOfPoints, POINTS_PER_LINE),
    currentGraph(MAX_CURRENT_GRAPH_LINES, POINTS_PER_LINE),
    graphAxis(&this->plotGraph, graphPlotOffset, maxXAxisTicks, maxYAxisTicks, axisType) {
    this->plotNumberOfPoints = plotNumberOfPoints;
    this->xAxisTickStep = xAxisTickStep;
    this->xAxisPointStep = xAxisPointStep;
    this->graphYAxisMinLimit = graphYAxisMinLimit;
    this->graphYAxisMaxLimit = graphYAxisMaxLimit;
    this->currentMultipliedWheelRotationsPerMin = 0;
}

void GraphMonitor::destroyLvObj() {
    // We overload as clean only as this object is the parent not created by this class
    if (this->this_obj) lv_obj_clean(this->this_obj);
    this->this_obj = NULL;
}

lv_obj_t* GraphMonitor::createLvObj(lv_obj_t* parent) {
    this->this_obj = parent;
    lv_point_t graphPlotOffset = this->graphAxis.getGraphPlotOffset();

    //  Create a lineand apply the new style we're using the style to control the size of the graph hence update the layout
    this->graphPlotParent = lv_obj_create(this->this_obj);
    if (this->graph_plot_style) { lv_obj_add_style(this->graphPlotParent, this->graph_plot_style, LV_PART_MAIN); }    
    lv_obj_align(this->graphPlotParent, LV_ALIGN_TOP_LEFT, graphPlotOffset.x, graphPlotOffset.y);
    lv_obj_update_layout(this->graphPlotParent);
    // Configure Plot graph
    this->plotGraph.setGraphLineStyle(this->graph_plot_style);
    this->plotGraph.createLvObj(this->graphPlotParent);
    this->plotGraph.setLimits(0, 1, 0, this->graphYAxisMinLimit);

    // Create a current graph
    this->graphCurrentLineParent = lv_obj_create(this->this_obj);
    if (this->graph_current_style) { lv_obj_add_style(this->graphCurrentLineParent, this->graph_current_style, LV_PART_MAIN); }
    lv_obj_align(this->graphCurrentLineParent, LV_ALIGN_TOP_LEFT, graphPlotOffset.x, graphPlotOffset.y);
    lv_obj_update_layout(this->graphCurrentLineParent);
    // Configure Current graph Line
    this->currentGraph.setGraphLineStyle(this->graph_current_style);
    this->currentGraph.createLvObj(this->graphCurrentLineParent);
    // Start with current line not visable
    this->currentGraph.setLimits(0, 1, 0, this->graphYAxisMinLimit);
    this->currentGraph.setXLimits(0, 1);
    //this->currentGraph.setLineVisible(0);

    // Create the axis
    this->graphAxisParent = lv_obj_create(this->this_obj);
    //lv_obj_set_size(this->graphAxisParent, 4, GRAPH_HEIGHT);
    if (this->graph_axis_style) { lv_obj_add_style(this->graphAxisParent, this->graph_axis_style, LV_PART_MAIN); }
    lv_obj_set_align(this->graphAxisParent, LV_ALIGN_TOP_LEFT);
    lv_obj_update_layout(this->graphAxisParent);
    // Configure GraphAxis
    this->graphAxis.setAxisLineStyle(this->graph_axis_style, this->graph_axis_large_ticks, this->graph_axis_small_ticks);
    this->graphAxis.createLvObj(this->graphAxisParent);
    this->graphAxis.setAxisXPos(this->plotGraph.getXMax());
    this->graphAxis.setAxisYPos(0);
 
    // We can't fill all the graphs details until we've obtained a wheel size for calculation of speed
    this->updateAxis = this->updatePlotGraph = this->updateCurrentGraph = true;

    return this->this_obj;
}

void GraphMonitor::initGraphs()
{

}

void GraphMonitor::updateLvObj() {
    if (this->updateAxis) this->graphAxis.updateLvObj();
    if (this->updatePlotGraph) this->plotGraph.updateLvObj();
    if (this->updateCurrentGraph) this->currentGraph.updateLvObj();
    this->updateAxis = this->updatePlotGraph = this->updateCurrentGraph = false;
}

void GraphMonitor::updateCurrent(int16_t currentValue) {
    this->currentValue = currentValue;
    this->currentGraph.setGraphPoint(0, 0, this->currentGraph.getXMin(), currentValue);
    this->currentGraph.setGraphPoint(0, 1, this->currentGraph.getXMax(), currentValue);
    this->updateCurrentGraph = true;
}

void GraphMonitor::setCurrentVisiblitiy(bool visible) {
    // Current is always line 0
    if (visible) this->currentGraph.setLineVisible(0);
    else this->currentGraph.setLineHidden(0);
}

void GraphMonitor::setYTickSizes(float minorTickSize, uint16_t minorTicksPerMajorTick) {
    this->graphAxis.setYMinorTickSize(minorTickSize);
    this->graphAxis.setYMinorTicksPerMajorTick(minorTicksPerMajorTick);
    this->updateAxis = true;
}

void GraphMonitor::setXTickSizes(float minorTickSize, uint16_t minorTicksPerMajorTick) {
    this->graphAxis.setXMinorTickSize(minorTickSize);
    this->graphAxis.setXMinorTicksPerMajorTick(minorTicksPerMajorTick);
    this->updateAxis = true;
}

void GraphMonitor::setXLimits(int16_t min, int16_t max) {
    this->plotGraph.setXLimits(min, max);
    // Note current is not affected by the plot x limits only the y limits
    this->updateAxis = this->updatePlotGraph = true;
}

void GraphMonitor::setYLimits(int16_t min, int16_t max) {
    if (max > this->graphYAxisMaxLimit) { max = this->graphYAxisMaxLimit; }
    if (max < this->graphYAxisMinLimit) { max = this->graphYAxisMinLimit; }
    if (this->plotGraph.getYMax() != max || this->plotGraph.getYMin() != min) {
        this->plotGraph.setYLimits(min, max);
        this->currentGraph.setYLimits(min, max);
        this->updateAxis = this->updatePlotGraph = this->updateCurrentGraph = true;
    }
}

void GraphMonitor::updatePlot(GraphMonitorPointsIterator* it) {
    it->init();
    for (uint16_t lineIndex = 0; lineIndex < this->plotGraph.getNumberOfLines(); lineIndex++) {
        if (it->getNext()) {
            this->plotGraph.setGraphPoint(lineIndex, 0, &(it->lineStart));
            this->plotGraph.setGraphPoint(lineIndex, 1, &(it->lineEnd));
            this->plotGraph.setLineVisible(lineIndex);
        }
        else {
            this->plotGraph.setLineHidden(lineIndex);
        }
    }
    this->updatePlotGraph = true;
}
