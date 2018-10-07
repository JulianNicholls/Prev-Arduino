#include <U8glib.h>

U8GLIB_ST7920_128X64_1X u8g(10);    // Hardware SPI, 10 = CS/CE, 11 = MOSI, 13 = SCK

/////////////////////////////////////////////////////////////////////////

int       timebase    = 100;    // 100ms
long      mv_per_div  = 1000;   // 1000mV (1V) per division
int       values[101];
const int nValues     = sizeof(values) / sizeof(values[0]);

/////////////////////////////////////////////////////////////////////////

void setup() {
  draw_loop();
}

void loop() {
  update();

  draw_loop();
  delay(500);
}

/////////////////////////////////////////////////////////////////////////
// Update the trace by reading A0 and transforming the returned raw values.

void update() {
  int i;
  
  analogRead(A0);   // Dummy read to prime the mux
  delay(10);

  // Read A0 repeatedly with a delay equal to the timebase * 100:
  //  We want 1/10 of the timebase, which is in ms, between successive 
  //  giving 10  readings per division, hence:
  
  //  delay in us = (timebase * 1000) / 10 = timebase * 100
  
  for(i = 0; i < nValues; ++i) {
    values[i] = analogRead(A0);
    delayMicroseconds(timebase * 100);
  }

  // Calculate the divisor to convert 0-1023 for 0-5V, into 60 
  // vertical divisions at the mv_per_div rate.
  
  int divisor = 1023 * mv_per_div / 60000;

  // Translate the raw 0-1023 values into 60-0, because the origin 
  // is at the top.
  
  for(i = 0; i < nValues; ++i) {
    values[i] = 60 - (values[i] / divisor);
  }
}

////////////////////////////////////////////////////////////////////////////
// Draw loop, specified by U8Glib.

void draw_loop() {
  u8g.firstPage();
  
  do {
    draw();
  } 
  while(u8g.nextPage());
}

// Redrawing dunction

void draw() {
  // Draw the grid as dotted lines every 10 pixels.
  
  for(int i = 0; i <= 100; i += 2) {
    for(int j = 0; j <= 60; j += 2) {
      if(j % 12 == 0 || i % 10 == 0) {
        u8g.drawPixel(i, j);
      }
    }
  }

  // Fill in the right side and display the mV per division and
  // ms per division
  
  u8g.setFont(u8g_font_profont10);

  u8g.drawBox(102, 0, 127, 63);
  u8g.setDefaultBackgroundColor();
  u8g.drawStr(103, 8, volts_str());
  u8g.drawStr(103, 16, timebase_str());

  u8g.setDefaultForegroundColor();

  // Draw the actual values
  
  for(int i = 0; i < nValues; ++i) {
    u8g.drawPixel(i, values[i]);  
  }
}

/////////////////////////////////////////////////////////////////////////
// Render volts as either, e.g 1.1V, or 200mV

char* volts_str() {
  static char str[6];   // enough for "999mV\0"

  if(mv_per_div >= 1000)
    sprintf(str, "%dV", mv_per_div / 1000);
  else
    sprintf(str, "%dmV", mv_per_div);

  return str;
}

/////////////////////////////////////////////////////////////////////////
// Render time as either, e.g 1.1s, or 200ms

char* timebase_str() {
  static char str[6];   // enough for "999ms\0"

  if(timebase >= 1000)
    sprintf(str, "%ds", timebase / 1000);
  else
    sprintf(str, "%dms", timebase);

  return str;
}

