#include "TouchScreen.h"

#define YP A2
#define XM A1
#define YM  6
#define XP  7

#define TS_MAXX 120
#define TS_MINX 950
#define TS_MAXY 150
#define TS_MINY 900

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define MINPRESSURE 1
#define MAXPRESSURE 1000

//------------------------------------
#include <UTFT.h>

UTFT myGLCD(ILI9327_8,A2,A1,A3,A4);

//------------------------------------
#include <stdint.h>
//------------------------------------
#define BOXSIZE 40
#define PENRADIUS 1
int oldcolor, currentcolor;
//************************************

void setup(void) {
  Serial.begin(9600);
  Serial.println("Paint!");
  myGLCD.InitLCD();
  myGLCD.clrScr();
  
  myGLCD.setColor(VGA_BLACK);
  myGLCD.fillRect(399-BOXSIZE,0, 399,  BOXSIZE);
  myGLCD.setColor(VGA_BLUE);
  myGLCD.fillRect(399-BOXSIZE,BOXSIZE, 399,  BOXSIZE*2);
  myGLCD.setColor(VGA_FUCHSIA);
  myGLCD.fillRect(399-BOXSIZE,BOXSIZE*2, 399,  BOXSIZE*3);
  myGLCD.setColor(VGA_LIME);
  myGLCD.fillRect(399-BOXSIZE,BOXSIZE*3, 399,  BOXSIZE*4);
  myGLCD.setColor(VGA_YELLOW);
  myGLCD.fillRect(399-BOXSIZE,BOXSIZE*4, 399,  BOXSIZE*5);
  myGLCD.setColor(VGA_RED);
  myGLCD.fillRect(399-BOXSIZE,BOXSIZE*5, 399,  BOXSIZE*6);

  myGLCD.setColor(VGA_RED);
  pinMode(13, OUTPUT);
  
}
//*************************************
void loop(void) {
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
 
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
  p.x = map(p.x, TS_MINX, TS_MAXX, 399, 0);
  p.y = map(p.y, TS_MAXY, TS_MINY, 239, 0);
  
  if (p.x > 399-BOXSIZE) {
     oldcolor = currentcolor;
     if (p.y < BOXSIZE) { 
       myGLCD.setColor(VGA_BLACK); 
       myGLCD.fillRect(0,0,398-BOXSIZE,239); 
       myGLCD.setColor(currentcolor);
     } 
     else if (p.y < BOXSIZE*2) {
       currentcolor = VGA_BLUE; 
     }
     else if (p.y < BOXSIZE*3) {
       currentcolor = VGA_FUCHSIA; 
     }
     else if (p.y < BOXSIZE*4) {
       currentcolor = VGA_LIME; 
     }
     else if (p.y < BOXSIZE*5) {
       currentcolor = VGA_YELLOW; 
     }
     else if (p.y < BOXSIZE*6) {
       currentcolor = VGA_RED; 
     }
    }  
    
    if (oldcolor != currentcolor){ 
      myGLCD.setColor(currentcolor); 
    }
    
    if ((p.x+PENRADIUS) < 399-BOXSIZE) {
    myGLCD.fillCircle(p.x, p.y, PENRADIUS);
    }
  }
}
  
