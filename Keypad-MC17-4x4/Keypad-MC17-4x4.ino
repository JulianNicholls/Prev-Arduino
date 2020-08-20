/* @file HelloKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates the simplest use of the matrix Keypad library.
|| #
   Modified for Keypad_MCP G. D. (Joe) Young July 29/12
   Modified for Keypad_MC16 G. D. (Joe) Young Jan 26/13
   Modified for Keypad_MC17 G. D. (Joe) Young May 18/14
*/
#include <Keypad_MC17.h>
#include <Wire.h>
#include <Keypad.h>

#define I2CADDR 0x20

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

Keypad_MC17 keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR );

//char str[128] = "";

void setup(){
  keypad.begin();
  Serial.begin(9600);

  Serial.println("Initialised");
}
  
void loop() {
  char key = keypad.getKey();
  
  if(key != NO_KEY) {
//    strcat(str, key); 
    Serial.println(key);
  }
}

