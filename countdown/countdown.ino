#include <Keypad_MC17.h>
#include <Wire.h>
#include <Keypad.h>

#define   SETTING1  0
#define   SETTING   1
#define   COUNTING  4
#define   FINISHED  5

const int DIGIT1  = 2;
const int DIGIT2  = 3;
const int DIGIT3  = 4;
const int DIGIT4  = 5;

const int SHIFT_DATA    = 6;
const int SHIFT_CLK     = 7;

const int BEEP_PIN      = 8;

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

Keypad_MC17 keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR);

int countdown = 0;
int start;

char entry[5];

void setup() {
  for(int i = 0; i < 4; ++i) {
    pinMode(digits[i], OUTPUT);
    digitalWrite(digits[i], LOW);
  }
  
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(BEEP_PIN, OUTPUT);

  keypad.begin();
}

void loop() {
  static byte state = SETTING1;
  static byte beep  = LOW;
  
  if(state < COUNTING) {
    if(state == SETTING1) {
      entry[0] = 0;
      light_digit(3, 0, 150, false);
      delay(200);      
    }
    else
      show_str(5);

    state = process_key(state);
  }
  else if(state == COUNTING) {
    int left = countdown - ((millis() - start) / 1000);

    if(left < 0)
      state = FINISHED;
    else
      show_time(left);
  }
  else { // FINISHED
    beep = (beep == LOW) ? HIGH : LOW;
    digitalWrite(BEEP_PIN, beep);
    delayMicroseconds(500);
  }
}

byte process_key(byte state) {
  char key[] = { 0, 0 };
  
  key[0] = keypad.getKey();

  switch(key[0]) {
    case NO_KEY:
      return state;

    case '*':
    {
      size_t len = strlen(entry);
      
      if(len > 0)
        entry[--len] = 0;

      return (len == 0) ? SETTING1 : SETTING;
    }
    
    case '#':
      countdown_from_str();
      start     = millis();
      return COUNTING;

    case 'A':
    case 'B':
    case 'C':
    case 'D':
      break;

    default:
      if(strlen(entry) < 4)
        strcat(entry, key);
      break;
  }
  
  return SETTING;
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
    digitalWrite(SHIFT_CLK, LOW);
    shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, value < 10 ? patterns[value] : value);
  }

  delay(on_time);
  digitalWrite(digits[digit], LOW);
}

void show_str(int on_time) {
  int e_index = 0;
  
  for(int i = 4 - strlen(entry); i < 4; ++i) {
    byte pattern = patterns[entry[e_index++] - '0'];

    if(i == 1)
      bitWrite(pattern, 7, 0);

    light_digit(i, pattern, on_time, false);
  }
}

void countdown_from_str() {
  const int multipliers[] = { 600, 60, 10, 1 };
  int       m_index       = 3;

  countdown = 0;

  for(int i = strlen(entry) - 1; i >= 0; --i) {
    countdown += multipliers[m_index--] * (entry[i] - '0');
  }
}

