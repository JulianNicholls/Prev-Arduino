#include "mcp.h"

// This Arduino example demonstrates bidirectional operation of a 
// 28BYJ-48, using a ULN2003 interface board to drive the stepper.
// The 28BYJ-48 motor is a 4-phase, 8-beat motor, geared down by
// a factor of 68. One bipolar winding is on motor pins 1 & 3 and
// the other on motor pins 2 & 4. The step angle is 5.625/64 and the 
// operating Frequency is 100pps. Current draw is 92mA. 
////////////////////////////////////////////////

//declare variables for the motor pins
int motorPin1 = 8;    // Blue   - 28BYJ48 pin 1
int motorPin2 = 9;    // Pink   - 28BYJ48 pin 2
int motorPin3 = 10;   // Yellow - 28BYJ48 pin 3
int motorPin4 = 11;   // Orange - 28BYJ48 pin 4
                      // Red    - 28BYJ48 pin 5 (VCC)

const int motorSpeed    = 1300;   // us delay between steps, 1300 is absolute minimum
const int countsperrev  = 512;    // Number of complete steps per full revolution
//const int lookup8[]     = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};
const int lookup4[]     = {0b1100, 0b0110, 0b0011, 0b1001};

int   count = 0;          // count of steps made
long  last  = 0;

MCP23017 mcp;

//////////////////////////////////////////////////////////////////////////////

void setup() {
  mcp.begin();
  
  //declare the motor pins as outputs
  mcp.pinMode(motorPin1, OUTPUT);
  mcp.pinMode(motorPin2, OUTPUT);
  mcp.pinMode(motorPin3, OUTPUT);
  mcp.pinMode(motorPin4, OUTPUT);

  Serial.begin(9600);
}

//////////////////////////////////////////////////////////////////////////////

void loop(){
  if(count < countsperrev)
    clockwise4();
  else if(count == countsperrev * 2)
    count = 0;
  else
    anticlockwise4();
    
  if(count == 0 || count == countsperrev) {
    long  elapsed = millis() - last;
    float rpm     = 600000.0 / elapsed;

    last = millis();
    
    Serial.print("Time: ");
    Serial.print(elapsed);
    Serial.print("ms = ");
    Serial.print(rpm / 10);
    Serial.println(" RPM");
  }
    
  ++count;
}

//////////////////////////////////////////////////////////////////////////////
// Set pins to ULN2003 high in sequence from 1 to 4
// delay "motorSpeed" between each pin setting (to determine speed)

void anticlockwise4()
{
  for(int i = 0; i < 4; ++i)
  {
    setOutput4(i);
    delayMicroseconds(motorSpeed);
  }
}

void clockwise4()
{
  for(int i = 3; i >= 0; --i)
  {
    setOutput4(i);
    delayMicroseconds(motorSpeed);
  }
}

void setOutput4(int out)
{
  mcp.write_GPIO(lookup4[out], 1);
}

