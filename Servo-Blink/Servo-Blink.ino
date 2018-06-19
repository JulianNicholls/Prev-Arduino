/* Costume
*/

#include <Servo.h>

const short LEFT_EYE = 4;
const short RIGHT_EYE = 5;

Servo myservo;  // create servo object to control a servo

int led1 = HIGH;

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object

  myservo.write(120);

  pinMode(LEFT_EYE, OUTPUT);
  pinMode(RIGHT_EYE, OUTPUT);

  flicker();
}

void loop() {
  int this_time = random(10);
  
  if (this_time >= 8)
    blink();

  if (this_time == 5)
    flicker();
  
  if (this_time < 5)
    eyes_random(100);

  digitalWrite(LEFT_EYE, HIGH);
  digitalWrite(RIGHT_EYE, HIGH);

  delay(500);
}

void blink() {
  myservo.write(60);
  delay(140);

  myservo.write(120);
//  delay(500);
}

/**** FLICKER SEQUENCE ****
 * Currently only used for the startup sequence.  It's just a pattern of
 * on/off signals to the eye LEDs to simulate faulty wiring.
 * Uses macros #defined above, for conciseness
 ****/

void eyes_on(unsigned short x) {
  digitalWrite(LEFT_EYE, HIGH);
  digitalWrite(RIGHT_EYE, HIGH);
  delay(x);
}

void eyes_off(unsigned short x) {
  digitalWrite(LEFT_EYE, LOW);
  digitalWrite(RIGHT_EYE, LOW);
  delay(x);
}

void eyes_random(unsigned short x) {
  if(random(10) >= 5) {
    digitalWrite(LEFT_EYE, LOW);
    digitalWrite(RIGHT_EYE, HIGH);
  }
  else {
    digitalWrite(LEFT_EYE, HIGH);
    digitalWrite(RIGHT_EYE, LOW);    
  }
  
  delay(x);
}

void flicker( void ) {
  eyes_on(150);
  eyes_off(150);
  
  for (int i = 80; i > 0; i -= 10){
    eyes_random(100);
    eyes_off(i);
  }
  
  eyes_on(300);
  eyes_off(50);
  eyes_on(100);
}

