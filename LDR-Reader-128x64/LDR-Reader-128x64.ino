#include "U8glib.h"

U8GLIB_ST7920_128X64_1X u8g(13, 11, 12, U8G_PIN_NONE);

void draw(char *raw, char *volts)
{
  u8g.drawStr(0, 12, "Raw Value");
  u8g.drawStr(95, 12, raw);

  u8g.drawStr(0, 24, "Voltage");
  u8g.drawStr(88, 24, volts);
}

void setup()
{
  pinMode(A0, INPUT);
    
  u8g.setFont(u8g_font_7x14);
}

void loop()
{
  static char str[10], str2[10];
  int raw = analogRead(A0);
  int v   = map(raw, 0, 1023, 0, 5000);

  sprintf(str, "%04d", raw);
  sprintf(str2, "%d.%03d", v / 1000, v % 1000);
  
  // Picture loop, required by u8g
  u8g.firstPage();

  do {
    draw(str, str2);
  }
  while(u8g.nextPage());

  delay(400);
}
