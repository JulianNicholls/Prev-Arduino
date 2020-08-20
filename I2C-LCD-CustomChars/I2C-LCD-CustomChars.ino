//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif

uint8_t bell[8]     = {0x04, 0x0e, 0x0e, 0x0e, 0x1f, 0x00, 0x04};
uint8_t note[8]     = {0x02, 0x03, 0x02, 0x0e, 0x1e, 0x0c, 0x00};
uint8_t clock[8]    = {0x00, 0x0e, 0x15, 0x17, 0x11, 0x0e, 0x00};
uint8_t heart[8]    = {0x00, 0x04, 0x0a, 0x11, 0x11, 0x15, 0x1f};
uint8_t duck[8]     = {0x00, 0x0c, 0x1d, 0x0f, 0x0f, 0x06, 0x00};
uint8_t check[8]    = {0x00, 0x01, 0x03, 0x16, 0x1c, 0x08, 0x00};
uint8_t cross[8]    = {0x00, 0x0e, 0x11, 0x11, 0x11, 0x0e, 0x00};
uint8_t retarrow[8] = {0x00, 0x04, 0x04, 0x04, 0x0e, 0x0a, 0x0e};
  
LiquidCrystal_I2C lcd(0x27,  16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  
  lcd.createChar(0, bell);
  lcd.createChar(1, note);
  lcd.createChar(2, clock);
  lcd.createChar(3, heart);
  lcd.createChar(4, duck);
  lcd.createChar(5, check);
  lcd.createChar(6, cross);
  lcd.createChar(7, retarrow);
  lcd.home();
  
  lcd.print("Hello world...");
  lcd.setCursor(0, 1);
  lcd.print(" I ");
  lcd.printByte(3);
  lcd.print(" Arduinos!");
  delay(2000);
  displayKeyCodes();
  
}

// display all keycodes
void displayKeyCodes(void) {
  lcd.clear();
  lcd.print(" Codes 00-07");
  lcd.setCursor(0, 1);
  
  for(int i = 0; i < 8; ++i) {
    lcd.printByte(i);
    lcd.print(' ');
  }
  
//  uint8_t i = 0;
//  while (1) {
//    lcd.clear();
//    lcd.print("Codes 0x"); lcd.print(i, HEX);
//    lcd.print("-0x"); lcd.print(i+16, HEX);
//    lcd.setCursor(0, 1);
//    for (int j=0; j<16; j++) {
//      lcd.printByte(i+j);
//    }
//    i+=16;
//    
//    delay(4000);
//  }
}

void loop()
{

}

