/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://www.arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */

const int INP2 = 2;
const int INP3 = 3;
const int INP4 = 4;
const int INP5 = 5;

const int LED6 = 6;
const int LED7 = 7;
const int LED8 = 8;
const int LED9 = 9;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);

  pinMode(INP2, INPUT);
  pinMode(INP3, INPUT);
  pinMode(INP4, INPUT);
  pinMode(INP5, INPUT);

  pinMode(LED6, OUTPUT);
  pinMode(LED7, OUTPUT);
  pinMode(LED8, OUTPUT);
  pinMode(LED9, OUTPUT);  
}

// the loop function runs over and over again forever
void loop() {
  for(int i = INP2; i <= INP5; ++i) {
    if(digitalRead(i) == LOW) {
      digitalWrite(i+4, HIGH);
    }
  }

  delay(200);
  
  for(int i = LED6; i <= LED9; ++i) {
    digitalWrite(i, LOW);
  }
}
