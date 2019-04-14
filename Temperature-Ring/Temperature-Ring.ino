#include <OneWire.h>
#include <DallasTemperature.h>

/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#define DEBUG

#include <FastLED.h>

#define LED_DATA_PIN    5 
#define TEMP_DATA_PIN   4
#define NUM_LEDS       12

OneWire oneWire(TEMP_DATA_PIN);
DallasTemperature sensors(&oneWire);
 
CRGB leds[NUM_LEDS];

byte r = 60;
byte g = 60;
byte b = 60;
  
void setup()
{
  FastLED.addLeds<WS2812B, LED_DATA_PIN, GRB>(leds, NUM_LEDS);
  
#ifdef DEBUG
  Serial.begin(115200);
#endif
  
}

bool off = false;

void loop() {
  if (irrecv.decode(&results)) {
    switch(results.value) {
      case BTN_CH:
        off = !off;
        break;

      case BTN_PLAY:
        r = min(255, r * 5 / 4);
        g = min(255, g * 5 / 4);
        b = min(255, b * 5 / 4);
        break;

      case BTN_EQ:
        r = (r * 4) / 5;
        g = (g * 4) / 5;
        b = (b * 4) / 5;
        break;

      case BTN_CH_MINUS:
        if(r >= 10) r -= 10;
        off = false;
        break;

      case BTN_CH_PLUS:
        if(r <= 245) r += 10;
        off = false;
        break;

      case BTN_PREV:
        if(g >= 10) g -= 10;
        off = false;
        break;

      case BTN_NEXT:
        if(g <= 245) g += 10;
        off = false;
        break;
        
      case BTN_MINUS:
        if(b >= 10) b -= 10;
        off = false;
        break;

      case BTN_PLUS:
        if(b <= 245) b += 10;
        off = false;
        break;
    }

#ifdef DEBUG
    Serial.print(results.value, HEX);
    Serial.print(", r: ");
    Serial.print(r);
    Serial.print(", g: ");
    Serial.print(g);
    Serial.print(", b: ");
    Serial.println(b);
#endif

    for(int i = 0; i < NUM_LEDS; ++i)
      leds[i] = off ? CRGB(0, 0, 0) : CRGB(r, g, b);
      
    FastLED.show();

    irrecv.resume(); // Receive the next value
  }

  delay(500);
}
