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
CHSV kHalloweenColors[4];
   
#define FRAMES_PER_SECOND  120

void setup() {
  kHalloweenColors[0] = CHSV(0, 0xff, 0xff);
  kHalloweenColors[1] = CHSV(16, 0xff, 0xff);
  kHalloweenColors[2] = CHSV(32, 0xff, 0xff);
  kHalloweenColors[3] = CHSV(240, 0xff, 0xaf);
  //kHalloweenColors[4] = CHSV(88, 0xff, 0x8f);
  
  FastLED.addLeds<WS2812B,DATA_PIN,GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(0x10);
  FastLED.setDither(DISABLE_DITHER);
  //FastLED.setDither(BINARY_DITHER);
  FastLED.setCorrection(UncorrectedColor);
  //FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setTemperature(UncorrectedTemperature);
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
//SimplePatternList gPatterns = { rainbow, gradiant, rainbowWithGlitter, confetti, sinelon, juggle, bpm };
//SimplePatternList gPatterns = { halloween, halloween2, halloween3 };
SimplePatternList gPatterns = { xmas };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint16_t gFrame = 0;
  
void loop() {
  FastLED.setBrightness(0xff);
  FastLED.clear();

  int numColors = 5;
  CRGB colors[numColors];
  colors[0] = CRGB::DarkRed;
  colors[1] = CRGB::Blue;
  colors[2] = CRGB::OrangeRed;
  colors[3] = CRGB::DarkGreen;
  colors[4] = CRGB::DarkViolet;

  int spacing = 25;
  int spacingVariance = 10;

  int nextColor = random8() % numColors;
  for (int i = 1; i < NUM_LEDS - 1; i += random16(spacing - spacingVariance, spacing + spacingVariance)) {
    leds[i] = colors[nextColor];
    leds[i] >>= 1;
    leds[i - 1] = leds[i + 1] = leds[i];
    leds[i - 1] >>= 2;
    leds[i + 1] >>= 2;

    nextColor = (nextColor + random8(1, numColors)) % numColors;
  }
  
  FastLED.show();
  
  int pause = random8(6, 18);
  for (int i = 0; i < pause; i++) {
    delay(10 * 1000);
  }

  for (int i = 0; i < 100; i++) {
    fadeToBlackBy(leds, NUM_LEDS, 4);
    FastLED.show();
  }
}

void loop_old() {
  FastLED.clear();
  FastLED.setBrightness(255);

  int stencil_size = 20;
  CRGB stencil[stencil_size];
  stencil[19] = CRGB::Gray;
  stencil[18] = CRGB::White;
  stencil[17] = CRGB::White;
  for (int i = 16; i > 0; i--) {
    stencil[i] = stencil[i + 1];
    stencil[i].nscale8(192);
  }
  stencil[0] = CRGB::Black;
  
  std::vector<int> positions;
  for (int run = 0; run < 0xc00; run++) {
    int smallest_pos = INT_MAX;
    for (int i = 0; i < positions.size();) {
      int pos = positions[i];
      smallest_pos = std::min(smallest_pos, pos);

      int src_start = std::max(0, stencil_size - 1 - pos);
      int dest_start = std::max(0, pos + 1 - stencil_size );
      int length = std::min(stencil_size, std::min(pos + 1, NUM_LEDS - pos));
      if (length > 0) {
        memcpy(&leds[dest_start], &stencil[src_start], length * sizeof(CRGB));
      } else {
        // remove it
        positions[i] = positions.back();
        positions.pop_back();
        continue;
      }
      
      positions[i]++;
      i++;
    }

    FastLED.show();
   
    if ((smallest_pos > stencil_size && random8() < 5) || smallest_pos > NUM_LEDS / 3) {
      positions.push_back(0);
    }
  }
//  
//
//  float heads[6] = {-random8(), -random8(), -random8(), -random8(), -random8(), -random8()};
//  float speeds[6] = {0.25 + random8() / 340.0, 0.25 + random8() / 340.0, 0.25 + random8() / 340.0, 0.25 + random8() / 340.0, 0.25 + random8() / 340.0, 0.25 + random8() / 340.0};
//  CRGB color[6] = {CRGB::DarkRed, CRGB::Blue, CRGB::OrangeRed, CRGB::DarkGreen, CRGB::DarkViolet, CRGB::OrangeRed};
//  for (int run = 0; run < 0xc00; run++) {
//      for (int j = 0; j < 6; j++) {
//        heads[j] += speeds[j];
//
//        int pos = int(heads[j]);
//        if (pos >= 0) {
//          if (pos >= 2 * NUM_LEDS) {
//            heads[j] = -random8() / 2;
//            speeds[j] = 0.25 + random8() / 340.0;
//          } else {
//            if (pos >= NUM_LEDS) {
//              pos = 2 * NUM_LEDS - pos - 1;
//            }
//            if (run & 0x400) {
//              leds[pos] = CRGB::White;
//            } else {
//              leds[pos] = color[j];
//            }
//          }
//        }
//      }
//      FastLED.show();
//
//    fadeToBlackBy(leds, NUM_LEDS, 32);
//  }

  for (int i = 0; i < 200; i++) {
    fadeToBlackBy(leds, NUM_LEDS, 4);
    FastLED.delay(20);
  }



  
  FastLED.clear();
  FastLED.setBrightness(0x10);
  
  CRGB colors[5];
  colors[0] = CRGB::DarkRed;
  colors[1] = CRGB::Blue;
  colors[2] = CRGB::OrangeRed;
  colors[3] = CRGB::DarkGreen;
  colors[4] = CRGB::DarkViolet;

//  for (int i = 0; i < 1000; i++) {
//    leds[random16() % NUM_LEDS] = colors[random8() % 5];
//    FastLED.delay(50);
//    fadeToBlackBy(leds, NUM_LEDS, 1);
//  }

  int width = 5;

  for (int i = 0; i < NUM_LEDS; i += width) {
    for (int j = i; j < i + width && j < NUM_LEDS; j++) {
      leds[j] = colors[(i / width) % 5];
    }
    FastLED.delay(50);
  }

  delay(1000);
  FastLED.clear();
  FastLED.delay(1000);

  for (int k = 0; k < 5; k++) {
    FastLED.clear();
    FastLED.delay(300);

    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = colors[(i / width) % 5];
    }
    FastLED.delay(300);
  }

  FastLED.delay(1700);

  for (int k = 0; k < 128 * 2; k++) {
    int offset = (int(sin8(2 * k)) - 128) / 8;
//    int offset = (k / 2 + 16) % 48;
    //if (offset >= 24) offset = 48 - offset;

    int start = std::max(-offset, 0);
    int end = std::min(NUM_LEDS, NUM_LEDS - offset);
    for (int i = 0; i < start; i++) {
      leds[i] = CRGB::Gray;
    }
    for (int i = start; i < end; i++) {
      leds[i] = colors[((i + offset) / width) % 5];
    }
    for (int i = end; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Gray;
    }
    FastLED.show();
  }

  for (int k = 0; k < NUM_LEDS; k++) {
    for (int j = 0; j < NUM_LEDS - k - 1; j++) {
      leds[j] = leds[j + 1];
    }
    leds[NUM_LEDS - k - 1] = CRGB::Gray;
    if (k % 3 == 2) {
      FastLED.show();
    }
  }

  for (int i = 0; i < 200; i++) {
    fadeToBlackBy(leds, NUM_LEDS, 4);
    FastLED.delay(20);
  }

  FastLED.clear();
  FastLED.delay(1000);
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
  FastLED.clear(); 
}

void colorsample()
{
  FastLED.clear();
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = kHalloweenColors[i % 4];
  }
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

int phaseStart = 0;
void xmas()
{
  FastLED.setBrightness(0x10);
  
  CRGB colors[5];
  colors[0] = CRGB::DarkRed;
  colors[1] = CRGB::Blue;
  colors[2] = CRGB::OrangeRed;
  colors[3] = CRGB::DarkGreen;
  colors[4] = CRGB::DarkViolet;

//  leds[random16() % NUM_LEDS] = colors[random8() % 5];
//  fadeToBlackBy(leds, NUM_LEDS, 1);

  int part = gFrame / 10;
  int width = 2;
  
  if (part < NUM_LEDS / width) {
    for (int i = part * width; i < (part * width + 2) && i < NUM_LEDS; i++) {
      leds[i] = colors[part % 5];
    }
  } else {
    phaseStart = gFrame;
  }

  if (phaseStart > 0 && gFrame > phaseStart + 120) {
    fadeToBlackBy(leds, NUM_LEDS, 1);
  }
}

void rainbow() 
{
  fill_rainbow(leds, NUM_LEDS, 0, 1);
}

void gradiant()
{
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(0, 0, i);
  }
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
