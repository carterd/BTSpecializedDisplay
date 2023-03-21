#include "BaseMonitorMain.h"

lv_style_t* BaseMonitorMain::getMonitorPanelStyle(display_theme_styles_t* display_theme_styles) {
    return &(display_theme_styles->main_monitor_panel);
}