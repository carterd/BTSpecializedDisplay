/*********************
 *      INCLUDES
 *********************/
#include <lvgl.h>

#include "lv_theme_TDISPLAY.h"

#include "../gui/monitors/main/BatteryCapacityMonitorMain.h"
#include "../gui/monitors/small/MotorAssistLevelDotsMonitorSmall.h"
#include "../gui/monitors/small/BatteryCapacityImageMonitorSmall.h"
#include "../img/tdisplays3/Ride.h"
#include "../img/tdisplays3/Configure.h"
#include "../img/tdisplays3/Bluetooth.h"
#include "../img/tdisplays3/Rides.h"

 /*********************
  *      DEFINES
  *********************/

#define COLOR_FG                    dark_bg ?  lv_color_white() : lv_color_black() 
#define COLOR_BG                    dark_bg ?  lv_color_black() : lv_color_white() 
#define COLOR_RED                   lv_color_make(255, 0, 0)
#define COLOR_GREEN                 lv_color_make(0, 255, 0)
#define COLOR_BAT_GOOD              lv_color_make(128, 255, 128)
#define COLOR_BAT_MODERATE          lv_color_make(213, 213, 213)
#define COLOR_BAT_BAD               lv_color_make(255, 128, 128)
#define COLOR_BAT_OUTLINE           lv_color_white()
#define COLOR_SPEED_GRAPH           lv_color_make(128, 255, 128)
#define COLOR_SPEED_GRAPH_CURRENT   lv_color_make(255, 128, 128)
#define COLOR_SPEED_AXIS            lv_color_white()
#define COLOR_DOT                   lv_color_make(0, 255, 0)

// Card Presets
#define CARD_OUTLINE_WIDTH      0
#define CARD_OUTLINE_PAD        0
#define CARD_BORDER_WIDTH       0
#define CARD_PAD                0
#define CARD_GAP_PAD            4

// Window Presets
#define WINDOW_OUTLINE_WIDTH    1
#define WINDOW_OUTLINE_PAD      1
#define WINDOW_BORDER_WIDTH     1
#define WINDOW_PAD              1

// Matrix Presets
#define MATRIX_OUTLINE_WIDTH    0
#define MATRIX_OUTLINE_PAD      0
#define MATRIX_BORDER_WIDTH     0
#define MATRIX_PAD              4

// Arc Presets
#define ARC_WIDTH               1
#define ARC_PAD                 0
#define ARC_INDICATOR_WIDTH     3
#define ARC_INDICATOR_PAD       -1
#define ARC_KNOB_OUTLINE_WIDTH       1                           // White outline beyond the black border
#define ARC_KNOB_OUTLINE_PAD         1                           // Black border size around the knob (0=no border)
#define ARC_KNOB_BORDER_WIDTH        SLIDER_KNOB_OUTLINE_PAD     //
#define ARC_KNOB_RADIUS              LV_RADIUS_CIRCLE            // The shape of the Knob (0=square ... LV_RADIUS_CIRCLE=round)
#define ARC_KNOB_PAD                 2                          // Padding Knob makes it larger than slider

// Spinner Presets
#define SPINNER_WIDTH           16
#define SPINNER_PAD             0
#define SPINNER_INDICATOR_WIDTH 8
#define SPINNER_INDICATOR_PAD   4

// Slider Presets
#define SLIDER_OUTLINE_WIDTH            1                           // White outline beyond the black border
#define SLIDER_OUTLINE_PAD              1                           // Black border size around the slider (0=no border)
#define SLIDER_BORDER_WIDTH             SLIDER_OUTLINE_PAD          //
#define SLIDER_RADIUS                   LV_RADIUS_CIRCLE            // The shape of the slider (0=square ... LV_RADIUS_CIRCLE=round)
#define SLIDER_PAD                      0                           // Slider horizontal increase beyond knob extremes
#define SLIDER_INDICATOR_BORDER_WIDTH   1                           // Black border around indicator (0=no border)
#define SLIDER_KNOB_OUTLINE_WIDTH       1                           // White outline beyond the black border
#define SLIDER_KNOB_OUTLINE_PAD         1                           // Black border size around the knob (0=no border)
#define SLIDER_KNOB_BORDER_WIDTH        SLIDER_KNOB_OUTLINE_PAD     //
#define SLIDER_KNOB_RADIUS              LV_RADIUS_CIRCLE            // The shape of the Knob (0=square ... LV_RADIUS_CIRCLE=round)
#define SLIDER_KNOB_PAD                 0                           // Padding Knob makes it larger than slider
#define SLIDER_ANIM_SPEED                50

// CheckBox Presets
#define CHECKBOX_PAD                    -2

// Button Presets
#define BUTTON_OUTLINE_WIDTH    1
#define BUTTON_OUTLINE_PAD      1
#define BUTTON_BORDER_WIDTH     1
#define BUTTON_RADIUS           2
#define BUTTON_PAD              1
#define BUTTON_PAD_COL          4
#define BUTTON_PAD_HOR          2

// List Presets
#define LIST_OUTLINE_WIDTH      0
#define LIST_OUTLINE_PAD        0
#define LIST_BORDER_WIDTH       0
#define LIST_SCROLL_WIDTH       1
#define LIST_BUTTON_PAD_COL     4
#define LIST_BUTTON_PAD_HOR     1

// Scroll Bar Presets
#define SCROLL_WIDTH        2

#define BORDER_W_PR         0
#define BORDER_W_DIS        0
#define BORDER_W_FOCUS      0
#define BORDER_W_EDIT       10
#define PAD_DEF             4

#define LABEL_ANIM_SPEED    10
#define DEFAULT_ANIM_SPEED  1

// Graph Presets
#define GRAPH_TICKS_LARGE 5
#define GRAPH_TICKS_SMALL 2


/**********************
 *  STATIC PROTOTYPES
 **********************/
static void style_init_reset(lv_style_t* style);
static void theme_apply(lv_theme_t* th, lv_obj_t* obj);

/**********************
 *      MACROS
 **********************/

 /**********************
  *   STATIC FUNCTIONS
  **********************/

