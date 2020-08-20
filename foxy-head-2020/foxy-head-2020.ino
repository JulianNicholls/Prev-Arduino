/* Costume 2020
*/

#include <Servo.h>

const short BLINK_REST = 60;
const short SWIVEL_REST = 70;

Servo blink, swivel;

void setup() {
  blink.attach(9);    // attaches the servo on pin 9 to the servo object
  swivel.attach(11);  // attaches the servo on pin 11 to the servo object

  blink.write(BLINK_REST);

  swivel.write(SWIVEL_REST);
}

void loop() {
  
  int this_time = random(60);
  
  if (this_time >= 48)   // Blink 12 times a minute 
    blinkEye();

  if (this_time < 12) {   // Swivel 12 times a minute
    swivelEye();  
  }
  
  delay(1000);
}

void blinkEye() {
  blink.write(130);
  delay(300);     // Humans blink for 300-400ms

  blink.write(BLINK_REST);
}

void swivelEye() {
  for(int i = SWIVEL_REST; i <= SWIVEL_REST + 30; i += 2) {
    swivel.write(i);
    delay(30);
  }

  for(int i = SWIVEL_REST + 30; i >= SWIVEL_REST - 30; i -= 2) {
    swivel.write(i);
    delay(30);
  }

  for(int i = SWIVEL_REST - 30; i <= SWIVEL_REST; i += 2) {
    swivel.write(i);
    delay(30);
  }
}

/**** FLICKER SEQUENCE ****
 * Currently only used for the startup sequence.  It's just a pattern of
 * on/off signals to the eye LEDs to simulate faulty wiring.
 * Uses macros #defined above, for conciseness
 ****/
