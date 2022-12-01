#include "Display.h"

#include <Adafruit_SH110X.h>

void Display::setContrast(int contrastLevel) {
    ((Adafruit_SH1107*) this->display)->setContrast(contrastLevel);
}