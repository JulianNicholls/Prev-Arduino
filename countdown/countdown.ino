#include <Keypad_MC17.h>
#include <Wire.h>
#include <Keypad.h>

#define   SETTING1  0
#define   SETTING   1
#define   COUNTING  4
#define   FINISHED  5

const byte DIGIT1  = 2;
const byte DIGIT2  = 3;
const byte DIGIT3  = 4;
const byte DIGIT4  = 5;

const byte SHIFT_DATA    = 6;
const byte SHIFT_CLK     = 7;

const byte BEEP_PIN      = 8;

const byte digits[]   = { DIGIT1, DIGIT2, DIGIT3, DIGIT4 };

const byte patterns[] = { 0b11000000, 0b11101011, 0b10010010, 0b10001010, 0b10101001, 
                         0b10001100, 0b10000100, 0b11101010, 0b10000000, 0b10001000 };

const byte done[]     = { 0b11000000, 0b10000111, 0b11100000, 0b10010100 };

const byte underbar   = 0b11011111;

const byte ROWS = 4; // four rows
const byte COLS = 3; // four columns
const char keys[ROWS][COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};

byte rowPins[ROWS] = { 0, 1, 2, 3 };  // Connect to the row pinouts of the keypad
byte colPins[COLS] = { 4, 5, 6 };     // Connect to the column pinouts of the keypad

#define MCP_ADDR 0x20

Keypad_MC17 keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS, MCP_ADDR);

int countdown = 0;
int start;

char entry[5] = { 0 };

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
    show_str(5);
    light_digit(3, underbar, 5, false);

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
    show_done();
    beep = (beep == LOW) ? HIGH : LOW;
    digitalWrite(BEEP_PIN, beep);
//    delayMicroseconds(500);
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

void show_str(int on_time) {
  size_t len     = strlen(entry); // 0 -> Nothing, 1 -> 2, 2 -> 1, 3 -> 0, 4 -> 0
  int    d_first = 0;
  int    d_last  = 2;
  int    e_index = 0;

  switch(len) {
    case 0:
      return;

    case 1:
    case 2:
    case 3:
      d_first = 3 - len;
      break;

    case 4:
      d_last = 3;
  }
  
  for(int i = d_first; i <= d_last; ++i) {
    byte pattern = patterns[entry[e_index++] - '0'];

    if(i == d_last - 2)
      bitWrite(pattern, 7, 0);
      
    light_digit(i, pattern, on_time, false);
  }
}

void show_done() {
  for(int i = 0; i < 4; ++i)
    light_digit(i, done[i], 5, false);
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

void countdown_from_str() {
  const int multipliers[] = { 600, 60, 10, 1 };
  int       m_index       = 3;

  countdown = 0;

  for(int i = strlen(entry) - 1; i >= 0; --i) {
    countdown += multipliers[m_index--] * (entry[i] - '0');
  }
}

