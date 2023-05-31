#ifndef LV_USE_THEME_TDISPLAY_S3_H
#define LV_USE_THEME_TDISPLAY_S3_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <lvgl.h>
#include "display_theme.h"

/**
 * Initialize the theme
 * @param color_primary the primary color of the theme
 * @param color_secondary the secondary color for the theme
 * @param font pointer to a font to use.
 * @return a pointer to reference this theme later
 */
lv_theme_t* lv_theme_tdisplay_s3_init(lv_disp_t * disp, bool dark_bg, const lv_font_t * font);

/**
* Check if the theme is initialized
* @return true if default theme is initialized, false otherwise
*/
bool lv_theme_tdisplay_s3_is_inited(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_USE_THEME_TDISPLAY_S3_H*/
