#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>

#define WS1812_PIN    2     // Serial Output for NeoPixels
#define PIXELS        8     // 8 Pixels on board

#define TRIG_PIN      12    // Trigger for ultrasonic
#define ECHO_PIN      13    // Echo from ultrasonic

#define BL_PIN        5     // Backlight control for LCD

#define ON_TIMEOUT    0000L // Time that it needs to be off before coming on again
#define ON_TIME       10000L // Time that it stays on

Adafruit_NeoPixel strip(PIXELS, WS1812_PIN, NEO_GRB | NEO_KHZ800);
LiquidCrystal     lcd(11, 10, 9, 8, 7, 6);

void setup() {
  pinMode(TRIG_PIN, OUTPUT);  // Trigger
  pinMode(ECHO_PIN, INPUT);   // Echo
  pinMode(BL_PIN, OUTPUT);    // Backlight

  // Initialise NeoPixels
  strip.begin();
  strip.show();

  // Initialise 20x4 LCD
  lcd.begin(20, 4);
}

long last_on = -60000;

void loop() {
  static byte intensity = 21;
  static byte int_delta = 3;

  int dist = measureDistance();
    
  if(dist < 60 && (millis() - last_on) > (ON_TIMEOUT + ON_TIME))
    showMessage();

  if((millis() - last_on) > ON_TIME)
    turnOffLCD();
  
  fillStrip(dist, 100, intensity);

  intensity += int_delta;

  if(intensity == 0 || intensity == 99)
    int_delta = -int_delta;
  
  delay(40);
}

int measureDistance() {
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

  return  int(distance + 0.5);
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

void showMessage() {
//  digitalWrite(BL_PIN, HIGH);
  last_on = millis();
  
  lcd.setCursor(0, 0);
  lcd.print("Fashion Hack 2016");
  lcd.setCursor(0, 1);
  lcd.print("Julian Nicholls");
  lcd.setCursor(0, 2);
  lcd.print("@ReallyBigShoeUK");
  lcd.setCursor(0, 3);
  lcd.print("reallybigshoe.co.uk");
}

void turnOffLCD() {
//  digitalWrite(BL_PIN, LOW);
  lcd.clear();
//  lcd.print("Off");
//  lcd.setCursor(0, 1);
//  lcd.print("Millis ");
//  lcd.print(millis());
//
//  lcd.setCursor(0, 2);
//  lcd.print(last_on);
}

