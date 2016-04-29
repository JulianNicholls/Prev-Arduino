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

  ucg.setColor(0, 120, 0, 0);   // Mid Red
  ucg.setColor(1, 120, 0, 0);
  ucg.setColor(2, 120, 0, 120); // Mid Magenta
  ucg.setColor(3, 120, 0, 120);

  ucg.drawGradientBox(0, 0, ucg.getWidth(), ucg.getHeight());

  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, channels[1]);
  radio.openReadingPipe(2, channels[2]);
  radio.startListening();
}

void loop(void)
{
  // get a random value between 0 and 255
  uint8_t   rnd = lcg_rnd();
  ucg_int_t y   = 0;
  ucg_int_t h   = 16;

  // Using the same transparent font in solid mode works better,
  // but still some pixel are not overwritten
  y += h;
  ucg.setFontMode(UCG_FONT_MODE_TRANSPARENT);
  ucg.setPrintPos(4,y);
  ucg.setFont(ucg_font_helvB10_tr);
  ucg.print(F("Pixel Errors:"));
  ucg.setFontMode(UCG_FONT_MODE_SOLID);
  ucg.setFont(ucg_font_helvB10_tr);
  ucg.setColor(0, 255, 255, 255);		// use white as main color for the font
  ucg.setColor(1, 64, 64, 255);			// use blue as background for SOLID mode
  ucg.setPrintPos(110,y);
  ucg.print(rnd);

  // Using the "h" or "m" type of font in solid mode will almost work
  // however, if the number changes from 100 to 99, the last number 
  // will not be overwritten.
  y += h;
  ucg.setFontMode(UCG_FONT_MODE_TRANSPARENT);
  ucg.setPrintPos(4,y);
  ucg.setFont(ucg_font_helvB10_tr);
  ucg.print(F("Value Errors:"));
  ucg.setFontMode(UCG_FONT_MODE_SOLID);
  ucg.setFont(ucg_font_helvB10_hr);
  ucg.setColor(0, 255, 255, 255);		// use white as main color for the font
  ucg.setColor(1, 64, 64, 255);			// use blue as background for SOLID mode
  ucg.setPrintPos(110, y);
  ucg.print(rnd);

  // Using the "h" or "m" type of font in solid mode will work fine, if
  // there are extra spaced after the string to overwrite the previous value.
  // As long as a "h" font is used, the number of spaces is not fixed.
  // This means, even two spaced my not be sufficient. Better use a monospace font.
  y += h;
  ucg.setFontMode(UCG_FONT_MODE_TRANSPARENT);
  ucg.setPrintPos(4,y);
  ucg.setFont(ucg_font_helvB10_tr);
  ucg.print(F("Almost OK:"));
  ucg.setFontMode(UCG_FONT_MODE_SOLID);
  ucg.setFont(ucg_font_helvB10_hr);
  ucg.setColor(0, 255, 255, 255);		// use white as main color for the font
  ucg.setColor(1, 64, 64, 255);			// use blue as background for SOLID mode
  ucg.setPrintPos(110,y);
  ucg.print(rnd);
  ucg.print("  ");  // two extra spaces (not sufficient here)

  // The transparent font and mode can be used, if the previous content
  // is fully erased or overwritten
  y += h;
  ucg.setFontMode(UCG_FONT_MODE_TRANSPARENT);
  ucg.setPrintPos(4,y);
  ucg.setFont(ucg_font_helvB10_tr);
  ucg.print(F("Manual Erase:"));  
  ucg.setColor(0, 0, 0, 64);
  ucg.setColor(2, 0, 0, 64);
  ucg.setColor(1, 0, 64, 0);
  ucg.setColor(3, 0, 64, 0);
  ucg.drawGradientBox(110-2, y-12, 27, 14);  // red to green bar will erase the previous value
  ucg.setFontMode(UCG_FONT_MODE_TRANSPARENT);
  ucg.setFont(ucg_font_helvB10_tr);
  ucg.setColor(0, 255, 255, 255);		// use white as main color for the font
  ucg.setPrintPos(110,y);
  ucg.print(rnd);

  // The is will use the solid font with the extra spaces to erase the previous value
  // A monospaced 8x8 pixel font is used (here: ucg_font_amstrad_cpc_8r).
  y += h;
  ucg.setFontMode(UCG_FONT_MODE_TRANSPARENT);
  ucg.setPrintPos(4,y);
  ucg.setFont(ucg_font_helvB10_tr);
  ucg.print(F("8x8 Font:"));
  ucg.setFontMode(UCG_FONT_MODE_SOLID);
  ucg.setFont(ucg_font_amstrad_cpc_8r);
  ucg.setColor(0, 255, 255, 255);		// use white as main color for the font
  ucg.setColor(1, 64, 64, 255);			// use blue as background for SOLID mode
  ucg.setPrintPos(110,y);
  ucg.print(rnd);
  ucg.print("  ");  // extra spaces

  // The is will use the solid font with the extra spaces to erase the previous value
  // Any monospaced font can be used (here: ucg_font_7x13_mr).
  y += h;
  ucg.setFontMode(UCG_FONT_MODE_TRANSPARENT);
  ucg.setPrintPos(4,y);
  ucg.setFont(ucg_font_helvB10_tr);
  ucg.print(F("_mr Font:"));
  ucg.setFontMode(UCG_FONT_MODE_SOLID);
  ucg.setFont(ucg_font_7x14_mr);
  ucg.setColor(0, 255, 255, 255);		// use white as main color for the font
  ucg.setColor(1, 64, 64, 255);			// use blue as background for SOLID mode
  ucg.setPrintPos(110,y);
  ucg.print(rnd);
  ucg.print("  ");  // extra spaces


  delay(100);  
}
