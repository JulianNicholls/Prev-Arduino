#include <U8glib.h>

U8GLIB_ST7920_128X64_1X u8g(10);    // Hardware SPI, 10 = CS/CE, 11 = MOSI, 13 = SCK

int       timebase    = 100;    // 100ms
long      mv_per_div  = 1000;   // 1000mV (1V) per division
int       values[101];
const int nValues     = sizeof(values) / sizeof(values[0]);

void setup() {
  // put your setup code here, to run once:

  draw_loop();
}

void loop() {
  update();

  draw_loop();
  delay(500);
}

////////////////////////////////////////////////////////////////////////////

void update() {
  int i;
  
  analogRead(A0);   // Dummy read to prime the mux
  delay(10);
  
  for(i = 0; i < nValues; ++i) {
    values[i] = analogRead(A0);
    delayMicroseconds(timebase * 100);
  }

  int divisor = 1023000L / (60 * mv_per_div);
  
  for(i = 0; i < nValues; ++i) {
    values[i] = (1023 - values[i]) / divisor;
  }
}

////////////////////////////////////////////////////////////////////////////

void draw_loop() {
  u8g.firstPage();
  
  do {
    draw();
  } 
  while(u8g.nextPage());
}

void draw() {
  u8g.setFont(u8g_font_profont10);

  for(int i = 0; i <= 100; i += 2) {
    for(int j = 0; j <= 60; j += 2) {
      if(j % 12 == 0 || i % 10 == 0) {
        u8g.drawPixel(i, j);
      }
    }
  }

  u8g.drawBox(102, 0, 127, 63);
  u8g.setDefaultBackgroundColor();
  u8g.drawStr(103, 8, "1V");
  u8g.drawStr(103, 16, timebase_str());

  u8g.setDefaultForegroundColor();

  for(int i = 0; i < nValues; ++i) {
    u8g.drawPixel(i, values[i]);  
  }

  
//  u8g.setFont(u8g_font_helvB10);
//  u8g.setPrintPos(0, 14);
//  u8g.print(steps);
//
//  u8g.drawStr(40, 8, degrees ? "Degrees" : "Steps");
//  u8g.drawStr(40, 18, clockwise ? "Clockwise" : "Anti-Clockwise");
//
//  if(motor_steps > 0) {
//    u8g.drawStr(0, 40, "Steps To Go:");
//    u8g.setPrintPos(80, 40);
//    u8g.print(motor_steps);
//  }
}

char* timebase_str() {
  static char str[6];   // enough for "100ms\0"

  if(timebase >= 1000)
    sprintf(str, "%ds", timebase / 1000);
  else
    sprintf(str, "%dms", timebase);

  return str;
}