static void style_init(bool dark_bg, const lv_font_t* font)
{
    // Images
    display_theme_styles->connect_button_img = &tdisplays3_ride_img_dsc;
    display_theme_styles->connecting_button_img = &tdisplays3_ride_img_dsc;
    display_theme_styles->configure_button_img = &tdisplays3_configure_img_dsc;
    display_theme_styles->bluetooth_button_img = &tdisplays3_bluetooth_img_dsc;

    // Lines
    display_theme_styles->main_battery_line_points = BatteryCapacityMonitorMain::battery_line_points_tdisplay;
    display_theme_styles->main_battery_coords = &BatteryCapacityMonitorMain::charge_coords_tdisplay;
    display_theme_styles->small_assist_coords = &MotorAssistLevelDotMonitorSmall::assist_coords_tdisplay;
    display_theme_styles->small_battery_line_points = BatteryCapacityImageMonitorSmall::battery_line_points_tdisplay;
    display_theme_styles->small_battery_coords = &BatteryCapacityImageMonitorSmall::charge_coords_tdisplay;

    // Sizes
    display_theme_styles->main_graph_axis_large_ticks = { GRAPH_TICKS_LARGE, GRAPH_TICKS_LARGE };
    display_theme_styles->main_graph_axis_small_ticks = { GRAPH_TICKS_SMALL, GRAPH_TICKS_SMALL };

    // Screen
    style_init_reset(&display_theme_styles->scr);
    lv_style_set_bg_opa(&display_theme_styles->scr, LV_OPA_COVER);
    lv_style_set_bg_color(&display_theme_styles->scr, COLOR_BG);
    lv_style_set_text_color(&display_theme_styles->scr, COLOR_FG);
    lv_style_set_pad_row(&display_theme_styles->scr, PAD_DEF);
    lv_style_set_pad_column(&display_theme_styles->scr, PAD_DEF);
    lv_style_set_text_font(&display_theme_styles->scr, font);
    lv_style_set_shadow_opa(&display_theme_styles->scr, LV_OPA_TRANSP);
    lv_style_set_anim_speed(&display_theme_styles->scr, DEFAULT_ANIM_SPEED);

    // Inverted
    style_init_reset(&display_theme_styles->inv);
    lv_style_set_bg_opa(&display_theme_styles->inv, LV_OPA_COVER);
    lv_style_set_bg_color(&display_theme_styles->inv, COLOR_FG);
    lv_style_set_border_color(&display_theme_styles->inv, COLOR_BG);
    lv_style_set_line_color(&display_theme_styles->inv, COLOR_BG);
    lv_style_set_arc_color(&display_theme_styles->inv, COLOR_BG);
    lv_style_set_text_color(&display_theme_styles->inv, COLOR_BG);
    lv_style_set_outline_color(&display_theme_styles->inv, COLOR_BG);

    // Disabled
    style_init_reset(&display_theme_styles->disabled);
    lv_style_set_border_width(&display_theme_styles->disabled, BORDER_W_DIS);

    // Focused
    style_init_reset(&display_theme_styles->focus);
    lv_style_set_bg_opa(&display_theme_styles->focus, LV_OPA_COVER);
    lv_style_set_outline_pad(&display_theme_styles->focus, BORDER_W_FOCUS);
    lv_style_set_pad_gap(&display_theme_styles->focus, 20);

    lv_style_set_bg_color(&display_theme_styles->focus, COLOR_FG);
    lv_style_set_border_color(&display_theme_styles->focus, COLOR_BG);
    lv_style_set_line_color(&display_theme_styles->focus, COLOR_BG);
    lv_style_set_arc_color(&display_theme_styles->focus, COLOR_BG);
    lv_style_set_text_color(&display_theme_styles->focus, COLOR_BG);

    // Edit
    style_init_reset(&display_theme_styles->edit);
    lv_style_set_outline_width(&display_theme_styles->edit, BORDER_W_EDIT);

    style_init_reset(&display_theme_styles->pad_zero);
    lv_style_set_pad_all(&display_theme_styles->pad_zero, 0);
    lv_style_set_pad_gap(&display_theme_styles->pad_zero, 0);

    style_init_reset(&display_theme_styles->no_radius);
    lv_style_set_radius(&display_theme_styles->no_radius, 0);

    style_init_reset(&display_theme_styles->radius_circle);
    lv_style_set_radius(&display_theme_styles->radius_circle, LV_RADIUS_CIRCLE);


    lv_style_t* sp;

    // ScrollBar
    //---------------------------------------------------------------------------------------------

    // On ScrollBar
    sp = &display_theme_styles->scrollbar;
    style_init_reset(sp);
    lv_style_set_bg_opa(sp, LV_OPA_COVER);
    lv_style_set_bg_color(sp, COLOR_FG);
    lv_style_set_width(sp, SCROLL_WIDTH);
    // Off ScrollBar
    sp = &display_theme_styles->no_scrollbar;
    style_init_reset(sp);
    lv_style_set_width(sp, 0);


    // Card Style
    //---------------------------------------------------------------------------------------------
    sp = &display_theme_styles->card;
    style_init_reset(sp);
    lv_style_set_bg_opa(sp, LV_OPA_COVER);

    lv_style_set_text_font(sp, font);

    lv_style_set_outline_width(sp, CARD_OUTLINE_WIDTH);       // Outline width    +===================+
    lv_style_set_outline_pad(sp, CARD_OUTLINE_PAD);           // Outline pad      |                   |
    lv_style_set_border_width(sp, CARD_BORDER_WIDTH);         // Border width     |  +=============+  |
    lv_style_set_pad_all(sp, CARD_PAD);                       // Pad all          |  |             |  |
    lv_style_set_pad_gap(sp, CARD_GAP_PAD);                     // Pad gap          |  | XXXXX XXXXX |  |

    lv_style_set_bg_color(sp, COLOR_BG);
    lv_style_set_border_color(sp, COLOR_FG);
    lv_style_set_radius(sp, 0);

    lv_style_set_text_color(sp, COLOR_FG);
    lv_style_set_line_width(sp, 1);
    lv_style_set_line_color(sp, COLOR_FG);
    lv_style_set_arc_width(sp, 1);
    lv_style_set_arc_color(sp, COLOR_FG);
    lv_style_set_outline_color(sp, COLOR_FG);
    lv_style_set_anim_time(sp, 300);

    // Window Style
    //---------------------------------------------------------------------------------------------
    sp = &display_theme_styles->window;
    style_init_reset(sp);
    lv_style_set_bg_opa(sp, LV_OPA_COVER);

    lv_style_set_text_font(sp, font);

    lv_style_set_outline_width(sp, WINDOW_OUTLINE_WIDTH);       // Outline width    +===================+
    lv_style_set_outline_pad(sp, WINDOW_OUTLINE_PAD);           // Outline pad      |                   |
    lv_style_set_border_width(sp, WINDOW_BORDER_WIDTH);         // Border width     |  +=============+  |
    lv_style_set_pad_all(sp, WINDOW_PAD);                       // Pad all          |  |             |  |
    lv_style_set_pad_gap(sp, WINDOW_PAD);                       // Pad gap          |  | XXXXX XXXXX |  |

    lv_style_set_pad_column(sp, BUTTON_PAD_COL);                // Gap between button symbols and label
    lv_style_set_pad_hor(sp, BUTTON_PAD_HOR);                   // Gap between edges on list entries

    lv_style_set_radius(sp, BUTTON_RADIUS);                     // Widget Radius
    lv_style_set_border_color(sp, COLOR_BG);
    lv_style_set_outline_color(sp, COLOR_FG);
    lv_style_set_bg_color(sp, COLOR_BG);
    lv_style_set_text_color(sp, COLOR_FG);
    lv_style_set_outline_color(sp, COLOR_FG);

    // Matrix Style
    //---------------------------------------------------------------------------------------------
    sp = &display_theme_styles->matrix;
    style_init_reset(sp);
    lv_style_set_bg_opa(sp, LV_OPA_COVER);

    lv_style_set_bg_color(sp, COLOR_BG);
    lv_style_set_outline_width(sp, MATRIX_OUTLINE_WIDTH);       // Outline width    +===================+
    lv_style_set_outline_pad(sp, MATRIX_OUTLINE_PAD);           // Outline pad      |                   |
    lv_style_set_border_width(sp, MATRIX_BORDER_WIDTH);         // Border width     |  +=============+  |
    lv_style_set_pad_all(sp, MATRIX_PAD);                       // Pad all          |  |             |  |
    //lv_style_set_pad_right(sp, MATRIX_PAD - 2);
    //lv_style_set_pad_bottom(sp, MATRIX_PAD - 2);
    lv_style_set_pad_gap(sp, MATRIX_PAD);                       // Pad gap          |  | XXXXX XXXXX |  |

    // Calender Style
    //---------------------------------------------------------------------------------------------
    sp = &display_theme_styles->calender;
    style_init_reset(sp);

    lv_style_set_border_opa(sp, LV_OPA_TRANSP);
    //lv_style_set_bg_opa(sp, LV_OPA_TRANSP);
    lv_style_set_pad_all(sp, 0);
    lv_style_set_outline_width(sp, 1);       // Outline width    +===================+
    lv_style_set_outline_pad(sp, 1);           // Outline pad     |                   |   
    lv_style_set_border_width(sp, 0);         // Border width     |  +=============+  |
    lv_style_set_outline_color(sp, COLOR_FG);
    lv_style_set_bg_color(sp, COLOR_FG);

    // Spinner Style
    //---------------------------------------------------------------------------------------------
    sp = &display_theme_styles->spinner;
    style_init_reset(sp);
    lv_style_set_bg_opa(sp, LV_OPA_TRANSP);

    lv_style_set_arc_width(sp, SPINNER_WIDTH);
    lv_style_set_arc_color(sp, COLOR_GREEN);
    lv_style_set_arc_rounded(sp, true);
    lv_style_set_pad_all(sp, SPINNER_PAD);
    lv_style_set_outline_pad(sp, 20);
    lv_style_set_height(sp, 300);
    lv_style_set_width(sp, 60);
    lv_style_set_pad_top(sp, 210);

    sp = &display_theme_styles->spinner_indicator;
    style_init_reset(sp);
    lv_style_set_bg_opa(sp, LV_OPA_TRANSP);

    lv_style_set_arc_width(sp, SPINNER_INDICATOR_WIDTH);
    lv_style_set_arc_color(sp, COLOR_BG);
    lv_style_set_arc_rounded(sp, true);
    lv_style_set_pad_all(sp, SPINNER_INDICATOR_PAD);

    // Arc Style
    //---------------------------------------------------------------------------------------------
    sp = &display_theme_styles->arc;
    style_init_reset(sp);
    lv_style_set_bg_opa(sp, LV_OPA_TRANSP);

    lv_style_set_outline_width(sp, 0);                          // Outline width    +===================+
    lv_style_set_outline_pad(sp, 0);                            // Outline pad      |                   |
    lv_style_set_border_width(sp, 0);                           // Border width     |  +=============+  |
    lv_style_set_pad_all(sp, ARC_PAD);                          // Pad all          |  |             |  |

    lv_style_set_arc_width(sp, ARC_WIDTH);
    lv_style_set_arc_color(sp, COLOR_FG);
    lv_style_set_arc_rounded(sp, true);

    lv_style_set_bg_color(sp, COLOR_BG);
    lv_style_set_text_color(sp, COLOR_FG);

    // Arc Indicator
    sp = &display_theme_styles->arc_indicator;
    style_init_reset(sp);
    lv_style_set_bg_opa(sp, LV_OPA_TRANSP);

    lv_style_set_pad_all(sp, ARC_INDICATOR_PAD);

    lv_style_set_arc_width(sp, ARC_INDICATOR_WIDTH);
    lv_style_set_arc_color(sp, COLOR_FG);
    lv_style_set_arc_rounded(sp, true);

    // Arc Knob
    sp = &display_theme_styles->arc_knob;
    style_init_reset(sp);
    lv_style_set_bg_opa(sp, LV_OPA_COVER);

    lv_style_set_outline_width(sp, ARC_KNOB_OUTLINE_WIDTH);       // Outline width    +===================+
    lv_style_set_outline_pad(sp, ARC_KNOB_OUTLINE_PAD);           // Outline pad      |                   |
    lv_style_set_border_width(sp, ARC_KNOB_BORDER_WIDTH);         // Border width     |  +=============+  |
    lv_style_set_pad_all(sp, ARC_KNOB_PAD);                       // Pad all          |  |             |  |
    lv_style_set_pad_gap(sp, ARC_KNOB_PAD);                       // Pad gap          |  | XXXXX XXXXX |  |

    lv_style_set_radius(sp, SLIDER_KNOB_RADIUS);                     // Widget Radius
    lv_style_set_border_color(sp, COLOR_BG);
    lv_style_set_outline_color(sp, COLOR_FG);
    lv_style_set_bg_color(sp, COLOR_FG);
    lv_style_set_text_color(sp, COLOR_FG);
    lv_style_set_outline_color(sp, COLOR_FG);

    // Slider Style
    //---------------------------------------------------------------------------------------------
    sp = &display_theme_styles->slider;
    style_init_reset(sp);
    lv_style_set_bg_opa(sp, LV_OPA_COVER);

    lv_style_set_outline_width(sp, SLIDER_OUTLINE_WIDTH);       // Outline width    +===================+
    lv_style_set_outline_pad(sp, SLIDER_OUTLINE_PAD);           // Outline pad      |                   |
    lv_style_set_border_width(sp, SLIDER_BORDER_WIDTH);         // Border width     |  +=============+  |
    lv_style_set_pad_all(sp, SLIDER_PAD);                       // Pad all          |  |             |  |
    lv_style_set_pad_hor(sp, SLIDER_PAD);                       // Pad gap          |  | XXXXX XXXXX |  |

    lv_style_set_radius(sp, SLIDER_RADIUS);                     // Widget Radius
    lv_style_set_border_color(sp, COLOR_BG);
    lv_style_set_outline_color(sp, COLOR_FG);
    lv_style_set_bg_color(sp, COLOR_BG);
    lv_style_set_text_color(sp, COLOR_FG);
    lv_style_set_outline_color(sp, COLOR_FG);

    lv_style_set_anim_time(sp, SLIDER_ANIM_SPEED);

    // Slider Indicator
    sp = &display_theme_styles->slider_indicator;
    style_init_reset(sp);
    lv_style_set_radius(sp, SLIDER_RADIUS);                     // Widget Radius
    lv_style_set_bg_opa(sp, LV_OPA_COVER);
    lv_style_set_border_width(sp, SLIDER_INDICATOR_BORDER_WIDTH);
    lv_style_set_border_color(sp, COLOR_BG);
    lv_style_set_bg_color(sp, COLOR_FG);

    // Slider Focus
    sp = &display_theme_styles->slider_focus;
    style_init_reset(sp);
    lv_style_set_border_color(sp, COLOR_FG);

    // Slider Knob
    sp = &display_theme_styles->slider_knob;
    style_init_reset(sp);
    lv_style_set_bg_opa(sp, LV_OPA_COVER);

    lv_style_set_outline_width(sp, SLIDER_KNOB_OUTLINE_WIDTH);       // Outline width    +===================+
    lv_style_set_outline_pad(sp, SLIDER_KNOB_OUTLINE_PAD);           // Outline pad      |                   |
    lv_style_set_border_width(sp, SLIDER_KNOB_BORDER_WIDTH);         // Border width     |  +=============+  |
    lv_style_set_pad_all(sp, SLIDER_KNOB_PAD);                       // Pad all          |  |             |  |
    lv_style_set_pad_gap(sp, SLIDER_KNOB_PAD);                       // Pad gap          |  | XXXXX XXXXX |  |

    lv_style_set_radius(sp, SLIDER_KNOB_RADIUS);                     // Widget Radius
    lv_style_set_border_color(sp, COLOR_BG);
    lv_style_set_outline_color(sp, COLOR_FG);
    lv_style_set_bg_color(sp, COLOR_FG);
    lv_style_set_text_color(sp, COLOR_FG);
    lv_style_set_outline_color(sp, COLOR_FG);

    // Slider Knob Focus
    sp = &display_theme_styles->slider_knob_edit;
    style_init_reset(sp);
    lv_style_set_border_color(sp, COLOR_FG);
    lv_style_set_outline_color(sp, COLOR_BG);

    // Checkbox Style
    //---------------------------------------------------------------------------------------------
    sp = &display_theme_styles->checkbox;
    style_init_reset(sp);
    lv_style_set_bg_opa(sp, LV_OPA_COVER);

    lv_style_set_outline_width(sp, BUTTON_OUTLINE_WIDTH);       // Outline width    +===================+
    lv_style_set_outline_pad(sp, BUTTON_OUTLINE_PAD);           // Outline pad      |                   |
    lv_style_set_border_width(sp, BUTTON_BORDER_WIDTH);         // Border width     |  +=============+  |
    lv_style_set_pad_all(sp, CHECKBOX_PAD);                       // Pad all          |  |             |  |
    lv_style_set_pad_gap(sp, CHECKBOX_PAD);                       // Pad gap          |  | XXXXX XXXXX |  |

    //lv_style_set_pad_column(sp, BUTTON_PAD_COL);                // Gap between button symbols and label
    //lv_style_set_pad_hor(sp, BUTTON_PAD_HOR);                   // Gap between edges on list entries

    lv_style_set_radius(sp, BUTTON_RADIUS);                     // Widget Radius
    lv_style_set_border_color(sp, COLOR_BG);
    lv_style_set_outline_color(sp, COLOR_FG);
    lv_style_set_bg_color(sp, COLOR_BG);
    lv_style_set_text_color(sp, COLOR_FG);
    lv_style_set_outline_color(sp, COLOR_FG);

    lv_style_set_bg_img_src(sp, NULL);

    // Checkbox checked
    sp = &display_theme_styles->checkbox_checked;
    style_init_reset(sp);
    lv_style_set_bg_img_src(sp, LV_SYMBOL_OK);
    lv_style_set_text_color(sp, COLOR_BG);
    lv_style_set_bg_color(sp, COLOR_FG);
    lv_style_set_text_color(sp, COLOR_BG);

    // Button Style
    //---------------------------------------------------------------------------------------------
    sp = &display_theme_styles->button;
    style_init_reset(sp);
    lv_style_set_bg_opa(sp, LV_OPA_COVER);

    lv_style_set_outline_width(sp, BUTTON_OUTLINE_WIDTH);       // Outline width    +===================+
    lv_style_set_outline_pad(sp, BUTTON_OUTLINE_PAD);           // Outline pad      |                   |
    lv_style_set_border_width(sp, BUTTON_BORDER_WIDTH);         // Border width     |  +=============+  |
    lv_style_set_pad_all(sp, BUTTON_PAD);                       // Pad all          |  |             |  |
    lv_style_set_pad_gap(sp, BUTTON_PAD);                       // Pad gap          |  | XXXXX XXXXX |  |

    lv_style_set_pad_column(sp, BUTTON_PAD_COL);                // Gap between button symbols and label
    lv_style_set_pad_hor(sp, BUTTON_PAD_HOR);                   // Gap between edges on list entries

    lv_style_set_radius(sp, BUTTON_RADIUS);                     // Widget Radius
    lv_style_set_border_color(sp, COLOR_BG);
    lv_style_set_outline_color(sp, COLOR_FG);
    lv_style_set_bg_color(sp, COLOR_BG);
    lv_style_set_text_color(sp, COLOR_FG);
    lv_style_set_outline_color(sp, COLOR_FG);

    // Button Focus
    sp = &display_theme_styles->button_focus;
    style_init_reset(sp);
    lv_style_set_border_color(sp, COLOR_FG);

    //lv_style_set_border_width(&display_theme_styles->button, BUTTON_BORDER_WIDTH + 1);

    // Button Checked
    sp = &display_theme_styles->button_checked;
    style_init_reset(sp);
    lv_style_set_bg_color(sp, COLOR_FG);
    lv_style_set_text_color(sp, COLOR_BG);

    // Button Pressed
    sp = &display_theme_styles->button_pressed;
    style_init_reset(sp);
    lv_style_set_outline_color(sp, COLOR_BG);
    lv_style_set_border_color(sp, COLOR_FG);

    // Button No Highlight (no borders)
    sp = &display_theme_styles->button_no_highlight;
    style_init_reset(sp);
    lv_style_set_outline_width(sp, 0);                          // Outline width    +===================+
    lv_style_set_outline_pad(sp, 0);                            // Outline pad      |                   |
    lv_style_set_border_width(sp, 0);                           // Border width     |  +=============+  |
    lv_style_set_pad_all(sp, 0);                                // Pad all          |  |             |  |
    lv_style_set_pad_gap(sp, 0);                                // Pad gap          |  | XXXXX XXXXX |  |

    // List Style
    //---------------------------------------------------------------------------------------------
    sp = &display_theme_styles->list;
    style_init_reset(sp);
    lv_style_set_bg_opa(sp, LV_OPA_COVER);

    lv_style_set_outline_width(sp, LIST_OUTLINE_WIDTH);      // Outline width    +===================+
    lv_style_set_outline_pad(sp, LIST_OUTLINE_PAD);          // Outline pad      |                   |
    lv_style_set_border_width(sp, LIST_BORDER_WIDTH);        // Border width     |  +=============+  |
    lv_style_set_pad_all(sp, 4);                            // Pad all          |  |             |  |
    lv_style_set_pad_gap(sp, 0);                             // Pad gap          |  | XXXXX XXXXX |  |

    lv_style_set_radius(sp, 0);                              // Widget Radius
    lv_style_set_line_width(sp, 1);                          // Line Widths
    lv_style_set_arc_width(sp, 1);                           // Arc Widths

    lv_style_set_bg_color(sp, COLOR_BG);                     // Background Colour
    lv_style_set_outline_color(sp, COLOR_FG);                // Outline Colour
    lv_style_set_border_color(sp, COLOR_FG);                 // Border Colour       
    lv_style_set_text_color(sp, COLOR_FG);                   // Text Colour
    lv_style_set_line_color(sp, COLOR_FG);                   // Line Colour
    lv_style_set_arc_color(sp, COLOR_FG);                    // Arcs Colour

    lv_style_set_text_decor(sp, LV_TEXT_DECOR_UNDERLINE);    // Default text in list to underlined

    // List Button
    sp = &display_theme_styles->list_button;
    style_init_reset(sp);
    lv_style_set_pad_column(sp, LIST_BUTTON_PAD_COL); // Gap between button symbols and label
    lv_style_set_pad_hor(sp, LIST_BUTTON_PAD_HOR);    // Gap between edges on list entries
    lv_style_set_pad_ver(sp, 5);

    lv_style_set_text_decor(sp, LV_TEXT_DECOR_NONE);   // Default text in buttons not underlined

    // List Focus
    sp = &display_theme_styles->list_button_focus;
    style_init_reset(sp);
    lv_style_set_bg_opa(sp, LV_OPA_COVER);

    lv_style_set_bg_color(sp, COLOR_FG);        // Background Colour
    lv_style_set_outline_color(sp, COLOR_FG);   // Outline Colour
    lv_style_set_border_color(sp, COLOR_FG);    // Border Colour
    lv_style_set_text_color(sp, COLOR_BG);      // Text Colour
    lv_style_set_line_color(sp, COLOR_BG);      // Line Colour
    lv_style_set_arc_color(sp, COLOR_BG);       // Arcs Colour

    // List ScrollBar
    sp = &display_theme_styles->list_scrollbar;
    style_init_reset(sp);
    lv_style_set_bg_opa(sp, LV_OPA_COVER);

    lv_style_set_bg_color(sp, COLOR_FG);
    lv_style_set_width(sp, LIST_SCROLL_WIDTH);

    // Slow Label Style
    sp = &display_theme_styles->slow_label;
    style_init_reset(sp);
    lv_style_set_anim_speed(sp, LABEL_ANIM_SPEED);     // Specific Animation for labels

    // Searching Lable
    sp = &display_theme_styles->searching_label;
    style_init_reset(sp);
    lv_style_set_bg_opa(sp, LV_OPA_TRANSP);
    lv_style_set_text_opa(sp, LV_OPA_TRANSP);
    lv_style_set_border_opa(sp, LV_OPA_TRANSP);
    lv_style_set_outline_opa(sp, LV_OPA_TRANSP);
    lv_style_set_outline_width(sp, 0);                          // Outline width    +===================+
    lv_style_set_outline_pad(sp, 0);                            // Outline pad      |                   |
    lv_style_set_border_width(sp, 0);                           // Border width     |  +=============+  |

    // Button Label Bar
    sp = &display_theme_styles->button_label_bar;
    style_init_reset(sp);
    lv_style_set_height(sp, 30);
    lv_style_set_bg_opa(sp, LV_OPA_COVER);
    lv_style_set_bg_color(sp, COLOR_RED);
    lv_style_set_border_color(sp, COLOR_RED);
    lv_style_set_line_color(sp, COLOR_RED);
    lv_style_set_text_color(sp, COLOR_FG);
    lv_style_set_outline_color(sp, COLOR_RED);
    

    // Menu Label Bar
    sp = &display_theme_styles->menu_label_bar;
    style_init_reset(sp);
    lv_style_set_height(sp, 32);
    lv_style_set_bg_opa(sp, LV_OPA_COVER);
    lv_style_set_bg_color(sp, COLOR_RED);
    lv_style_set_border_color(sp, COLOR_RED);
    lv_style_set_line_color(sp, COLOR_RED);
    lv_style_set_text_color(sp, COLOR_FG);
    lv_style_set_outline_color(sp, COLOR_FG);
    lv_style_set_pad_hor(sp, 4);

    // Monitor Panels
    sp = &display_theme_styles->small_monitor_panel;
    style_init_reset(sp);
    lv_style_set_height(sp, 30);

    // Monitor Panels
    sp = &display_theme_styles->medium_monitor_panel;
    style_init_reset(sp);
    lv_style_set_height(sp, 90);

    sp = &display_theme_styles->medium_monitor_label;
    style_init_reset(sp);
    lv_style_set_text_font(sp, &lv_font_montserrat_28);
    lv_style_set_pad_top(sp, 48);

    sp = &display_theme_styles->medium_monitor_value;
    style_init_reset(sp);
    lv_style_set_text_font(sp, &lv_font_montserrat_48);
    lv_style_set_pad_bottom(sp, 28);

    // Monitor Panels
    sp = &display_theme_styles->main_monitor_panel;
    style_init_reset(sp);
    lv_style_set_height(sp, 210);

    // Graph Styles
    sp = &display_theme_styles->main_graph_left_axis;
    style_init_reset(sp);
    lv_style_set_height(sp, 100);
    lv_style_set_width(sp, 4);

    sp = &display_theme_styles->main_graph_panel;
    style_init_reset(sp);
    lv_style_set_height(sp, 200);
    lv_style_set_width(sp, 130);
    lv_style_set_line_width(sp, 3);
    lv_style_set_line_color(sp, COLOR_SPEED_GRAPH);
    lv_style_set_line_rounded(sp, true);

    sp = &display_theme_styles->current_graph_panel;
    style_init_reset(sp);
    lv_style_set_height(sp, 200);
    lv_style_set_width(sp, 130);
    lv_style_set_line_width(sp, 1);
    lv_style_set_line_dash_gap(sp, 3);
    lv_style_set_line_dash_width(sp, 3);
    lv_style_set_line_color(sp, COLOR_SPEED_GRAPH_CURRENT);
    lv_style_set_line_rounded(sp, true);

    sp = &display_theme_styles->main_graph_axis;
    style_init_reset(sp);
    lv_style_set_height(sp, 210);
    lv_style_set_width(sp, 135);
    lv_style_set_line_width(sp, 1);
    lv_style_set_line_color(sp, COLOR_SPEED_AXIS);
    lv_style_set_line_rounded(sp, true);

    // Main Battery display
    sp = &display_theme_styles->main_battery_outline;
    style_init_reset(sp);
    lv_style_set_line_width(sp, 4);
	lv_style_set_line_color(sp, COLOR_BAT_OUTLINE);
	lv_style_set_line_rounded(sp, true);

    sp = &display_theme_styles->main_battery_charge_good;
    style_init_reset(sp);
    lv_style_set_line_width(sp, 16);
	lv_style_set_line_color(sp, COLOR_BAT_GOOD);
	lv_style_set_line_rounded(sp, true);

    sp = &display_theme_styles->main_battery_charge_moderate;
    style_init_reset(sp);
    lv_style_set_line_width(sp, 16);
	lv_style_set_line_color(sp, COLOR_BAT_MODERATE);
	lv_style_set_line_rounded(sp, true);

    sp = &display_theme_styles->main_battery_charge_bad;
    style_init_reset(sp);
    lv_style_set_line_width(sp, 16);
	lv_style_set_line_color(sp, COLOR_BAT_BAD);
	lv_style_set_line_rounded(sp, true);
    
    sp = &display_theme_styles->small_assist_dot_line;
    style_init_reset(sp);
    lv_style_set_line_width(sp, 12);
	lv_style_set_line_color(sp, COLOR_DOT);
	lv_style_set_line_rounded(sp, true);
    
    sp = &display_theme_styles->small_battery_outline;
    style_init_reset(sp);
	lv_style_set_line_width(sp, 1);
	lv_style_set_line_color(sp, COLOR_BAT_OUTLINE);
	lv_style_set_line_rounded(sp, true);

    sp = &display_theme_styles->small_battery_charge_good;
    style_init_reset(sp);
	lv_style_set_line_width(sp, 20);
	lv_style_set_line_color(sp, COLOR_BAT_GOOD);
	lv_style_set_line_rounded(sp, false);

    sp = &display_theme_styles->small_battery_charge_moderate;
    style_init_reset(sp);
	lv_style_set_line_width(sp, 20);
	lv_style_set_line_color(sp, COLOR_BAT_MODERATE);
	lv_style_set_line_rounded(sp, false);

    sp = &display_theme_styles->small_battery_charge_bad;
    style_init_reset(sp);
	lv_style_set_line_width(sp, 20);
	lv_style_set_line_color(sp, COLOR_BAT_BAD);
	lv_style_set_line_rounded(sp, false);

#if LV_USE_TEXTAREA
    // TA Cursor
    //---------------------------------------------------------------------------------------------

    sp = &display_theme_styles->ta_cursor;
    style_init_reset(sp);
    lv_style_set_border_side(sp, LV_BORDER_SIDE_LEFT);
    lv_style_set_border_color(sp, COLOR_FG);
    lv_style_set_pad_left(sp, -1);
    lv_style_set_border_width(sp, 1);
    lv_style_set_bg_opa(sp, LV_OPA_TRANSP);
    lv_style_set_anim_time(sp, 500);
#endif
}


