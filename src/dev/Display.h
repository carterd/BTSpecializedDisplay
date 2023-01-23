#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <Arduino_LvGL_Glue.h>

/// <summary>
/// Simple abstraction of the display device so we can quickly move from SH110X displays by putting it only into this class
/// </summary>
class Display {
public:
    DISPLAY_TYPE* display;
public:
    /// <summary>
    /// Constructor of the display takes the DISPLAY_TYPE as extracted from Arduino_LvGL_Glue.display
    /// </summary>
    Display(DISPLAY_TYPE* display) {
        this->display = display;
    }

    /// <summary>
    /// Sets the contrast of the display to the given level
    /// </summary>
    void setContrast(int contrastLevel);
};

#endif