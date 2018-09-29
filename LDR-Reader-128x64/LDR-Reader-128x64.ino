#include "U8glib.h"

U8GLIB_ST7920_128X64_1X u8g(10);    // Hardware SPI, 10 = CS/CE, 11 = MOSI, 13 = SCK

void draw(int raw, double volts)
{
  u8g.drawStr(0, 12, "Raw Value");
  u8g.setPrintPos(95, 12);
  u8g.print(raw);

  u8g.drawStr(0, 24, "Voltage");
  u8g.setPrintPos(88, 24);
  u8g.print(volts);
}

void setup()
{
  pinMode(A0, INPUT);
    
  u8g.setFont(u8g_font_6x10);
}

void loop()
{
  int raw = analogRead(A0);
  int v   = map(raw, 0, 1023, 0, 5000);

  // Picture loop, required by u8g
  u8g.firstPage();

  do {
    draw(raw, v / 1000.0);
  }
  while(u8g.nextPage());

  delay(400);
}