/**********************
 *   GLOBAL FUNCTIONS
 **********************/

bool lv_theme_tdisplay_is_inited(void)
{
    return  display_theme_styles == NULL ? false : true;
}

lv_theme_t* lv_theme_tdisplay_init(lv_disp_t* disp, bool dark_bg, const lv_font_t* font)
{
    /*This trick is required only to avoid the garbage collection of
     *styles' data if LVGL is used in a binding (e.g. Micropython)
     *In a general case styles could be in simple `static lv_style_t my_style...` variables*/
    if (!lv_theme_tdisplay_is_inited()) {
        display_theme_inited = false;
#if LVGL_VERSION_MAJOR > 8
        display_theme_styles = (display_theme_styles_t*) lv_malloc(sizeof(display_theme_styles_t));
#else
        display_theme_styles = (display_theme_styles_t*) lv_mem_alloc(sizeof(display_theme_styles_t));
#endif
    }

    display_theme.disp = disp;
    display_theme.font_small = LV_FONT_DEFAULT;
    display_theme.font_normal = LV_FONT_DEFAULT;
    display_theme.font_large = LV_FONT_DEFAULT;
    display_theme.apply_cb = theme_apply;
    display_theme.user_data = display_theme_styles;

    style_init(dark_bg, font);

    if (disp == NULL || lv_disp_get_theme(disp) != &display_theme) {
        lv_obj_report_style_change(NULL);
    }

    display_theme_inited = true;

    return (lv_theme_t*)&display_theme;
}


