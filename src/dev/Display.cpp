#include "Display.h"

void Display::setContrast(int contrastLevel) {
    displaySetContrast(this->display, contrastLevel);
}