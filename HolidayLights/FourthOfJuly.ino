// Based on Mark Kriegsman's DemoReel100.ino.

#include "FastLED.h"
#include "ArduinoSTL.h"
#include "RTClib.h"
#include <set>
#include <limits>

#define DATA_PIN    2
#define NUM_LEDS    794
//#define NUM_LEDS    103
CRGB leds[NUM_LEDS];
   
#define FRAMES_PER_SECOND  120

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
  FastLED.addLeds<WS2812B,DATA_PIN,GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(0xff);
  FastLED.setDither(DISABLE_DITHER);
  //FastLED.setDither(BINARY_DITHER);
  FastLED.setCorrection(UncorrectedColor);
  //FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setTemperature(UncorrectedTemperature);

  FastLED.clear();
}

void loop() {
  int width = NUM_LEDS / 13 / 2;
  for (int i = 0; i < width; i++) {
    int color = 0;
    for (int j = i; j < NUM_LEDS / 2; j += width, color++) {
      if (color % 2 == 0) {
        leds[j] = CRGB::Red;
      } else {
        leds[j] = CRGB::White;
      }  
    }
  }

  int blue_start = width * 13;
  for (int i = blue_start; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Blue;
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] %= 32;
  }

  int star_distance = (NUM_LEDS - blue_start) / 50;
  for (int i = blue_start + star_distance / 2; i < NUM_LEDS; i += star_distance) {
    //leds[i - 1] = CRGB::White;
    leds[i] = CRGB::White;
  }

  FastLED.delay(10 * 1000);
  //delay(5000);
}
