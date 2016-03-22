// UTouch_Calibration (C)2013-2014 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
#include "TouchScreen.h"

#define YP A2
#define XM A1
#define YM  6
#define XP  7
int pmx,pmy;


#define TS_MAXX 957
#define TS_MINX 122
#define TS_MAXY 141
#define TS_MINY 915

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define MINPRESSURE 1 // 10
#define MAXPRESSURE 1000
// This program can be used to calibrate the touchscreen
// of the display modules.
// This program requires the UTFT library and a touch
// screen module that is compatible with UTFT.
//
// It is assumed that the display module is connected to an
// appropriate shield or that you know how to change the pin 
// numbers in the setup.
//
// Instructions will be given on the display.
//

#include <UTFT.h>

// Initialize display
// ------------------
// Set the pins to the correct ones for your development board
// -----------------------------------------------------------
// Standard Arduino Uno/2009 Shield            : <display model>,19,18,17,16
// Standard Arduino Mega/Due shield            : <display model>,38,39,40,41
// CTE TFT LCD/SD Shield for Arduino Due       : <display model>,25,26,27,28
// Teensy 3.x TFT Test Board                   : <display model>,23,22, 3, 4
// ElecHouse TFT LCD/SD Shield for Arduino Due : <display model>,22,23,31,33
//
// Remember to change the model parameter to suit your display module!
//UTFT    myGLCD(ITDB32S,38,39,40,41);
//          
UTFT myGLCD(ILI9327_8,A2,A1,A3,A4);

// Initialize touchscreen
// ----------------------
// Set the pins to the correct ones for your development board
// -----------------------------------------------------------
// Standard Arduino Uno/2009 Shield            : 15,10,14, 9, 8
// Standard Arduino Mega/Due shield            :  6, 5, 4, 3, 2
// CTE TFT LCD/SD Shield for Arduino Due       :  6, 5, 4, 3, 2
// Teensy 3.x TFT Test Board                   : 26,31,27,28,29
// ElecHouse TFT LCD/SD Shield for Arduino Due : 25,26,27,29,30
//

// ************************************
// DO NOT EDIT ANYTHING BELOW THIS LINE
// ************************************

// Declare which fonts we will be using
extern uint8_t SmallFont[];

uint32_t cx, cy;
uint32_t rx[8], ry[8];
uint32_t clx, crx, cty, cby;
float px, py;
int dispx, dispy, text_y_center;
uint32_t calx, caly, cals;
char buf[13];
int xdis,ydis;

#define BOXSIZE  40
#define PENRADIUS 1
int oldcolor, currentcolor;

void setup()
{
  Serial.begin(9600);
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);

  dispx=myGLCD.getDisplayXSize();
  dispy=myGLCD.getDisplayYSize();

  text_y_center=(dispy/2)-6;
  Serial.print(" dispx "); 
  Serial.print(dispx);
  Serial.print(" dispy "); 
  Serial.println(dispy);
}

void drawCrossHair(int x, int y)
{
  myGLCD.drawRect(x-10, y-10, x+10, y+10);
  myGLCD.drawLine(x-5, y, x+5, y);
  myGLCD.drawLine(x, y-5, x, y+5);
}

void readCoordinates()
{
  int iter = 10; // 5000 msec
  int failcount = 0;
  int cnt = 0;
  int pix,piy;
  uint32_t tx=0;
  uint32_t ty=0;
  boolean OK = false;

  while (OK == false)
  {
    myGLCD.setColor(255, 255, 255);
    myGLCD.print("*  PRESS  *", CENTER, text_y_center);

    digitalWrite(13, HIGH);

    TSPoint p = ts.getPoint();
    digitalWrite(13, LOW);
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    pix = p.x;
    piy = p.y;

    if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
      pmx = map(pix,TS_MINX, TS_MAXX, 0,399);
      pmy = map(piy, TS_MAXY, TS_MINY, 239, 0);

      if (cnt < iter)
      {
        tx += pix;
        ty += piy;
        cnt++;
      }
      else 
      {
        cx = tx / iter;
        cy = ty / iter;
        Serial.print(" cx "); 
        Serial.print(cx);
        Serial.print(" cy "); 
        Serial.println(cy);
        return;
      }
    }
  }

}

