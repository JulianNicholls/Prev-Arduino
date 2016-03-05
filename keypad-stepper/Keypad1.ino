#include <Key.h>
#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;

// Motor pins

const int motorPin1 = 10;   // Blue   - 28BYJ48 pin 1
const int motorPin2 = 11;   // Pink   - 28BYJ48 pin 2
const int motorPin3 = 12;   // Yellow - 28BYJ48 pin 3
const int motorPin4 = 13;   // Orange - 28BYJ48 pin 4
                            // Red    - 28BYJ48 pin 5 (VCC)

const char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
};

const int max_digits = 4;

const int  motor_delay     = 1600;   // us delay between steps, 1600 is absolute minimum
const long counts_per_rev  = 512;    // Number of complete steps per full revolution
const int  motor_bits[]    = {B01100, B00110, B00011, B01001};

byte rowPins[ROWS] = { 9, 8, 7, 6 };
byte colPins[COLS] = { 5, 4, 3, 2 };

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int degrees     = 0;
byte digits     = 0;
bool clockwise  = true;

void setup() {
  // Set the motor pins as outputs
  
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  
  Serial.begin(9600);  
}

void loop() {
  char key = keypad.getKey();

  if(key != NO_KEY) {
    Serial.print(key);
    Serial.print(": ");
    
    if(key >= '0' && key <= '9') {
      int value = key - '0';

      if(digits < max_digits) {
        degrees = (degrees * 10) + value;
        ++digits;
      }
    }
    else {
      switch(key) {
        case '*':   // Backspace
          if(digits > 0) {
            --digits;
            degrees /= 10;
          }
          break;

        case '#':   // Enter
          if(degrees != 0) {
            rotate(degrees);
            degrees = 0;
            digits = 0;
          }
          break;  

        case 'C':   // Swap 'C'lockwise / Anti
          clockwise = !clockwise;
          break;
      }
    }

    Serial.println();
  }
}

void rotate(int degrees) {
  int steps = counts_per_rev * degrees / 360;

  Serial.print(degrees);
  Serial.print(" deg: rotate ");
  Serial.print(steps);
  Serial.print(clockwise ? " clockwise" : " anti-clockwise");

  for(int i = 0; i < steps; ++i)
    clockwise ? turn_clockwise() : turn_anticlockwise();
}

//////////////////////////////////////////////////////////////////////////////
// Set pins to ULN2003 high in sequence from 1 to 4
// delay "motor_delay" between each pin setting (to determine speed)

void turn_anticlockwise()
{
  for(int i = 0; i < 4; ++i)
  {
    setOutput(i);
    delayMicroseconds(motor_delay);
  }
}

void turn_clockwise()
{
  for(int i = 3; i >= 0; --i)
  {
    setOutput(i);
    delayMicroseconds(motor_delay);
  }
}

void setOutput(int out)
{
  digitalWrite(motorPin1, bitRead(motor_bits[out], 0));
  digitalWrite(motorPin2, bitRead(motor_bits[out], 1));
  digitalWrite(motorPin3, bitRead(motor_bits[out], 2));
  digitalWrite(motorPin4, bitRead(motor_bits[out], 3));
}

