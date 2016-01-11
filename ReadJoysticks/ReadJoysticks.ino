/*
  Analog input, analog output, serial output

 Reads an analog input pin, maps the result to a range from 0 to 255
 and uses the result to set the pulsewidth modulation (PWM) of an output pin.
 Also prints the results to the serial monitor.

 The circuit:
 * potentiometer connected to analog pin 0.
   Center pin of the potentiometer goes to the analog pin.
   side pins of the potentiometer go to +5V and ground
 * LED connected from digital pin 9 to ground

 created 29 Dec. 2008
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

// These constants won't change.  They're used to give names
// to the pins used:

const int ud1 = A1;   // Joystick 1 Up-Down
const int lr1 = A0;   // Joystick 1 Left-Right
const int ud2 = A3;   // Joystick 1 Up-Down
const int lr2 = A2;   // Joystick 1 Left-Right

int sensorValues[] = { 0, 0, 0, 0 };        // values read from the pot

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}

void loop() {
  sensorValues[0] = analogRead(ud1);
  delay(5); 

  sensorValues[1] = analogRead(lr1);
  delay(5); 
  
  sensorValues[2] = analogRead(ud2);
  delay(5); 

  sensorValues[3] = analogRead(lr2);
  delay(5); 

  // print the results to the serial monitor:

  for(int i = 0; i < 4; ++i)
  {
    Serial.print(sensorValues[i]);
    Serial.print(" ");
  }
  
  Serial.println();

  // wait 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(80);
}