void calibrate(int x, int y, int i)
{
  myGLCD.setColor(255, 255, 255);
  drawCrossHair(x,y);
  readCoordinates();
  myGLCD.setBackColor(0,0,0);
  myGLCD.setColor(0, 255,255);
  if (i <= 4) {
    myGLCD.printNumI(pmx, x+14,y-10,3,' ');   
    myGLCD.printNumI(cx, x+45,y-10,3,' ');
    myGLCD.printNumI(pmy, x+14,y,3,' ');  
    myGLCD.printNumI(cy, x+45,y,3,' ');
    myGLCD.setBackColor(255, 0, 0);
  }  
  else {
    myGLCD.printNumI(pmx, x-69,y-10,3,' ');   
    myGLCD.printNumI(cx, x-35,y-10,3,' ');
    myGLCD.printNumI(pmy, x-69,y,3,' ');  
    myGLCD.printNumI(cy, x-35,y,3,' ');
    myGLCD.setBackColor(255, 0, 0);
  }  

  if ( i == 7) delay(8000);

  myGLCD.setColor(255, 255, 255);
  myGLCD.print("* RELEASE *", CENTER, text_y_center);
  myGLCD.setColor(80, 80, 80);
  drawCrossHair(x,y);
  rx[i]=cx;
  ry[i]=cy;
  Serial.print(" xdis "); 
  Serial.print(x);
  Serial.print(" ydis "); 
  Serial.print(y);
  Serial.print(" i "); 
  Serial.print(i);
  Serial.print(" rx "); 
  Serial.print(cx);
  Serial.print(" ry "); 
  Serial.println(cy);

}

void toHex(uint32_t num)
{
  buf[0] = '0';
  buf[1] = 'x';
  buf[10] = 'U';
  buf[11] = 'L';
  buf[12] = 0;
  for (int zz=9; zz>1; zz--)
  {
    if ((num & 0xF) > 9)
      buf[zz] = (num & 0xF) + 55;
    else
      buf[zz] = (num & 0xF) + 48;
    num=num>>4;
  }
}

void startup()
{
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(0, 0, dispx-1, 13);  // 0,0,399
  myGLCD.setColor(255, 255, 255);  // weiss
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.drawLine(0, 14, dispx-1, 14);
  myGLCD.print("TouchScreen Calibration", CENTER, 1);
  myGLCD.setBackColor(0, 0, 0);

  if (dispx==220) //220)
  {  
    myGLCD.print("Use a stylus or something", 1, 30);
    myGLCD.print("similar to touch as close", 1, 42);
    myGLCD.print("to the center of the", 1, 54);
    myGLCD.print("highlighted crosshair as", 1, 66);
    myGLCD.print("possible. Keep as still as", 1, 78);
    myGLCD.print("possible and keep holding", 1, 90);
    myGLCD.print("until the highlight is", 1, 102);
    myGLCD.print("removed. Repeat for all", 1, 114);
    myGLCD.print("crosshairs in sequence.", 1, 126);
    myGLCD.print("wait about 5 second to continue", 1, 162);

  }
  else
  {
    myGLCD.print("INSTRUCTIONS", CENTER, 30);
    myGLCD.print("Use a stylus or something similar to", LEFT, 50);
    myGLCD.print("touch as close to the center of the", LEFT, 62);
    myGLCD.print("highlighted crosshair as possible. Keep", LEFT, 74);
    myGLCD.print("as still as possible and keep holding", LEFT, 86);
    myGLCD.print("until the highlight is removed. Repeat", LEFT, 98);
    myGLCD.print("for all crosshairs in sequence.", LEFT, 110);

    myGLCD.print("Further instructions will be displayed", LEFT, 134);
    myGLCD.print("when the calibration is complete.", LEFT, 146);

    myGLCD.print("Do NOT use your finger as a calibration", LEFT, 170);
    myGLCD.print("stylus or the result WILL BE imprecise.", LEFT, 182);

    myGLCD.print("wait about 5 seconds to continue", CENTER, 226);
  }
  delay(5000);

}

