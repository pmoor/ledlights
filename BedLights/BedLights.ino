#include "FastLED.h"


#define DATA_PIN    3
#define NUM_LEDS    100
CRGB leds[NUM_LEDS];
float offset = 0.0;
   
#define FRAMES_PER_SECOND  60

void setup() {
  pinMode(3, OUTPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  
  FastLED.addLeds<WS2812B,DATA_PIN,GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(0xff);
  //FastLED.setDither(DISABLE_DITHER);
  FastLED.setDither(BINARY_DITHER);
  FastLED.setCorrection(UncorrectedColor);
  //FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setTemperature(UncorrectedTemperature);
}

  
void loop()
{
  int brightness = map(analogRead(0), 0, 1023, 0, 255);
  int speed = map(analogRead(1), 0, 1023, 0, 10);
  int base = map(analogRead(2), 0, 1023, 0, 255);
  int width = map(analogRead(3), 0, 1023, 1, 100);

  bool glitter = digitalRead(4) ==  HIGH;
  bool on = digitalRead(5) == HIGH;

  if (!on) {
    fadeToBlackBy(leds, NUM_LEDS, 1);
    FastLED.delay(10);
    return;
  }
  
  unsigned long time_start = millis();

  int intoffset = int(offset);
  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t hue = base + intoffset + i / width;
    leds[i] = CHSV(hue, 0xff, brightness);
  }

  if (glitter && random8() < 64) {
    leds[random16(NUM_LEDS)] = CRGB::White;
  }

  offset += speed / 10.0;

  unsigned long time_end = millis();
  if (time_end < time_start + 1000 / FRAMES_PER_SECOND) {
    FastLED.delay((time_start + 1000 / FRAMES_PER_SECOND) - time_end);
  } else {
    FastLED.show();
  }
}
