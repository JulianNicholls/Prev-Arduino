/**
 * ESP-Wroom connections
 * 
 * ESP32  RC522
 *  D18   SCK   (Pin 2)
 *  D19   MISO  (Pin 4)
 *  D21   SDA   (Pin 1)
 *  D22   RST   (Pin 7)
 *  D23   MOSI  (Pin 3)
 *  GND   GND   (Pin 6)
 *  3v3   3.3V  (Pin 8)
 */

#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>

#include <HTTPClient.h>
#include <WiFiClientSecure.h>

#define SS_PIN          21           // Configurable, see typical pin layout above
#define RST_PIN         22           // Configurable, see typical pin layout above

#ifndef STASSID
  #define STASSID   "BTHub6-SZ8H"
  #define STAPSK    "RwyNcn7qdmqi"
#endif


const char* rootCertificate = 
"-----BEGIN CERTIFICATE-----\n"
"MIIESTCCAzGgAwIBAgITBn+UV4WH6Kx33rJTMlu8mYtWDTANBgkqhkiG9w0BAQsF\n"
"ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n"
"b24gUm9vdCBDQSAxMB4XDTE1MTAyMjAwMDAwMFoXDTI1MTAxOTAwMDAwMFowRjEL\n"
"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEVMBMGA1UECxMMU2VydmVyIENB\n"
"IDFCMQ8wDQYDVQQDEwZBbWF6b24wggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEK\n"
"AoIBAQDCThZn3c68asg3Wuw6MLAd5tES6BIoSMzoKcG5blPVo+sDORrMd4f2AbnZ\n"
"cMzPa43j4wNxhplty6aUKk4T1qe9BOwKFjwK6zmxxLVYo7bHViXsPlJ6qOMpFge5\n"
"blDP+18x+B26A0piiQOuPkfyDyeR4xQghfj66Yo19V+emU3nazfvpFA+ROz6WoVm\n"
"B5x+F2pV8xeKNR7u6azDdU5YVX1TawprmxRC1+WsAYmz6qP+z8ArDITC2FMVy2fw\n"
"0IjKOtEXc/VfmtTFch5+AfGYMGMqqvJ6LcXiAhqG5TI+Dr0RtM88k+8XUBCeQ8IG\n"
"KuANaL7TiItKZYxK1MMuTJtV9IblAgMBAAGjggE7MIIBNzASBgNVHRMBAf8ECDAG\n"
"AQH/AgEAMA4GA1UdDwEB/wQEAwIBhjAdBgNVHQ4EFgQUWaRmBlKge5WSPKOUByeW\n"
"dFv5PdAwHwYDVR0jBBgwFoAUhBjMhTTsvAyUlC4IWZzHshBOCggwewYIKwYBBQUH\n"
"AQEEbzBtMC8GCCsGAQUFBzABhiNodHRwOi8vb2NzcC5yb290Y2ExLmFtYXpvbnRy\n"
"dXN0LmNvbTA6BggrBgEFBQcwAoYuaHR0cDovL2NydC5yb290Y2ExLmFtYXpvbnRy\n"
"dXN0LmNvbS9yb290Y2ExLmNlcjA/BgNVHR8EODA2MDSgMqAwhi5odHRwOi8vY3Js\n"
"LnJvb3RjYTEuYW1hem9udHJ1c3QuY29tL3Jvb3RjYTEuY3JsMBMGA1UdIAQMMAow\n"
"CAYGZ4EMAQIBMA0GCSqGSIb3DQEBCwUAA4IBAQCFkr41u3nPo4FCHOTjY3NTOVI1\n"
"59Gt/a6ZiqyJEi+752+a1U5y6iAwYfmXss2lJwJFqMp2PphKg5625kXg8kP2CN5t\n"
"6G7bMQcT8C8xDZNtYTd7WPD8UZiRKAJPBXa30/AbwuZe0GaFEQ8ugcYQgSn+IGBI\n"
"8/LwhBNTZTUVEWuCUUBVV18YtbAiPq3yXqMB48Oz+ctBWuZSkbvkNodPLamkB2g1\n"
"upRyzQ7qDn1X8nn8N8V7YJ6y68AtkHcNSRAnpTitxBKjtKPISLMVCx7i4hncxHZS\n"
"yLyKQXhw2W2Xs0qLeC1etA+jTGDK4UfLeC0SF7FSi8o5LL21L8IzApar2pR/\n"
"-----END CERTIFICATE-----";

//const char* endpoint = "http://aff251f2c133.ngrok.io/card";
const char* endpoint = "https://listen.daizy.io/clockd";

byte mac[6];

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

/**
 * Initialize.
 */
void setup() {
    Serial.begin(115200); // Initialize serial communications with the PC
    while (!Serial);      // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
    
    SPI.begin();          // Init SPI bus
    mfrc522.PCD_Init();   // Init MFRC522 card

    Serial.println(F("Connecting..."));
    WiFi.begin(STASSID, STAPSK);
    while(WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print('.');
    }

    Serial.print(F("Connected: "));
    Serial.println(WiFi.localIP());
    dump_byte_array(WiFi.macAddress(mac), 6);
    
    Serial.println(F("\nWaiting for a card"));
}

/**
 * Main loop.
 */
void loop() {
    // Look for new cards
    if (!mfrc522.PICC_IsNewCardPresent())
        return;

    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial())
        return;

    // Show some details of the PICC (that is: the tag/card)
    Serial.print(F("\n\nCard UID:"));
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();
    Serial.print(F("PICC type: "));
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.println(mfrc522.PICC_GetTypeName(piccType));

    // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();

    WiFiClientSecure* client = new WiFiClientSecure;

    if (client) {
      client->setCACert(rootCertificate);
      {  
        HTTPClient https;
        https.begin(*client, endpoint);
        https.addHeader("Content-Type", "application/json");
        https.addHeader("x-api-key", "ade7283a3477e2cb9c1724f76410631f");
    
        String httpData = "{ \"data\": \"";
        httpData += hex2digit(mfrc522.uid.uidByte[0]);
        httpData += hex2digit(mfrc522.uid.uidByte[1]);
        httpData += hex2digit(mfrc522.uid.uidByte[2]);
        httpData += hex2digit(mfrc522.uid.uidByte[3]);
    
        httpData += "\", \"deviceid\": \"";
        
        httpData += hex2digit(mac[0]);
        httpData += hex2digit(mac[1]);
        httpData += hex2digit(mac[2]);
        httpData += hex2digit(mac[3]);
        httpData += hex2digit(mac[4]);
        httpData += hex2digit(mac[5]);
        
        httpData += "\" }";
    
        Serial.println("\nSending to Daizy endpoint...");
    
        Serial.println(httpData);
        int response = https.POST(httpData);
        https.end();

        Serial.print("Response: ");
        Serial.println(response);
      }
      
      delete client;
    }
}

/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}

String hex = "";

String& hex2digit(int value) {
  if (value < 16) {
    hex = "0";
    hex += String(value, HEX);
  } 
  else {
    hex = String(value, HEX);
  }
  
  return hex;
}
