#include "U8glib.h"

U8GLIB_ST7920_128X64_1X u8g(10);    // Hardware SPI, 10 = CS/CE, 11 = MOSI, 13 = SCK

const int top     = 10;
const int height  = 8;
const int col1    = 0;
const int col2    = 80;
const int col3    = 110;

const int connections[] = { A0, A1, A2, A3 };

void draw(int raw[], int cooked[])
{
  // Draw left cross
  
  u8g.drawHLine(8, 26, 48);
  u8g.drawVLine(32, 2, 48);
  
  // Draw right cross
  
  u8g.drawHLine(72, 26, 48);
  u8g.drawVLine(96, 2, 48);

  // Position
  
  u8g.drawBox(31 + 4 * cooked[1], 25 - 4 * cooked[0], 3, 3);
  u8g.drawBox(95 + 4 * cooked[3], 25 - 4 * cooked[2], 3, 3);

  // Debugging only

  u8g.setFont(u8g_font_u8glib_4);
  
  for(int i = 0; i < 4; ++i ) {
    u8g.setPrintPos(i * 32, 62);
    u8g.print(raw[i]);
  }
}


void setup()
{
  u8g.setFont(u8g_font_u8glib_4);
}

// The -6..5 range is emprically correct to end up with -5..5.
// Specifying -5..5 gives a zero section which is skewed too high.

void loop()
{
  int raw[4], cooked[4];

  for(int i = 0; i < 4; ++i) {
    // Reading twice is better for stability if switching between analog inputs
    int dummy = analogRead(connections[i]);
    delay(10);
    raw[i]    = analogRead(connections[i]);
    cooked[i] = map(raw[i], 0, 1023, 5, -6);
  }

  // Picture loop, required by u8g
  u8g.firstPage();

  do {
    draw(raw, cooked);
  }
  while(u8g.nextPage());

  delay(100);
}

