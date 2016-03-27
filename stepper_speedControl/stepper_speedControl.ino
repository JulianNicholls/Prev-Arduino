
/*
 Stepper Motor Control - speed control

 This program drives a unipolar or bipolar stepper motor.
 The motor is attached to digital pins 8 - 11 of the Arduino.
 A potentiometer is connected to analog input 0.

 The motor will rotate in a clockwise direction. The higher the potentiometer value,
 the faster the motor speed. Because setSpeed() sets the delay between steps,
 you may notice the motor is less responsive to changes in the sensor value at
 low speeds.

 Created 30 Nov. 2009
 Modified 28 Oct 2010
 by Tom Igoe

 */

#include <Stepper.h>

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor


// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 3, 4, 5, 6);

int stepCount = 0;  // number of steps the motor has taken

void setup() {
  // nothing to do inside the setup

  Serial.begin(9600);
}

void loop() {
  static int prevSpeed = 0;
  // read the sensor value:
  int sensorReading = analogRead(A0);
  // map it to a range from 0 to 100:
  int motorSpeed = map(sensorReading, 0, 1023, -320, 320);

  if(abs(motorSpeed - prevSpeed) > 2) {
    Serial.println(motorSpeed);
    prevSpeed = motorSpeed;
  }
  
  // set the motor speed:
  if(abs(motorSpeed) > 10) {  // Add some hysteresis
    myStepper.setSpeed(abs(motorSpeed));
    // step 1/100 of a revolution:
    myStepper.step(motorSpeed / 10);
  }
}


