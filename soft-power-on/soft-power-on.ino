#include "U8glib.h"

U8GLIB_ST7920_128X64_1X u8g(10);    // Hardware SPI, 10 = CS/CE, 11 = MOSI, 13 = SCK

unsigned int count = 0;

void setup() {
  // Ensure we stay on
    
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

  u8g.setFont(u8g_font_6x10); 
}

void loop() {
  // put your main code here, to run repeatedly:

  draw_loop();

  delay(200);
}

void draw_loop() {
  u8g.firstPage();
  
  do {
    draw();
  } 
  while(u8g.nextPage());
}

void draw() {
  u8g.drawStr(0, 10, "Power-on pin set");
  
  u8g.setPrintPos(0, 32);
  u8g.print(count++, BIN);
}

