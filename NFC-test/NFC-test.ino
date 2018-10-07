#include <PN532.h>

#define SS 10
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  #define MISO 50
  #define MOSI 51
  #define SCK 52
#else
  #define MISO 12
  #define MOSI 11
  #define SCK 13
#endif
 
PN532 nfc(SCK, MISO, MOSI, SS);
 
void setup(void) {
  Serial.begin(9600);
  nfc.begin();
 
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  Serial.print("Supports "); Serial.println(versiondata & 0xFF, HEX);
 
  // configure board to read RFID tags and cards
  nfc.SAMConfig();
}
 
void loop(void) {
  uint32_t id;
  // look for MiFare type cards
  id = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A);
 
  if (id != 0) {
    Serial.println();
    Serial.print("Read card #"); 
    Serial.println(id);
  }
  else {
    Serial.print("No Card  ");
  }

  delay(1000);
}
