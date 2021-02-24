#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <NewPing.h>

#define PIN 12

#define TRIGGER_PIN 8
#define ECHO_PIN 9
#define MAX_DIS 400

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DIS);

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  pinMode(11, OUTPUT);
  digitalWrite(11, HIGH);

  Serial.begin(115200);
}

void loop()
{
  delay(100);
  unsigned int uS = sonar.ping();

  // Some example procedures showing how to display to the pixels:
  if (uS >= 5000)
  {
    colorWipe(strip.Color(0, 167, 104), 30);
  }
  else if (uS >= 3000 && uS < 5000)
  {
    colorWipe(strip.Color(0, 47, 167), 30);
  }
  else if (uS >= 2000 && uS < 3000)
  {
    colorWipe(strip.Color(65, 0, 167), 30);
  }
  else if (uS >= 1000 && uS < 2000)
  {
    colorWipe(strip.Color(155, 0, 167), 30);
  }
  else if (uS >= 500 && uS < 1000)
  {
    colorWipe(strip.Color(167, 0, 69), 30);
  }
  else
  {
    colorWipe(strip.Color(255, 255, 0), 30);
  }
}

void transition(uint32_t c1, uint32_t c2)
{
  if (c1 > c2)
  {
    uint32_t m = c1;
    c1 = c2;
    c2 = m;
  }

  for (uint32_t i = c1; i < c2; i++)
  {
    colorWipe(i, 10);
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait)
{
  for (uint16_t i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait)
{
  uint16_t i, j;

  for (j = 0; j < 256; j++)
  {
    for (i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait)
{
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++)
  { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85)
  {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  else if (WheelPos < 170)
  {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else
  {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
