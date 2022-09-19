#include <Arduino.h>
#include "LedError.h"

void led_error(int value) {
  // Initialise LED to display issues
  pinMode(LED_BUILTIN, OUTPUT);
  while (true) {
    for (int i = 0; i <= value; i++) {
      digitalWrite(LED_BUILTIN, HIGH);  
      delay(LED_ERROR_WAIT_MS);
      digitalWrite(LED_BUILTIN, LOW);
      delay(i == value ? LED_ERROR_WAIT_MS : LED_ERROR_WAIT_MS * 4);
    }
  }
}
