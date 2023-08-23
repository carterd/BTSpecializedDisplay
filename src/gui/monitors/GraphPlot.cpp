#include "GraphPlot.h"

#include <lvgl.h>
#include "../../themes/display_theme.h"


GraphPlot::GraphPlot(uint16_t numberOfLines, uint16_t pointsInLine) {
    this->numberOfLines = numberOfLines;
    this->pointsInLine = pointsInLine;
    this->numberOfPoints = numberOfLines * pointsInLine;
    this->xMultiplier = 1.0;
    this->yMultiplier = 1.0;

    this->graph_line_points = new lv_point_t[this->numberOfPoints];
    this->graphPoints = new GraphPoint[this->numberOfPoints];
}

GraphPlot::~GraphPlot() {
    if (this->graph_line_points) {
        delete(this->graph_line_points);
    }
    if (this->graphPoints) {
        delete(this->graphPoints);
    }
}

lv_obj_t* GraphPlot::createLvObj(lv_obj_t* parent) {
    this->parent_obj = parent;
    this->width = lv_obj_get_width(parent);
    if (this->width == 0) { this->width = 1; }
    this->height = lv_obj_get_height(parent); 
    if (this->height == 0) { this->height = 1; }

    //  Create a lineand apply the new style
    this->this_obj = lv_line_create(parent);
    lv_obj_set_size(this->this_obj, this->width, this->height);
    lv_obj_set_style_bg_opa(this->this_obj, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_update_layout(this->this_obj);

    // Create all the lines but initally hidden
    for (int i = 0; i < this->numberOfLines; i++) {
        lv_obj_t* graphLine = lv_line_create(this->this_obj);
        if (this->graph_line_style) { lv_obj_add_style(graphLine, this->graph_line_style, LV_PART_MAIN); }
        lv_line_set_points(graphLine, &this->graph_line_points[i * pointsInLine], pointsInLine);
        lv_obj_add_flag(graphLine, LV_OBJ_FLAG_HIDDEN);
    }

    // UpdateMuliplier
	return parent;
}

void GraphPlot::setLineVisible(uint16_t lineIndex) {
    if (lineIndex >= 0 && lineIndex < this->numberOfLines) {  
        lv_obj_clear_flag(lv_obj_get_child(this->this_obj, lineIndex), LV_OBJ_FLAG_HIDDEN);
    }
}

void GraphPlot::setLineHidden(uint16_t lineIndex) {
    if (lineIndex >= 0 && lineIndex < this->numberOfLines) {  
        lv_obj_add_flag(lv_obj_get_child(this->this_obj, lineIndex), LV_OBJ_FLAG_HIDDEN);
    }
}

void GraphPlot::updateLvObj() {
    // No updates if the graph isn't in a display form due to no data to set limits
    if (!this->xMultiplier || !this->yMultiplier) return;
    float xOffset = this->graphMin.x;
    float yOffset = this->graphMax.y;
    for (int i = 0; i < this->numberOfLines; i++) {
        // Keep calculating graph positions
        lv_point_t* lv_line_points = &this->graph_line_points[i * this->pointsInLine];
        GraphPoint* linePoint = &this->graphPoints[i * this->pointsInLine];
        for (int p = 0; p < this->pointsInLine ; p++) {
            lv_line_points[p].x = (linePoint[p].x - xOffset) / this->xMultiplier;
            lv_line_points[p].y = (linePoint[p].y - yOffset) / this->yMultiplier;
        }
        lv_obj_t* graph_line_obj = lv_obj_get_child(this->this_obj, i);
    }
    lv_obj_invalidate(this->this_obj);
}

void GraphPlot::focusLvObj(BaseLvObject* defocusLvObj) {
    // No real focus on a graph object
}