#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

#include <SPI.h>
#include "Ucglib.h"

   
/*
  Hardware SPI Pins:
    Arduino Uno / Nano		sclk=13, data=11
*/

Ucglib_ST7735_18x128x160_HWSPI ucg(/*cd=*/ 9 , /*cs=*/ 10, /*reset=*/ 8);
RF24                           radio(9, 10);

const uint64_t channels[3]    = { 0xDEADBEEF0000, 0xDEADBEEF0002, 0xDEADBEEF0004 };

uint8_t z = 127;	// random start value

uint32_t lcg_rnd(void) {
  z = (uint8_t)((uint16_t)65 * (uint16_t)z + (uint16_t)17);
  
  return (uint32_t)z;
}


void setup(void)
{
  delay(1000);
  ucg.begin(UCG_FONT_MODE_TRANSPARENT);
  ucg.setRotate90();

  ucg.setColor(0, 255, 240, 0);
  ucg.setColor(1, 255, 240, 0);
  ucg.setColor(2, 32, 16, 0);
  ucg.setColor(3, 32, 16, 0);

  ucg.drawGradientBox(0, 0, ucg.getWidth(), ucg.getHeight());

  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, channels[1]);
  radio.openReadingPipe(2, channels[2]);
  radio.startListening();

  ucg.setColor(0, 0, 0, 0);
  ucg.setFontMode(UCG_FONT_MODE_TRANSPARENT);
  ucg.setPrintPos(4, 16);
  ucg.setFont(ucg_font_helvB10_tr);
  ucg.print(F("Radio Initialised"));
}

void loop(void)
{
  ucg.setColor(0, 0, 0, 0);
  ucg.setPrintPos(4, 32);
  ucg.print(F("Listening: "));

  ucg.setPrintPos(100, 32);

  if(radio.available()) {
      ucg.setColor(0, 0, 150, 0);
      ucg.print(F("Data "));
  }
  else {
      ucg.setColor(0, 150, 0, 0);
      ucg.print(F("None "));    
  }
  delay(1000);
}
