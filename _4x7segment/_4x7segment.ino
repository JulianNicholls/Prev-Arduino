#include <Keypad_MC17.h>
#include <Wire.h>
#include <Keypad.h>

#define   SETTING1  0
#define   SETTING2  1
#define   SETTING3  2
#define   SETTING4  3
#define   COUNTING  4
#define   FINISHED  5

const int DIGIT1  = 2;
const int DIGIT2  = 3;
const int DIGIT3  = 4;
const int DIGIT4  = 5;
const int DATA    = 6;
const int CLK     = 7;

const int digits[] = { DIGIT1, DIGIT2, DIGIT3, DIGIT4 };

const int patterns[] = { 0b11000000, 0b11101011, 0b10010010, 0b10001010, 0b10101001, 
                         0b10001100, 0b10000100, 0b11101010, 0b10000000, 0b10001000 };

const byte ROWS = 4; // four rows
const byte COLS = 4; // four columns
const char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = { 0, 1, 2, 3 };  // Connect to the row pinouts of the keypad
byte colPins[COLS] = { 4, 5, 6, 7 };  // Connect to the column pinouts of the keypad

#define I2CADDR 0x20

Keypad_MC17 keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR );

int  countdown = 0;
                       
void setup() {
  for(int i = 0; i < 4; ++i) {
    pinMode(digits[i], OUTPUT);
    digitalWrite(digits[i], LOW);
  }
  
  pinMode(DATA, OUTPUT);
  pinMode(CLK, OUTPUT);

  keypad.begin();
}

byte count = 0;

void loop() {
  static byte state     = SETTING1;
  
  if(state < COUNTING) {
    if(state == SETTING1) {
      light_digit(3, 0, 200, false);
      delay(200);      
    }
    else
      show_time(countdown);

    state = process_key(state);
  }
  else {
    int uptime = millis() / 1000;
      
    uptime = (uptime <= countdown) ? (130 - uptime) : uptime;
  
    show_time(uptime);
  }
}

byte process_key(byte state) {
  int key = keypad.getKey();

  if(key == NO_KEY)
    return state;
    
  return COUNTING;
}

void show_time(int seconds) {
  int minute    = seconds / 60;
  int second    = seconds % 60;
  int values[]  = { minute / 10, minute % 10, second / 10, second % 10 };

  for(int i = 0; i < 4; ++i) {
    byte pattern = patterns[values[i]];

    if(i == 1)
      bitWrite(pattern, 7, values[3] & 1);

    light_digit(i, pattern, 5, false);
  }
}

void light_digit(byte digit, byte value, int on_time, bool suppress_zero) {
  if(!suppress_zero || value != 0) {
    digitalWrite(digits[digit], HIGH);
    digitalWrite(CLK, LOW);
    shiftOut(DATA, CLK, MSBFIRST, value < 10 ? patterns[value] : value);
  }

  delay(on_time);
  digitalWrite(digits[digit], LOW);
}

