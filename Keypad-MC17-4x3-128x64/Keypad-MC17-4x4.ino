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
#include <U8glib.h>

#define I2CADDR 0x20

const byte ROWS = 4; // four rows
const byte COLS = 3; // four columns
const char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = { 0, 1, 2, 3 };  // Connect to the row pinouts of the keypad
byte colPins[COLS] = { 4, 5, 6 };     // Connect to the column pinouts of the keypad

Keypad_MC17             keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR );
U8GLIB_ST7920_128X64_1X u8g(10);    // Hardware SPI, 10 = CS/CE, 11 = MOSI, 13 = SCK

char str[128] = "";

void setup(){
  keypad.begin();
  
  u8g.setFont(u8g_font_5x8);
}
  
void loop() {
  char key[2] = { 0, 0 };
  key[0] = keypad.getKey();
  
  if(key[0]) {
    strcat(str, key); 
//    Serial.println(key);
  }

  draw_loop();
}

////////////////////////////////////////////////////////////////////////

void draw_loop() {
  u8g.firstPage();
  
  do {
    draw();
  } 
  while(u8g.nextPage());
}

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here

  u8g.drawStr(0, 8, str);
}

