/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int potpin = A0;  // analog pin used to connect the potentiometer
int prev_angle;

void setup() {
  Serial.begin(9600);

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  int inp   = analogRead(potpin);            // Read the value of the potentiometer (0..1023)
  int angle = map(inp, 0, 1023, 179, 1);     // scale it to use it with the servo (1..179)
  int diff  = abs(prev_angle - angle);       // Difference
  
  Serial.print(inp, DEC);
  Serial.print(", ");
  Serial.println(angle, DEC);

  if (diff > 5) {                     // Take out the judder
    myservo.write(angle);             // Set the servo position
    delay(diff * 2);                  // Wait for the servo to get there
    prev_angle = (prev_angle + angle) / 2;
  }
  
  delay(50);
}

