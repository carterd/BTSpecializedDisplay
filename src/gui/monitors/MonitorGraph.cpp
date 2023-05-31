#include "MonitorGraph.h"

#include <lvgl.h>
#include "../../themes/display_theme.h"


MonitorGraph::MonitorGraph(uint16_t numberOfLines, uint16_t pointsInLine) {
    this->numberOfLines = numberOfLines;
    this->pointsInLine = pointsInLine;
    this->numberOfPoints = numberOfLines * pointsInLine;

    this->graph_line_points = new lv_point_t[this->numberOfPoints];
    this->graphPoints = new GraphPoint[this->numberOfPoints];
}

MonitorGraph::~MonitorGraph() {
    if (this->graph_line_points) {
        delete(this->graph_line_points);
    }
    if (this->graphPoints) {
        delete(this->graphPoints);
    }
}

lv_obj_t* MonitorGraph::createLvObj(lv_obj_t* parent) {
    this->parent_obj = parent;
    this->width = lv_obj_get_width(parent);
    if (this->width == 0) { this->width = 1; }
    this->height = lv_obj_get_height(parent); 
    if (this->height == 0) { this->height = 1; }

    display_theme_styles_t* display_theme_styles = (display_theme_styles_t*)lv_disp_get_theme(lv_obj_get_disp(parent))->user_data;
    lv_style_t* main_graph_panel_style = &(display_theme_styles->main_graph_panel);

    //  Create a lineand apply the new style
    this->this_obj = lv_line_create(parent);
    lv_obj_set_size(this->this_obj, this->width, this->height);
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
void MonitorGraph::setLineVisible(uint16_t lineIndex) { 
    if (lineIndex >= 0 && lineIndex < this->numberOfLines) {  
        lv_obj_clear_flag(lv_obj_get_child(this->this_obj, lineIndex), LV_OBJ_FLAG_HIDDEN);
    }
}

void MonitorGraph::setLineHidden(uint16_t lineIndex) { 
    if (lineIndex >= 0 && lineIndex < this->numberOfLines) {  
        lv_obj_add_flag(lv_obj_get_child(this->this_obj, lineIndex), LV_OBJ_FLAG_HIDDEN);
    }
}

void MonitorGraph::updateLvObj() {
    float xMultiplier = this->graphXMax - this->graphXMin / this->width;
    float yMultiplier = this->graphYMin - this->graphYMax / this->height;
    float xOffset = -this->graphXMin;
    float yOffset = this->graphYMax;
    for (int i = 0; i < this->numberOfLines; i++) {
        // Keep calculating graph positions
        lv_point_t* lv_line_points = &this->graph_line_points[i * this->pointsInLine];
        GraphPoint* linePoint = &this->graphPoints[i * this->pointsInLine];
        for (int p = 0; p < this->pointsInLine ; p++) {
            lv_line_points[p].x = xOffset + xMultiplier * linePoint[p].x;
            lv_line_points[p].y = yOffset + yMultiplier * linePoint[p].y;            
        }
        lv_obj_t* graph_line_obj = lv_obj_get_child(this->this_obj, i);
        lv_line_set_points(graph_line_obj, lv_line_points, this->pointsInLine);
    }
}

void MonitorGraph::focusLvObj(BaseLvObject* defocusLvObj) {}