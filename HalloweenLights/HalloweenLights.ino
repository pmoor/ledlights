// Based on Mark Kriegsman's DemoReel100.ino.

#include "FastLED.h"

#define DATA_PIN    2
#define NUM_LEDS    794
//#define NUM_LEDS    103
CRGB leds[NUM_LEDS];
CHSV kHalloweenColors[4];
   
#define FRAMES_PER_SECOND 120

void setup() {
  kHalloweenColors[0] = CHSV(0, 0xff, 0xff);
  kHalloweenColors[1] = CHSV(16, 0xff, 0xff);
  kHalloweenColors[2] = CHSV(32, 0xff, 0xff);
  kHalloweenColors[3] = CHSV(240, 0xff, 0xaf);
  //kHalloweenColors[4] = CHSV(88, 0xff, 0x8f);
  
  FastLED.addLeds<WS2812B,DATA_PIN,GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(0xbb);
  FastLED.setDither(DISABLE_DITHER);
  //FastLED.setDither(BINARY_DITHER);
  //FastLED.setCorrection(UncorrectedColor);
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setTemperature(UncorrectedTemperature);
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { halloween, halloween2, halloween3 };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint16_t gFrame = 0;

void loop()
{
  unsigned long time_start = millis();

  gPatterns[gCurrentPatternNumber]();

  EVERY_N_MILLISECONDS( 50 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 30 ) { nextPattern(); } // change patterns periodically
  gFrame++;

  unsigned long time_end = millis();
  if (time_end < time_start + 1000 / FRAMES_PER_SECOND) {
    FastLED.delay((time_start + 1000 / FRAMES_PER_SECOND) - time_end);
  } else {
    FastLED.show();
  }
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
  FastLED.clear(); 
}

void halloween()
{
  fadeToBlackBy(leds, NUM_LEDS, 1);
  
  CHSV color = kHalloweenColors[(gFrame / 80) % 4];
  color.hue += 1 - random8(3);
  leds[random16(NUM_LEDS)] = color;
  color.hue += 1 - random8(3);
  leds[random16(NUM_LEDS)] = color;
  color.hue += 1 - random8(3);
  leds[random16(NUM_LEDS)] = color;
  color.hue += 1 - random8(3);
  leds[random16(NUM_LEDS)] = color;
}

void halloween2()
{
  uint8_t v = beatsin8(10, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) {
    uint8_t hue = gHue + i / 8;
    hue = (hue & 0x20) ? (hue & 0x1f) : 0x1f - (hue & 0x1f);
    leds[i] = CHSV(hue, 0xff, v);
  }
}

void halloween3()
{
  fadeToBlackBy(leds, NUM_LEDS, 64);
  if (random8(64) == 0) {
    fill_solid(leds, NUM_LEDS, CRGB::White);
  }
}

