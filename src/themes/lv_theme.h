#ifndef _LV_THEME_STYLES_H
#define _LV_THEME_STYLES_H

#include <lvgl.h>

typedef struct {
    lv_coord_t xStart;
    lv_coord_t xEnd;
    lv_coord_t yStart;
    lv_coord_t yStep;
} charge_coords_vert_t;

typedef struct {
    lv_coord_t xSize;
    lv_coord_t xStep;
    lv_coord_t yPos;
} assist_coords_t;

typedef struct {
    lv_coord_t xMin;
    lv_coord_t xMax;
    lv_coord_t yPos;
} charge_coords_hor_t;

typedef struct {
        // Generic Theme Styles
        lv_style_t scr;
        lv_style_t card;
        lv_style_t no_scrollbar;
        lv_style_t scrollbar;
        lv_style_t inv;
        lv_style_t disabled;
        lv_style_t focus;
        lv_style_t edit;
        lv_style_t pad_zero;
        lv_style_t no_radius;
        lv_style_t radius_circle;

        lv_style_t window;

        lv_style_t matrix;

        lv_style_t calender;

        lv_style_t arc;
        lv_style_t arc_indicator;
        lv_style_t arc_knob;

        lv_style_t spinner;
        lv_style_t spinner_indicator;

        lv_style_t slider;
        lv_style_t slider_focus;
        lv_style_t slider_knob;
        lv_style_t slider_knob_edit;
        lv_style_t slider_indicator;

        lv_style_t checkbox;
        lv_style_t checkbox_checked;

        lv_style_t button;
        lv_style_t button_focus;
        lv_style_t button_checked;
        lv_style_t button_pressed;
        lv_style_t button_no_highlight;

        lv_style_t list;
        lv_style_t list_button;
        lv_style_t list_button_focus;
        lv_style_t list_scrollbar;
        lv_style_t slow_label;

        lv_style_t searching_label;

        // Bar Styles
        lv_style_t button_label_bar;
        lv_style_t menu_label_bar;

        // Panel Styles
        lv_style_t small_monitor_panel;
        lv_style_t medium_monitor_panel;
        lv_style_t medium_monitor_label;
        lv_style_t medium_monitor_value;
        lv_style_t main_monitor_panel;

        // Line Styles
        lv_style_t main_battery_outline;
        lv_style_t main_battery_charge_good;
        lv_style_t main_battery_charge_moderate;
        lv_style_t main_battery_charge_bad;
        lv_style_t small_assist_dot_line;
        lv_style_t small_battery_outline;
        lv_style_t small_battery_charge_good;
        lv_style_t small_battery_charge_moderate;
        lv_style_t small_battery_charge_bad;

        // Graph Styles
        lv_style_t main_graph_left_axis;
        lv_style_t main_graph_panel;

        // Images
        lv_img_dsc_t* connect_button_img;
        lv_img_dsc_t* connecting_button_img;
        lv_img_dsc_t* configure_button_img;
        lv_img_dsc_t* bluetooth_button_img;

        // Lines
        const lv_point_t* main_battery_line_points;
        const charge_coords_vert_t* main_battery_coords;
        const assist_coords_t* small_assist_coords;
        const lv_point_t* small_battery_line_points;
        const charge_coords_hor_t* small_battery_coords;

#if LV_USE_TEXTAREA
        lv_style_t ta_cursor;
#endif
} display_theme_styles_t;

/**
 * @brief This is the structure of styles used by the display
 */
extern display_theme_styles_t* display_theme_styles;
/**
 * @brief This is the displays theme which is using the styles
 */
extern lv_theme_t display_theme;
/**
 * @brief Registers if the theme has been initialised
 */
extern bool display_theme_inited;

#endif