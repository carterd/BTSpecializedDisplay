#include <cmath>
#include "GraphAxis.h"

GraphAxis::GraphAxis(GraphPlot* graphPlot, lv_point_t graphPlotOffset, uint16_t tickPointsOnXAxis, uint16_t tickPointsOnYAxis, AxisType axisType) {
    this->graphPlot = graphPlot;
    this->graphPlotOffset = graphPlotOffset;
    this->tickPointsOnXAxis = tickPointsOnXAxis;
    this->tickPointsOnYAxis = tickPointsOnYAxis;
    this->axisType = axisType;

    this->graph_x_axis_tick_points = new lv_point_t[this->tickPointsOnXAxis*2];
    this->graph_y_axis_tick_points = new lv_point_t[this->tickPointsOnYAxis*2];
}

lv_obj_t* GraphAxis::createLvObj(lv_obj_t* parent) {
    this->width = lv_obj_get_width(parent);
    if (this->width == 0) { this->width = 1; }
    this->height = lv_obj_get_height(parent);
    if (this->height == 0) { this->height = 1; }

    //  Create a lineand apply the new style
    this->this_obj = lv_obj_create(parent);
    lv_obj_set_size(this->this_obj, this->width, this->height);
    lv_obj_update_layout(this->this_obj);

    lv_obj_t* graphXAxisLine_obj = lv_line_create(this->this_obj);    
    lv_line_set_points(graphXAxisLine_obj, this->graph_x_axis_points, 2);
    lv_obj_t* graphYAxisLine_obj = lv_line_create(this->this_obj);
    lv_line_set_points(graphYAxisLine_obj, this->graph_y_axis_points, 2);

    if (this->graph_axis_line_style) { 
        lv_obj_add_style(graphXAxisLine_obj, this->graph_axis_line_style, LV_PART_MAIN);
        lv_obj_add_style(graphYAxisLine_obj, this->graph_axis_line_style, LV_PART_MAIN);
    }

    this->x_ticks_parent_obj = lv_obj_create(this->this_obj);
    lv_obj_set_size(this->x_ticks_parent_obj, this->width, this->height);
    for (int i = 0; i < this->tickPointsOnXAxis; i++) {
        lv_obj_t* graphXAxisTick_obj = lv_line_create(this->x_ticks_parent_obj);
        lv_line_set_points(graphXAxisTick_obj, &this->graph_x_axis_tick_points[i*2], 2);
        lv_obj_add_flag(graphXAxisTick_obj, LV_OBJ_FLAG_HIDDEN);
        if (this->graph_axis_line_style) {
            lv_obj_add_style(graphXAxisTick_obj, this->graph_axis_line_style, LV_PART_MAIN);
        }
    }

    this->y_ticks_parent_obj = lv_obj_create(this->this_obj);
    lv_obj_set_size(this->y_ticks_parent_obj, this->width, this->height);
    for (int i = 0; i < this->tickPointsOnYAxis; i++) {
        lv_obj_t* graphYAxisTick_obj = lv_line_create(this->y_ticks_parent_obj);
        lv_line_set_points(graphYAxisTick_obj, &this->graph_y_axis_tick_points[i * 2], 2);
        lv_obj_add_flag(graphYAxisTick_obj, LV_OBJ_FLAG_HIDDEN);
        if (this->graph_axis_line_style) {
            lv_obj_add_style(graphYAxisTick_obj, this->graph_axis_line_style, LV_PART_MAIN);
        }
    }

    // UpdateMuliplier
    return parent;
}

void GraphAxis::focusLvObj(BaseLvObject* defocusLvObj) {
    // No real focus on a graph object
}

void GraphAxis::updateXTicksLvObj() {
    if (this->xGraphTickSize.minorTickSize) {
        int16_t yAxis = this->graphPlot->getYpixelOffset(this->axisPos.y) + this->graphPlotOffset.y;
        int16_t xTicCount = std::floor(this->graphPlot->getXMin() / this->xGraphTickSize.minorTickSize);
        float xTic = xTicCount * this->xGraphTickSize.minorTickSize;
        if (xTic < this->graphPlot->getXMin()) {
            xTic += this->xGraphTickSize.minorTickSize;
            xTicCount++;
        }
        int16_t graphXMax = this->graphPlot->getXMax();
        for (int i = 0; i < this->tickPointsOnXAxis; i++) {
            lv_obj_t* graphXAxisTick_obj = lv_obj_get_child(this->x_ticks_parent_obj, i);            
            if (xTic > graphXMax) {
                lv_obj_add_flag(graphXAxisTick_obj, LV_OBJ_FLAG_HIDDEN);
            }
            else
            {
                int pointIndex = i * 2;
                lv_obj_clear_flag(graphXAxisTick_obj, LV_OBJ_FLAG_HIDDEN);
                int16_t xAxis = this->graphPlot->getXpixelOffset(xTic) + this->graphPlotOffset.x;
                int16_t tickSize = (this->xGraphTickSize.minorTicksPerMajorTick && (xTicCount % this->xGraphTickSize.minorTicksPerMajorTick)) ? this->smallTicksSize.y : this->largeTicksSize.y;
                int16_t yAxisTickBot = yAxis + tickSize;
                int16_t yAxisTickTop = yAxis - tickSize;
                switch (this->axisType) {
                case AxisType::XaxisBottomYaxisLeft:
                case AxisType::XaxisBottomYaxisMid:
                case AxisType::XaxisBottomYaxisRight:
                    yAxisTickTop = yAxis;
                    break;
                case AxisType::XaxisTopYaxisLeft:
                case AxisType::XaxisTopYaxisMid:
                case AxisType::XaxisTopYaxisRight:
                    yAxisTickBot = yAxis;
                    break;
                }
                this->graph_x_axis_tick_points[pointIndex].x = xAxis;
                this->graph_x_axis_tick_points[pointIndex].y = yAxisTickTop;
                this->graph_x_axis_tick_points[pointIndex+1].x = xAxis;
                this->graph_x_axis_tick_points[pointIndex+1].y = yAxisTickBot;
                lv_line_set_points(graphXAxisTick_obj, &this->graph_x_axis_tick_points[pointIndex], 2);
            }
            xTic += this->xGraphTickSize.minorTickSize;
            xTicCount++;
        }
    }
}

