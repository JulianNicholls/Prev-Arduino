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


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize all digital pins as outputs.

  for(int i = 0; i < 5; ++i)
    pinMode(i, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  for(int set = 0; set < 5; ++set) {
    for(int pin = 0; pin < 5; ++pin)
      digitalWrite(pin, (pin == set) ? HIGH : LOW);
      
    delay(300);              // wait for a while
  }
}
