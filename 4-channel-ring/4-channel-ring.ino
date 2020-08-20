#include <FastLED.h>

#define LED_DATA_PIN   D2
#define NUM_LEDS       12

#define BTN_A          D5
#define BTN_B          D6
#define BTN_C          D7
#define BTN_D          D8

#define DEBUG
CRGB leds[NUM_LEDS];

byte red = 40;
byte green = 40;
byte blue = 40;

void setup() {
  // put your setup code here, to run once:

  pinMode(BTN_A, INPUT);
  pinMode(BTN_B, INPUT);
  pinMode(BTN_C, INPUT);
  pinMode(BTN_D, INPUT);

  FastLED.addLeds<WS2812B, LED_DATA_PIN, GRB>(leds, NUM_LEDS);
#ifdef DEBUG
  Serial.begin(115200);
#endif
}

void loop() {
  // put your main code here, to run repeatedly:

  byte inA = digitalRead(BTN_A);
  byte inB = digitalRead(BTN_B);
  byte inC = digitalRead(BTN_C);
  byte inD = digitalRead(BTN_D);

  if (inA) {
#ifdef DEBUG
    Serial.println(" ***");
    Serial.println("*   *");
    Serial.println("*****");
    Serial.println("*   *");
    Serial.println("*   *");
#endif

    red = 40; green = 0; blue = 0;
  }

  if (inB) {
#ifdef DEBUG
    Serial.println("****");
    Serial.println("*   *");
    Serial.println("****");
    Serial.println("*   *");
    Serial.println("****");
#endif

    red = 0; green = 40; blue = 0;
  }

  if (inC) {
#ifdef DEBUG
    Serial.println("****");
    Serial.println("*   *");
    Serial.println("*");
    Serial.println("*   *");
    Serial.println("****");
#endif

    red = 0; green = 0; blue = 40;
  }

  if (inD) {
#ifdef DEBUG
    Serial.println("***");
    Serial.println("*  *");
    Serial.println("*   *");
    Serial.println("*  *");
    Serial.println("***");
#endif

    red = 20; green = 20; blue = 0;
  }

#ifdef DEBUG
  Serial.println("");
#endif

  for(int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = CRGB(red, green, blue);

    FastLED.show();
  }

 #ifdef debug
  delay(500);
 #else
  delay(100);
 #endif
}
