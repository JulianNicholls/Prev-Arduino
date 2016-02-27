#include <Adafruit_NeoPixel.h>

const byte WS1812_PIN   = 2;
const byte PIXELS       = 8;
const byte TRIG_PIN     = 13;
const byte ECHO_PIN     = 12;
const byte PROX_PIN     = 11;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELS, WS1812_PIN, NEO_GRB | NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:

  pinMode(TRIG_PIN, OUTPUT);  // Trigger
  pinMode(ECHO_PIN, INPUT);   // Echo

  strip.begin();
  strip.show();
}

void loop() {
  static byte intensity = 21;
  static byte int_delta = 3;
  
  digitalWrite(TRIG_PIN, LOW);

  // Send a 10uS pulse to the trigger pin

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Wait for the Echo pin to go high

  while(digitalRead(ECHO_PIN) == LOW)
    ;

  int start = micros();

  // Flight time is the length of time that the Echo pin is high

  while(digitalRead(ECHO_PIN) == HIGH)
    ;

  int     elapsed   = micros() - start;
  double  distance  = elapsed * 0.017163; // Speed of Sound is 34326cm/s (~768mph, 1,236km/h)
  int     idist     = int(distance + 0.5);
  
  fillStrip(idist, 70, intensity);

  // Raise the proximity pin if the distance is less than 40cm.
  digitalWrite(PROX_PIN, (idist < 40));

  intensity += int_delta;

  if(intensity == 0 || intensity == 99)
    int_delta = -int_delta;
  
  delay(40);
}

void fillStrip(int dist, int max, byte intensity) {
  int close = dist * PIXELS / max;

  for(int i = 0; i < PIXELS; ++i) {
    if(i >= close)
      strip.setPixelColor(i, strip.Color(0, intensity, 0));
     else 
      strip.setPixelColor(i, strip.Color(intensity, 0, 0));
  }

  strip.show();
}