static void theme_apply(lv_theme_t* th, lv_obj_t* obj)
{
    LV_UNUSED(th);

    lv_obj_t* parent = lv_obj_get_parent(obj);
    if (parent == NULL) {
        lv_obj_add_style(obj, &display_theme_styles->scr, LV_PART_MAIN);
        lv_obj_add_style(obj, &display_theme_styles->scrollbar, LV_PART_SCROLLBAR);
        return;
    }

    lv_obj_t* parent_parent = lv_obj_get_parent(parent);

    if (lv_obj_check_type(obj, &lv_obj_class)) {
#if LV_USE_TABVIEW
        /*Tabview content area*/
        if (lv_obj_check_type(parent, &lv_tabview_class)) {
            return;
        }
        /*Tabview pages*/
        else if (lv_obj_check_type(parent_parent, &lv_tabview_class)) {
            lv_obj_add_style(obj, &display_theme_styles->card, LV_PART_MAIN);
            lv_obj_add_style(obj, &display_theme_styles->no_radius, LV_PART_MAIN);
            lv_obj_add_style(obj, &display_theme_styles->scrollbar, LV_PART_SCROLLBAR);
            return;
        }
#endif

#if LV_USE_WIN
        /*Header*/
        if (lv_obj_get_index(obj) == 0 && lv_obj_check_type(parent, &lv_win_class)) {
            lv_obj_add_style(obj, &display_theme_styles->card, LV_PART_MAIN);
            lv_obj_add_style(obj, &display_theme_styles->no_radius, LV_PART_MAIN);
            return;
        }
        /*Content*/
        else if (lv_obj_get_index(obj) == 1 && lv_obj_check_type(parent, &lv_win_class)) {
            lv_obj_add_style(obj, &display_theme_styles->card, LV_PART_MAIN);
            lv_obj_add_style(obj, &display_theme_styles->no_radius, LV_PART_MAIN);
            lv_obj_add_style(obj, &display_theme_styles->scrollbar, LV_PART_SCROLLBAR);
            return;
        }
#endif
        //lv_obj_add_style(obj, &display_theme_styles->card, LV_PART_MAIN);
        //lv_obj_add_style(obj, &display_theme_styles->scrollbar, LV_PART_SCROLLBAR);
    }
#if LV_USE_BTN
    else if (lv_obj_check_type(obj, &lv_btn_class)) {
        lv_obj_add_style(obj, &display_theme_styles->button, LV_PART_MAIN);
        lv_obj_add_style(obj, &display_theme_styles->button_pressed, LV_STATE_PRESSED);
        lv_obj_add_style(obj, &display_theme_styles->button_checked, LV_STATE_CHECKED);
        lv_obj_add_style(obj, &display_theme_styles->disabled, LV_STATE_DISABLED);
        lv_obj_add_style(obj, &display_theme_styles->button_focus, LV_STATE_FOCUS_KEY);
    }
#endif
#if LV_USE_BTNMATRIX
    else if (lv_obj_check_type(obj, &lv_btnmatrix_class)) {
#if LV_USE_MSGBOX
        if (lv_obj_check_type(parent, &lv_msgbox_class)) {
            lv_obj_add_style(obj, &display_theme_styles->matrix, LV_PART_MAIN);
            lv_obj_add_style(obj, &display_theme_styles->button_focus, LV_STATE_FOCUS_KEY);
            lv_obj_add_style(obj, &display_theme_styles->button, LV_PART_ITEMS);
            lv_obj_add_style(obj, &display_theme_styles->button_pressed, LV_PART_ITEMS | LV_STATE_PRESSED);
            lv_obj_add_style(obj, &display_theme_styles->button_checked, LV_PART_ITEMS | LV_STATE_CHECKED);
            lv_obj_add_style(obj, &display_theme_styles->disabled, LV_PART_ITEMS | LV_STATE_DISABLED);
            lv_obj_add_style(obj, &display_theme_styles->button_focus, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);

            return;
        }
#endif
#if LV_USE_TABVIEW
        if (lv_obj_check_type(parent, &lv_tabview_class)) {
            lv_obj_add_style(obj, &display_theme_styles->matrix, LV_PART_MAIN);
            lv_obj_add_style(obj, &display_theme_styles->button_focus, LV_STATE_FOCUS_KEY);
            lv_obj_add_style(obj, &display_theme_styles->button, LV_PART_ITEMS);
            lv_obj_add_style(obj, &display_theme_styles->button_pressed, LV_PART_ITEMS | LV_STATE_PRESSED);
            lv_obj_add_style(obj, &display_theme_styles->button_checked, LV_PART_ITEMS | LV_STATE_CHECKED);
            lv_obj_add_style(obj, &display_theme_styles->disabled, LV_PART_ITEMS | LV_STATE_DISABLED);
            lv_obj_add_style(obj, &display_theme_styles->button_focus, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
            return;
        }
#endif
#if LV_USE_CALENDAR
        if (lv_obj_check_type(parent, &lv_calendar_class)) {
            lv_obj_add_style(obj, &display_theme_styles->matrix, LV_PART_MAIN);

            //lv_obj_add_style(obj, &display_theme_styles->button_focus, LV_STATE_FOCUS_KEY);
            lv_obj_add_style(obj, &display_theme_styles->button, LV_PART_ITEMS);
            lv_obj_add_style(obj, &display_theme_styles->button_pressed, LV_PART_ITEMS | LV_STATE_PRESSED);
            lv_obj_add_style(obj, &display_theme_styles->button_checked, LV_PART_ITEMS | LV_STATE_CHECKED);
            lv_obj_add_style(obj, &display_theme_styles->card, LV_PART_ITEMS | LV_STATE_DISABLED);
            lv_obj_add_style(obj, &display_theme_styles->button_focus, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
            //lv_obj_add_style(obj, &display_theme_styles->focus, LV_PART_ITEMS | LV_STATE_DISABLED | LV_BTNMATRIX_CTRL_DISABLED);
            return;
        }
#endif
        lv_obj_add_style(obj, &display_theme_styles->matrix, LV_PART_MAIN);
        //lv_obj_add_style(obj, &display_theme_styles->button_focus, LV_STATE_FOCUS_KEY);
        lv_obj_add_style(obj, &display_theme_styles->button, LV_PART_ITEMS);
        lv_obj_add_style(obj, &display_theme_styles->button_pressed, LV_PART_ITEMS | LV_STATE_PRESSED);
        lv_obj_add_style(obj, &display_theme_styles->button_checked, LV_PART_ITEMS | LV_STATE_CHECKED);
        lv_obj_add_style(obj, &display_theme_styles->disabled, LV_PART_ITEMS | LV_STATE_DISABLED);
        lv_obj_add_style(obj, &display_theme_styles->button_focus, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
    }
#endif

#if LV_USE_BAR
    else if (lv_obj_check_type(obj, &lv_bar_class)) {

        lv_obj_add_style(obj, &display_theme_styles->button, LV_PART_MAIN);
        //lv_obj_add_style(obj, &display_theme_styles->pad_zero, 0);
        lv_obj_add_style(obj, &display_theme_styles->inv, LV_PART_INDICATOR);
        //lv_obj_add_style(obj, &display_theme_styles->focus, LV_STATE_FOCUS_KEY);
    }
#endif

#if LV_USE_SLIDER
    else if (lv_obj_check_type(obj, &lv_slider_class)) {
        lv_obj_add_style(obj, &display_theme_styles->slider, LV_PART_MAIN);
        lv_obj_add_style(obj, &display_theme_styles->slider_indicator, LV_PART_INDICATOR);
        lv_obj_add_style(obj, &display_theme_styles->slider_knob, LV_PART_KNOB);
        lv_obj_add_style(obj, &display_theme_styles->slider_focus, LV_STATE_FOCUS_KEY);
        lv_obj_add_style(obj, &display_theme_styles->slider_focus, LV_PART_INDICATOR | LV_STATE_FOCUS_KEY);
        lv_obj_add_style(obj, &display_theme_styles->slider_knob_edit, LV_PART_KNOB | LV_STATE_EDITED);
    }
#endif

#if LV_USE_TABLE
    else if (lv_obj_check_type(obj, &lv_table_class)) {
        lv_obj_add_style(obj, &display_theme_styles->matrix, LV_PART_MAIN);
        lv_obj_add_style(obj, &display_theme_styles->scrollbar, LV_PART_SCROLLBAR);
        lv_obj_add_style(obj, &display_theme_styles->scr, LV_PART_ITEMS);
        //lv_obj_add_style(obj, &display_theme_styles->no_radius, LV_PART_ITEMS);
        lv_obj_add_style(obj, &display_theme_styles->list_button_focus, LV_PART_ITEMS | LV_STATE_PRESSED);
        lv_obj_add_style(obj, &display_theme_styles->list_button_focus, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
        lv_obj_add_style(obj, &display_theme_styles->list_button_focus, LV_STATE_FOCUS_KEY);
        //lv_obj_add_style(obj, &display_theme_styles->edit, LV_STATE_EDITED);
    }
#endif

#if LV_USE_CHECKBOX
    else if (lv_obj_check_type(obj, &lv_checkbox_class)) {
        lv_obj_add_style(obj, &display_theme_styles->card, LV_PART_MAIN);
        lv_obj_add_style(obj, &display_theme_styles->checkbox, LV_PART_INDICATOR);
        lv_obj_add_style(obj, &display_theme_styles->disabled, LV_PART_INDICATOR | LV_STATE_DISABLED);
        lv_obj_add_style(obj, &display_theme_styles->checkbox_checked, LV_PART_INDICATOR | LV_STATE_CHECKED);
        lv_obj_add_style(obj, &display_theme_styles->button_pressed, LV_PART_INDICATOR | LV_STATE_PRESSED);
    }
#endif

#if LV_USE_SWITCH
    else if (lv_obj_check_type(obj, &lv_switch_class)) {
        lv_obj_add_style(obj, &display_theme_styles->slider, LV_PART_MAIN);
        lv_obj_add_style(obj, &display_theme_styles->slider_indicator, LV_PART_INDICATOR);
        lv_obj_add_style(obj, &display_theme_styles->slider_knob, LV_PART_KNOB);
        lv_obj_add_style(obj, &display_theme_styles->slider_focus, LV_STATE_FOCUS_KEY);
        lv_obj_add_style(obj, &display_theme_styles->slider_focus, LV_PART_INDICATOR | LV_STATE_FOCUS_KEY);
        lv_obj_add_style(obj, &display_theme_styles->slider_knob_edit, LV_PART_KNOB | LV_STATE_PRESSED);
    }
#endif

#if LV_USE_CHART
    else if (lv_obj_check_type(obj, &lv_chart_class)) {
        lv_obj_add_style(obj, &display_theme_styles->card, 0);
        lv_obj_add_style(obj, &display_theme_styles->scrollbar, LV_PART_SCROLLBAR);
        lv_obj_add_style(obj, &display_theme_styles->card, LV_PART_ITEMS);
        lv_obj_add_style(obj, &display_theme_styles->card, LV_PART_TICKS);
        lv_obj_add_style(obj, &display_theme_styles->card, LV_PART_CURSOR);
        lv_obj_add_style(obj, &display_theme_styles->focus, LV_STATE_FOCUS_KEY);
    }
#endif

#if LV_USE_ROLLER
    else if (lv_obj_check_type(obj, &lv_roller_class)) {
        lv_obj_add_style(obj, &display_theme_styles->list, LV_PART_MAIN);
        lv_obj_add_style(obj, &display_theme_styles->list_button_focus, LV_PART_SELECTED);
        lv_obj_add_style(obj, &display_theme_styles->list_button_focus, LV_STATE_FOCUS_KEY);
        //lv_obj_add_style(obj, &display_theme_styles->edit, LV_STATE_EDITED);
    }
#endif

#if LV_USE_DROPDOWN
    else if (lv_obj_check_type(obj, &lv_dropdown_class)) {
        lv_obj_add_style(obj, &display_theme_styles->button, LV_PART_MAIN);
        lv_obj_add_style(obj, &display_theme_styles->button_pressed, LV_STATE_PRESSED);
        lv_obj_add_style(obj, &display_theme_styles->button_focus, LV_STATE_FOCUS_KEY);
        //lv_obj_add_style(obj, &display_theme_styles->edit, LV_STATE_EDITED);
    }
    else if (lv_obj_check_type(obj, &lv_dropdownlist_class)) {
        lv_obj_add_style(obj, &display_theme_styles->list, LV_PART_MAIN);
        lv_obj_add_style(obj, &display_theme_styles->scrollbar, LV_PART_SCROLLBAR);
        lv_obj_add_style(obj, &display_theme_styles->list_button_focus, LV_PART_SELECTED | LV_STATE_CHECKED);
        lv_obj_add_style(obj, &display_theme_styles->list_button_focus, LV_PART_SELECTED | LV_STATE_PRESSED);
        lv_obj_add_style(obj, &display_theme_styles->list_button_focus, LV_STATE_FOCUS_KEY);
        //lv_obj_add_style(obj, &display_theme_styles->edit, LV_STATE_EDITED);
    }
#endif
#if LV_USE_SPINNER
    else if (lv_obj_check_type(obj, &lv_spinner_class)) {

        lv_obj_add_style(obj, &display_theme_styles->spinner, LV_PART_MAIN);
        lv_obj_add_style(obj, &display_theme_styles->spinner_indicator, LV_PART_INDICATOR);
        //lv_obj_add_style(obj, &display_theme_styles->slider_knob, LV_PART_KNOB);
        //lv_obj_add_style(obj, &display_theme_styles->slider_focus, LV_STATE_FOCUS_KEY);
        //lv_obj_add_style(obj, &display_theme_styles->slider_focus, LV_PART_INDICATOR | LV_STATE_FOCUS_KEY);
        //lv_obj_add_style(obj, &display_theme_styles->slider_knob_edit, LV_PART_KNOB | LV_STATE_EDITED);
    }
#endif
#if LV_USE_ARC
    else if (lv_obj_check_type(obj, &lv_arc_class)) {
        lv_obj_add_style(obj, &display_theme_styles->arc, LV_PART_MAIN);
        lv_obj_add_style(obj, &display_theme_styles->arc_indicator, LV_PART_INDICATOR);
        //lv_obj_add_style(obj, &display_theme_styles->slider_indicator, LV_PART_INDICATOR);
        lv_obj_add_style(obj, &display_theme_styles->arc_knob, LV_PART_KNOB);
        lv_obj_add_style(obj, &display_theme_styles->slider_focus, LV_STATE_FOCUS_KEY);
        lv_obj_add_style(obj, &display_theme_styles->slider_focus, LV_PART_INDICATOR | LV_STATE_FOCUS_KEY);
        lv_obj_add_style(obj, &display_theme_styles->slider_knob_edit, LV_PART_KNOB | LV_STATE_EDITED);

        //lv_obj_add_style(obj, &display_theme_styles->card, 0);
        //lv_obj_add_style(obj, &display_theme_styles->inv, LV_PART_INDICATOR);
        //lv_obj_add_style(obj, &display_theme_styles->pad_zero, LV_PART_INDICATOR);
        //lv_obj_add_style(obj, &display_theme_styles->card, LV_PART_KNOB);
        //lv_obj_add_style(obj, &display_theme_styles->radius_circle, LV_PART_KNOB);
        //lv_obj_add_style(obj, &display_theme_styles->focus, LV_STATE_FOCUS_KEY);
        //lv_obj_add_style(obj, &display_theme_styles->edit, LV_STATE_EDITED);
    }
#endif

#if LV_USE_METER
    else if (lv_obj_check_type(obj, &lv_meter_class)) {
        lv_obj_add_style(obj, &display_theme_styles->card, LV_PART_MAIN);
    }
#endif

#if LV_USE_TEXTAREA
    else if (lv_obj_check_type(obj, &lv_textarea_class)) {
        lv_obj_add_style(obj, &display_theme_styles->button, LV_PART_MAIN);
        lv_obj_add_style(obj, &display_theme_styles->scrollbar, LV_PART_SCROLLBAR);
        lv_obj_add_style(obj, &display_theme_styles->ta_cursor, LV_PART_CURSOR | LV_STATE_FOCUSED);
        lv_obj_add_style(obj, &display_theme_styles->button_focus, LV_STATE_FOCUSED);
        lv_obj_add_style(obj, &display_theme_styles->button_pressed, LV_STATE_EDITED);
    }
#endif

#if LV_USE_CALENDAR
    else if (lv_obj_check_type(obj, &lv_calendar_class)) {
        lv_obj_add_style(obj, &display_theme_styles->calender, LV_PART_MAIN);
        lv_obj_add_style(obj, &display_theme_styles->calender, LV_BTNMATRIX_CTRL_DISABLED);
    }
#endif

#if LV_USE_KEYBOARD
    else if (lv_obj_check_type(obj, &lv_keyboard_class)) {

        lv_obj_add_style(obj, &display_theme_styles->matrix, LV_PART_MAIN);
        //lv_obj_add_style(obj, &display_theme_styles->button_focus, LV_STATE_FOCUS_KEY);
        lv_obj_add_style(obj, &display_theme_styles->button, LV_PART_ITEMS);
        lv_obj_add_style(obj, &display_theme_styles->button_pressed, LV_PART_ITEMS | LV_STATE_PRESSED);
        lv_obj_add_style(obj, &display_theme_styles->button_checked, LV_PART_ITEMS | LV_STATE_CHECKED);
        lv_obj_add_style(obj, &display_theme_styles->disabled, LV_PART_ITEMS | LV_STATE_DISABLED);
        lv_obj_add_style(obj, &display_theme_styles->button_focus, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
    }
#endif
#if LV_USE_LIST
    // The List itself
    else if (lv_obj_check_type(obj, &lv_list_class)) {
        lv_obj_add_style(obj, &display_theme_styles->list, LV_PART_MAIN);
        lv_obj_add_style(obj, &display_theme_styles->list_scrollbar, LV_PART_SCROLLBAR);
    }
    // List Buttons itself
    else if (lv_obj_check_type(obj, &lv_list_btn_class)) {
        lv_obj_add_style(obj, &display_theme_styles->list_button, LV_STATE_DEFAULT);
        lv_obj_add_style(obj, &display_theme_styles->list_button_focus, LV_STATE_FOCUSED | LV_STATE_FOCUS_KEY);
        lv_obj_add_style(obj, &display_theme_styles->list_button_focus, LV_STATE_PRESSED);
        //lv_obj_add_style(obj, &display_theme_styles->large_border, LV_STATE_EDITED);
    }
    // List Buttons Text
    else if (lv_obj_check_type(obj, &lv_label_class) && lv_obj_check_type(parent, &lv_list_btn_class)) {
        lv_obj_add_style(obj, &display_theme_styles->slow_label, 0);
    }
    // List Text
    else if (lv_obj_check_type(obj, &lv_list_text_class)) {
    }
#endif
#if LV_USE_MSGBOX
    else if (lv_obj_check_type(obj, &lv_msgbox_class)) {
        lv_obj_add_style(obj, &display_theme_styles->window, 0);
        return;
    }
#endif
#if LV_USE_SPINBOX
    else if (lv_obj_check_type(obj, &lv_spinbox_class)) {
        lv_obj_add_style(obj, &display_theme_styles->window, 0);
        lv_obj_add_style(obj, &display_theme_styles->inv, LV_PART_CURSOR);
        lv_obj_add_style(obj, &display_theme_styles->focus, LV_STATE_FOCUS_KEY);
        lv_obj_add_style(obj, &display_theme_styles->edit, LV_STATE_EDITED);
    }
#endif
#if LV_USE_TILEVIEW
    else if (lv_obj_check_type(obj, &lv_tileview_class)) {
        lv_obj_add_style(obj, &display_theme_styles->card, 0);
        lv_obj_add_style(obj, &display_theme_styles->scrollbar, LV_PART_SCROLLBAR);
    }
    else if (lv_obj_check_type(obj, &lv_tileview_tile_class)) {
        lv_obj_add_style(obj, &display_theme_styles->scrollbar, LV_PART_SCROLLBAR);
    }
#endif

#if LV_USE_LED
    else if (lv_obj_check_type(obj, &lv_led_class)) {
        lv_obj_add_style(obj, &display_theme_styles->card, 0);
    }
#endif
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void style_init_reset(lv_style_t* style)
{
    if (display_theme_inited) {
        lv_style_reset(style);
    }
    else {
        lv_style_init(style);
    }
}