void GraphAxis::updateYTicksLvObj() {
    if (this->yGraphTickSize.minorTickSize) {
        int16_t xAxis = this->graphPlot->getXpixelOffset(this->axisPos.x) + this->graphPlotOffset.x;
        int16_t yTicCount = std::floor(this->graphPlot->getYMin() / this->yGraphTickSize.minorTickSize);
        float yTic = yTicCount * this->yGraphTickSize.minorTickSize;
        if (yTic < this->graphPlot->getYMin()) {
            yTic += this->yGraphTickSize.minorTickSize;
            yTicCount++;
        }
        int16_t graphYMax = this->graphPlot->getYMax();
        for (int i = 0; i < this->tickPointsOnYAxis; i++) {
            lv_obj_t* graphYAxisTick_obj = lv_obj_get_child(this->y_ticks_parent_obj, i);
            if (yTic > graphYMax) {
                lv_obj_add_flag(graphYAxisTick_obj, LV_OBJ_FLAG_HIDDEN);
            }
            else
            {
                int pointIndex = i * 2;
                lv_obj_clear_flag(graphYAxisTick_obj, LV_OBJ_FLAG_HIDDEN);
                int16_t yAxis = this->graphPlot->getYpixelOffset(yTic) + this->graphPlotOffset.y;
                int16_t tickSize = (this->yGraphTickSize.minorTicksPerMajorTick && (yTicCount % this->yGraphTickSize.minorTicksPerMajorTick)) ? this->smallTicksSize.x : this->largeTicksSize.x;
                int16_t xAxisTickRight = xAxis + tickSize;
                int16_t xAxisTickLeft = xAxis - tickSize;
                switch (this->axisType) {
                case AxisType::XaxisTopYaxisLeft:
                case AxisType::XaxisMidYaxisLeft:
                case AxisType::XaxisBottomYaxisLeft:
                    xAxisTickRight = xAxis;
                    break;
                case AxisType::XaxisTopYaxisRight:
                case AxisType::XaxisMidYaxisRight:
                case AxisType::XaxisBottomYaxisRight:
                    xAxisTickLeft = xAxis;
                    break;
                }
                this->graph_y_axis_tick_points[pointIndex].x = xAxisTickLeft;
                this->graph_y_axis_tick_points[pointIndex].y = yAxis;
                this->graph_y_axis_tick_points[pointIndex + 1].x = xAxisTickRight;
                this->graph_y_axis_tick_points[pointIndex + 1].y = yAxis;
                lv_line_set_points(graphYAxisTick_obj, &this->graph_y_axis_tick_points[pointIndex], 2);
            }
            yTic += this->yGraphTickSize.minorTickSize;
            yTicCount++;
        }
    }
}

void GraphAxis::updateLvObj() {
    //this->axisPos.x = 0;
    //this->axisPos.y = this->monitorGraph->Get

    int16_t xAxis = this->graphPlot->getXpixelOffset(this->axisPos.x);
    int16_t yAxis = this->graphPlot->getYpixelOffset(this->axisPos.y);
    int16_t plotHeight = this->graphPlot->getHeight();
    int16_t plotWidth = this->graphPlot->getWidth();

    // This is the x axis
    this->graph_x_axis_points[0].x = this->graphPlotOffset.x;
    this->graph_x_axis_points[0].y = yAxis + this->graphPlotOffset.y;
    this->graph_x_axis_points[1].x = plotWidth + this->graphPlotOffset.x;
    this->graph_x_axis_points[1].y = yAxis + this->graphPlotOffset.y;
    lv_obj_t* graphXAxisLine_obj = lv_obj_get_child(this->this_obj, 0);
    lv_line_set_points(graphXAxisLine_obj, graph_x_axis_points, 2);

    // This is the y axis
    this->graph_y_axis_points[0].x = xAxis + this->graphPlotOffset.x;
    this->graph_y_axis_points[0].y = this->graphPlotOffset.y;
    this->graph_y_axis_points[1].x = xAxis + this->graphPlotOffset.x;
    this->graph_y_axis_points[1].y = plotHeight + this->graphPlotOffset.y;;
    lv_obj_t* graphYAxisLine_obj = lv_obj_get_child(this->this_obj, 1);
    lv_line_set_points(graphYAxisLine_obj, graph_y_axis_points, 2);

    this->updateXTicksLvObj();
    this->updateYTicksLvObj();
}
