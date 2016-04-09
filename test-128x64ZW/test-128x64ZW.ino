#include <U8glib.h>

U8GLIB_ST7920_128X64_1X u8g(10);    // Hardware SPI, 10 = CS/CE, 11 = MOSI, 13 = SCK

void setup(void)
{ 
}

void loop(void)
{
  static bool done = false;
  
  // Picture loop

  if(!done) {
    draw_loop();
  
    done = true;
  }
  
  // Don't Rebuild the picture after some delay
  
  delay(200);
}

void draw_loop() {
  u8g.firstPage();
  
  do {
    draw();
  } 
  while(u8g.nextPage());
}


void draw(void) {
  // graphic commands to redraw the complete screen should be placed here
  // u8g.setFont(u8g_font_04b_03b);

  u8g.setFont(u8g_font_helvB10);
  u8g.drawStr(5, 17, "Testing HW SPI");

  u8g.setFont(u8g_font_6x10);
  u8g.drawStr(5, 28, "6x10 7");

  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(70, 28, "helvB08 8");

  u8g.setFont(u8g_font_5x8);
  u8g.drawStr(5, 36, "5x8 6");

  u8g.setFont(u8g_font_courB08);
  u8g.drawStr(70, 36, "courB08 6");

  u8g.setFont(u8g_font_5x8);
  u8g.drawStr(5, 45, "3 lines of 24 characters");
  u8g.drawStr(5, 52, "XXXXXXXXXXXXXXXXXXXXXXXX");
  u8g.drawStr(0, 59, "12345678901234567890123456");

  u8g.drawFrame(0, 0, 128, 64);
  u8g.drawFrame(2, 2, 124, 60);
}


