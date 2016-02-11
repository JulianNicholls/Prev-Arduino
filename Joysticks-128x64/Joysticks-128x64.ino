#include "U8glib.h"

U8GLIB_ST7920_128X64_1X u8g(10);    // Hardware SPI, 10 = CS/CE, 11 = MOSI, 13 = SCK

const int top     = 10;
const int height  = 9;
const int col1    = 0;
const int col2    = 80;
const int col3    = 110;

const int connections[] = { A0, A1, A2, A3 };
const char* dirs[]      = { "Joystick 1 U/D", "Joystick 1 L/R", "Joystick 2 U/D", "Joystick 2 L/R" };

void draw(int raw[], int cooked[])
{
  for(int i = 0; i < 4; ++i ) {
    int line = top + height * i;
    u8g.drawStr(col1, line, dirs[i]);
    u8g.setPrintPos(col2, line);
    u8g.print(raw[i]);
    u8g.setPrintPos(col3, line);
    u8g.print(cooked[i]);
  }
}


void setup()
{
//  u8g.setFont(u8g_font_5x8);  // A little small
  u8g.setFont(u8g_font_profont10);
}

void loop()
{
  int raw[4], cooked[4];

  for(int i = 0; i < 4; ++i) {
    int dummy = analogRead(connections[i]);
    delay(50);
    raw[i]    = analogRead(connections[i]);
    cooked[i] = map(raw[i], 0, 1023, 5, -5);
  }

  // Picture loop, required by u8g
  u8g.firstPage();

  do {
    draw(raw, cooked);
  }
  while(u8g.nextPage());

  delay(200);
}
