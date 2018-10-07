#include <SPI.h>
#include "Ucglib.h"

   
/*
  Hardware SPI Pins:
    Arduino Uno / Nano		sclk=13, data=11
*/

Ucglib_ST7735_18x128x160_HWSPI ucg(/*cd=*/ 9 , /*cs=*/ 10, /*reset=*/ 8);
RF24                           radio(7, 6);

const uint64_t channels[3]    = { 0xDEADBEEF0000, 0xDEADBEEF0002, 0xDEADBEEF0004 };

byte buffer[16];

void setup(void)
{
  delay(1000);
  ucg.begin(UCG_FONT_MODE_TRANSPARENT);
  ucg.setRotate90();

  ucg.setColor(0, 255, 240, 0);
  ucg.setColor(1, 255, 240, 0);
  ucg.setColor(2, 255, 255, 255);
  ucg.setColor(3, 255, 255, 255);

  ucg.drawGradientBox(0, 0, ucg.getWidth(), ucg.getHeight());

  ucg.setColor(0, 0, 0, 0);
  ucg.setFontMode(UCG_FONT_MODE_TRANSPARENT);
  ucg.setPrintPos(4, 16);
  ucg.setFont(ucg_font_helvB10_tr);
  ucg.print(F("Initialising"));

  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, channels[1]);
  radio.openReadingPipe(2, channels[2]);
  radio.startListening();

  ucg.setColor(0, 255, 240, 0);
  ucg.drawBox(0, 0, 128, 20);
  ucg.setColor(0, 0, 0, 0);
  ucg.setPrintPos(4, 16);
  ucg.print(F("Radio Initialised"));
}

void loop(void)
{
  ucg.setColor(0, 0, 0, 0);
  ucg.setPrintPos(4, 32);
  ucg.setFont(ucg_font_helvB10_tr);
  ucg.print(F("Listening: "));

  ucg.setPrintPos(100, 32);

  if(radio.available()) {
      ucg.setColor(0, 0, 150, 0);
      ucg.print(F("Data "));
      delay(50);
      radio.read(buffer, 16);
      show_buffer();
      delay(1000);
  }
  else {
      ucg.setColor(0, 150, 0, 0);
      ucg.print(F("None "));    
  }
  
  delay(1000);
}

void show_buffer() {
  ucg.setColor(0, 255, 255, 255);
  ucg.drawBox(0, 48, ucg.getWidth(), 40);
  ucg.setPrintPos(4, 64);
  ucg.setFont(ucg_font_helvB08_tr);
  ucg.setColor(0, 0, 0, 0);

  for(int i = 0; i < 16; ++i) {
    ucg.print(buffer[i], HEX);
    ucg.print(" ");
  }
}

