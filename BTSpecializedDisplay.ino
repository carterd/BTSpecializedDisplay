#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <ButtonEncoder.h>
#include <Adafruit_LvGL_Glue.h>
#include <Adafruit_LvGL_Tick_Inc.h>
#include <DisplayCallbacks/DisplayCallback_SH110X.h>
#include <InputCallbacks/InputCallback_ButtonEncoder.h>
#include <Fonts/PixelOperator8pt7b.h>
#include <LvglThemes/lv_theme_binary.h>
#include <LvglFonts/lv_font_symbols_8.h>
#include <Utilities/Adafruit_To_LvGL_Font.h>

#include "LedError.h"
#include "src/LvglGui.h"
#include "src/dev/BluetoothBikeController.h"

#define MIN_RESET_BUTTON_PRESS_TIME_MS 10 * 1000

static ButtonEncoder encoder(16, 14, 15);
static Adafruit_SH1107 display(64, 128, &Wire, 20);


/**
 * @brief Adafruit displayGlue static
 */
static Adafruit_LvGL_Glue displayGlue;
/**
 * @brief lv_font generated from an Adafruit font static
 */
lv_font_t myAdaFont = {};
/**
 * @brief Statics kept around
 * 
 */
BluetoothBikeController* bluetoothBikeController;
ConfigStore* configStore;

bool resetButtonTest() {
  unsigned long startTime = millis();
  do {
    encoder.update();
  } while (encoder.keyPressed() && encoder.lastKeyPressed() == Encoder::EncoderKey::ENCODER_ENTER);
  if (millis() - startTime > MIN_RESET_BUTTON_PRESS_TIME_MS) return true;
  return false;
}

void setDisplayContrast() {
  display.setContrast(configStore->getDisplayConfig().contrast);
}

void setup() {
  delay(200);
  pinMode(LED_BUILTIN, OUTPUT);
  for (int i = 0; i < 10; i++ ) {
    display.begin(0x3C, true);
    display.clearDisplay();
    display.display();
    //display.dispaly();
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(10);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(10);                       // wait for a second
  }

  // Remote if no serial monitor
  //Serial.begin(9600);
  //while (!Serial);

  // Initialise display
  bool displayWorking = false;
  while (!displayWorking) {
    displayWorking = display.begin(0x3C, true); // Address 0x3C default
    digitalWrite(LED_BUILTIN, HIGH);            // turn the LED on (HIGH is the voltage level)
    if (!displayWorking) delay(2000);           // wait for a second
  };

  // Initialise display glue
  LvGLStatus result = displayGlue.begin(&display, displayCallback_SH110X, (INPUT_TYPE *) &encoder, inputCallback_ButtonEncoder, LV_INDEV_TYPE_ENCODER, true);
  if (result != LVGL_OK) {    
    led_error(result);
  }

  bluetoothBikeController = new BluetoothBikeController();
  configStore = new ConfigStore();
  bluetoothBikeController->init();
  configStore->init();

  if (resetButtonTest()) {
    configStore->defaults();
  }

  setDisplayContrast();

  lv_disp_t *display = displayGlue.getLvDisplay();
  lv_indev_t *indev = displayGlue.getLvInputDevice();
  
  // initalise the font required for the given display
  adafruitToLvGLFont(&PixelOperator8pt7b, &myAdaFont, &lv_font_symbols_8);

  // initialise the binary theme
  lv_theme_t* binary_theme = lv_theme_binary_init(displayGlue.getLvDisplay(), true, &myAdaFont);
  lv_disp_set_rotation(NULL, LV_DISP_ROT_180);
  lv_disp_set_theme(display, binary_theme);

  lvgl_setup(configStore, bluetoothBikeController, display, indev);
}

void loop() {
  // put your main code here, to run repeatedly:
  lv_task_handler(); // Call LittleVGL task handler periodically
  unsigned long start = millis();
  
  bluetoothBikeController->checkForChange();

  unsigned long end = millis();
  if (start <= end && end - start < 20) {
    delay(20 + start - end);
  }
}
