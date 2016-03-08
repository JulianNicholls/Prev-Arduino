#include <Keypad_MC17.h>

const byte ROWS = 4;
const byte COLS = 4;

const char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
};

const int  MCP_addr   = 0x20;         // MCP23017 base address

const int  max_digits = 4;

const int  motor_delay     = 1250;   // us delay between steps, 1250 is absolute minimum
const long counts_per_rev  = 512;    // Number of complete steps per full revolution
const int  motor_bits[]    = {0b1100, 0b0110, 0b0011, 0b1001};

byte rowPins[ROWS] = { 0, 1, 2, 3 };  // GPA0-3 for rows
byte colPins[COLS] = { 4, 5, 6, 7 };  // GPA4-7 for columns

Keypad_MC17 keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS, MCP_addr);

int   degrees   = 0;
byte  digits    = 0;
bool  clockwise = true;

void setup() {
  // Set the motor pins as outputs

  Serial.begin(9600);
  keypad.begin();

  keypad.iodir_write(keypad.iodir_read() & 0x00ff); // Set Port B as outputs
}

void loop() {
  char key = keypad.getKey();

  if (key != NO_KEY) {
    Serial.print(key);
    Serial.print(": ");

    if (key >= '0' && key <= '9') {
      int value = key - '0';

      if (digits < max_digits) {
        degrees = (degrees * 10) + value;
        ++digits;
        Serial.print(degrees);
      }
    }
    else {
      switch (key) {
        case '*':   // Backspace
          if (digits > 0) {
            --digits;
            degrees /= 10;
          }
          break;

        case '#':   // Enter
          if (degrees != 0) {
            rotate(degrees);
            degrees = 0;
            digits = 0;
          }
          break;

        case 'C':   // Swap 'C'lockwise / Anti
          clockwise = !clockwise;
          Serial.print(clockwise ? "" : "Anti-");
          Serial.print("Clockwise");
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

  for (int i = 0; i < steps; ++i)
    clockwise ? turn_clockwise() : turn_anticlockwise();

  Serial.println("Done");
}

//////////////////////////////////////////////////////////////////////////////
// Set pins to ULN2003 high in sequence from 1 to 4
// delay "motor_delay" between each pin setting (to determine speed)

void turn_anticlockwise()
{
  for (int i = 0; i < 4; ++i)
  {
    setOutput(i);
    delayMicroseconds(motor_delay);
  }
}

void turn_clockwise()
{
  for (int i = 3; i >= 0; --i)
  {
    setOutput(i);
    delayMicroseconds(motor_delay);
  }
}

// Conservatively update Port B

void setOutput(int out)
{
  int prev = keypad.pinState_set() & 0x00ff;
  int newval = prev | (motor_bits[out] << 8);
  
  keypad.port_write(newval);
}