void done()
{
  myGLCD.clrScr();
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(0, 0, dispx-1, 13);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.drawLine(0, 14, dispx-1, 14);
  myGLCD.print("UTouch Calibration", CENTER, 1);
  myGLCD.setBackColor(0, 0, 0);

  if (dispx==400)
  {  
    myGLCD.print("To use the new calibration", 1, 30);
    myGLCD.print("settings you must edit the", 1, 42);
    myGLCD.setColor(160, 160, 255);
    myGLCD.print("UTouchCD.h", 1, 54);
    myGLCD.setColor(255, 255, 255);
    myGLCD.print("file and change", 88, 54);  // 88 , 54);
    myGLCD.print("the following values. The", 1, 66);
    myGLCD.print("values are located right", 1, 78);
    myGLCD.print("below the opening comment.", 1, 90);
    myGLCD.print("CAL_X", 1, 110);
    myGLCD.print("CAL_Y", 1, 122);
    myGLCD.print("CAL_S", 1, 134);
    toHex(calx);
    myGLCD.print(buf, 75, 110);
    toHex(caly);
    myGLCD.print(buf, 75, 122);
    toHex(cals);
    myGLCD.print(buf, 75, 134);
  }
  else
  {  
    myGLCD.print("CALIBRATION COMPLETE", CENTER, 30);
    myGLCD.print("To use the new calibration", 1, 50);
    myGLCD.print("settings you must edit the", 1, 62);
    myGLCD.setColor(160, 160, 255);
    myGLCD.print("UTouchCD.h", 1, 74);
    myGLCD.setColor(255, 255, 255);
    myGLCD.print("file and change", 1,74);//88, 74);
    myGLCD.print("the following values.", 1, 86);
    myGLCD.print("The values are located right", 1, 98);
    myGLCD.print("below the opening comment in", 1, 110);
    myGLCD.print("the file.", 1, 122);
    myGLCD.print("CAL_X", 1, 150);
    myGLCD.print("CAL_Y", 1, 162);
    myGLCD.print("CAL_S", 1, 174);

    toHex(calx);
    myGLCD.print(buf, 75, 150);
    toHex(caly);
    myGLCD.print(buf, 75, 162);
    toHex(cals);
    myGLCD.print(buf, 75, 174);
  }

}

void loop()
{
  startup();

start_cal: 
  myGLCD.clrScr();
  myGLCD.setColor(80, 80, 80);
  drawCrossHair(dispx-11, 10);
  drawCrossHair(dispx/2, 10);
  drawCrossHair(10, 10);
  drawCrossHair(dispx-11, dispy/2);
  drawCrossHair(10, dispy/2);
  drawCrossHair(dispx-11, dispy-11);
  drawCrossHair(dispx/2, dispy-11);
  drawCrossHair(10, dispy-11);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.print("***********", CENTER, text_y_center-12);
  myGLCD.print("***********", CENTER, text_y_center+12);

  calibrate(10, 10, 0);
  calibrate(10, dispy/2, 1);
  calibrate(10, dispy-11, 2);
  calibrate(dispx/2, 10, 3);
  calibrate(dispx/2, dispy-11, 4);
  calibrate(dispx-11, 10, 5);
  calibrate(dispx-11, dispy/2, 6);
  calibrate(dispx-11, dispy-11, 7);
  delay(5000);
  goto start_cal;

  cals=(long(dispx-1)<<12)+(dispy-1);
  px = abs(((float(rx[5]+rx[6]+rx[7])/3)-(float(rx[0]+rx[1]+rx[2])/3))/(dispy-20));  // LANDSCAPE
  clx = (((rx[0]+rx[1]+rx[2])/3));  // LANDSCAPE
  crx = (((rx[5]+rx[6]+rx[7])/3));  // LANDSCAPE

  if (clx<crx)
  {
    clx = clx - (px*10);
    crx = crx + (px*10);
  }
  else
  {
    clx = clx + (px*10);
    crx = crx - (px*10);
  }

  py = abs(((float(ry[0]+ry[3]+ry[5])/3)-(float(ry[2]+ry[4]+ry[7])/3))/(dispx-20));  // LANDSCAPE
  cty = (((ry[0]+ry[3]+ry[5])/3));  // LANDSCAPE
  cby = (((ry[2]+ry[4]+ry[7])/3));  // LANDSCAPE

  if (cty<cby)
  {
    cty = cty - (py*10);
    cby = cby + (py*10);
  }
  else
  {
    cty = cty + (py*10);
    cby = cby - (py*10);
  }

  calx = (long(clx)<<14) + long(crx);
  caly = (long(cty)<<14) + long(cby);
  cals = cals + (1L<<31);

  Serial.print(" px "); 
  Serial.print(px);
  Serial.print(" py "); 
  Serial.print(py);
  Serial.print(" calx "); 
  Serial.print(calx);
  Serial.print(" caly "); 
  Serial.println(caly);


  done();
  while(true) {
  }
}
