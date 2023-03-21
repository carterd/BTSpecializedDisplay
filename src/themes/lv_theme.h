#ifndef _LV_THEME_STYLES_H
#define _LV_THEME_STYLES_H

#include <lvgl.h>

typedef struct {
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

#if LV_USE_TEXTAREA
        lv_style_t ta_cursor;
#endif

        lv_style_t button_label_bar;
        lv_style_t menu_label_bar;
        lv_style_t small_monitor_panel;
        lv_style_t medium_monitor_panel;
        lv_style_t main_monitor_panel;

        lv_img_dsc_t* connect_button_img;
        lv_img_dsc_t* connecting_button_img;
        lv_img_dsc_t* configure_button_img;
        lv_img_dsc_t* bluetooth_button_img;
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