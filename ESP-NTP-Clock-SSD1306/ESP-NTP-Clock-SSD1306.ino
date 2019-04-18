/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 by ThingPulse, Daniel Eichhorn
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * ThingPulse invests considerable time and money to develop these open source libraries.
 * Please support us by buying our products (and not the clones) from
 * https://thingpulse.com
 *
 */

#include <TimeLib.h>

// WiFi-related stuff
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// Include the correct display library
// For a connection via I2C using Wire include
// #include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#include "OLEDDisplayUi.h"

// Include custom images
#include "images.h"

const char ssid[] = "BTHub6-SZ8H";  //  your network SSID (name)
const char pass[] = "RwyNcn7qdmqi";       // your network password

const int timeZone = 1;     // CET / BST

WiFiUDP udp;
const unsigned int localPort = 8888;  // local port to listen for udp packets

// Initialize the OLED display using Wire library
SSD1306Wire  display(0x3c, D3, D5);

OLEDDisplayUi ui (&display);

const int screenW = 128;
const int screenH = 64;
const int clockCenterX = screenW/2;
const int clockCenterY = ((screenH-16)/2)+16;   // top yellow part is 16 px height
const int clockRadius = 23;

// utility function for digital clock display: prints leading 0
String twoDigits(int digits) {
  if(digits < 10) {
    String i = '0'+String(digits);
    return i;
  }
  else {
    return String(digits);
  }
}

void clockOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {

}

void analogClockFrame(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  // Draw the clock face
    display->drawCircle(clockCenterX + x, clockCenterY + y, clockRadius);
//  display->drawCircle(clockCenterX + x, clockCenterY + y, 2);
 
  // hour ticks
  for(int z = 0; z < 360; z += 30) {
    // Begin at 0° and stop at 360°
    float angle = z / 57.29577951; // Convert degrees to radians
    
    int x2 = (clockCenterX + (sin(angle) * clockRadius));
    int y2 = (clockCenterY - (cos(angle) * clockRadius));
    int x3 = (clockCenterX + (sin(angle) * (clockRadius - (clockRadius / 8))));
    int y3 = (clockCenterY - (cos(angle) * (clockRadius - (clockRadius / 8))));
    
    display->drawLine( x2 + x, y2 + y, x3 + x, y3 + y);
  }

  // Display second hand
  
  float angle = second() * 6;
  angle /= 57.29577951; // Convert degrees to radians
  
  int x3 = ( clockCenterX + ( sin(angle) * ( clockRadius - ( clockRadius / 5 ) ) ) );
  int y3 = ( clockCenterY - ( cos(angle) * ( clockRadius - ( clockRadius / 5 ) ) ) );
  
  display->drawLine(clockCenterX + x, clockCenterY + y, x3 + x, y3 + y);
  
  // Display minute hand
  
  angle = minute() * 6;
  angle /= 57.29577951; // Convert degrees to radians
  
  x3 = (clockCenterX + (sin(angle) * (clockRadius - (clockRadius / 4))));
  y3 = (clockCenterY - (cos(angle) * (clockRadius - (clockRadius / 4))));
  display->drawLine(clockCenterX + x, clockCenterY + y, x3 + x, y3 + y);

  // Display hour hand
  
  angle = hour() * 30 + int((minute() / 12) * 6);
  angle /= 57.29577951; // Convert degrees to radians
  
  x3 = (clockCenterX + (sin(angle) * (clockRadius - (clockRadius / 2))));
  y3 = (clockCenterY - (cos(angle) * (clockRadius - (clockRadius / 2))));
  display->drawLine(clockCenterX + x, clockCenterY + y, x3 + x, y3 + y);
}

void digitalClockFrame(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  String timenow = String(hour()) + ":" + twoDigits(minute()) + ":" + twoDigits(second());
  
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_24);
  display->drawString(clockCenterX + x, clockCenterY + y, timenow );
}

// This array keeps function pointers to all frames
// frames are the single views that slide in
FrameCallback frames[] = { analogClockFrame, digitalClockFrame };

// how many frames are there?
int frameCount = 2;

// Overlays are statically drawn on top of a frame eg. a clock
OverlayCallback overlays[] = { clockOverlay };

int overlaysCount = 1;

void setup() {
  Serial.begin(115200);
  Serial.println();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());
  
  Serial.print("IP number assigned by DHCP is ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");

	// The ESP is capable of rendering 60fps in 80Mhz mode
	// but that won't give you much time for anything else
	// run it in 160Mhz mode or just set it to 30 fps
  ui.setTargetFPS(30);

	// Customize the active and inactive symbol
  ui.setActiveSymbol(activeSymbol);
  ui.setInactiveSymbol(inactiveSymbol);

  // You can change this to
  // TOP, LEFT, BOTTOM, RIGHT
  ui.setIndicatorPosition(TOP);

  // Defines where the first frame is located in the bar.
  ui.setIndicatorDirection(LEFT_RIGHT);

  // You can change the transition that is used
  // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_UP, SLIDE_DOWN
  ui.setFrameAnimation(SLIDE_LEFT);

  // Add frames
  ui.setFrames(frames, frameCount);

  // Add overlays
  ui.setOverlays(overlays, overlaysCount);

  // Initialising the UI will init the display too.
  ui.init();

  display.flipScreenVertically();

  unsigned long secsSinceStart = millis();
  // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
  const unsigned long seventyYears = 2208988800UL;
  // subtract seventy years:
  unsigned long epoch = secsSinceStart - seventyYears * SECS_PER_HOUR;
  setTime(epoch);

  Serial.println("waiting for sync");
  setSyncProvider(getNTPTime);
}


void loop() {
  int remainingTimeBudget = ui.update();

  if (remainingTimeBudget > 0) {
    // You can do some work here.
    // Don't do stuff if you are below your time budget.
    
    delay(remainingTimeBudget);
  }
}

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48;     // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; // Buffer to hold incoming & outgoing packets

time_t getNTPTime()
{
  // Discard any previously received packets
  while (udp.parsePacket() > 0);
  
  Serial.print("Transmit NTP Request - ");
  sendNTPpacket();
  
  uint32_t beginWait = millis();
  
  while (millis() - beginWait < 1500) {
    int size = udp.parsePacket();
    
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Received NTP Response.");
      
      udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      
      unsigned long secsSince1900;
      
      // convert four bytes starting at location 40 to a long integer
      
      secsSince1900 =  (unsigned long) packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long) packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long) packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long) packetBuffer[43];
      
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  
  Serial.println("No Response.");
  return 0; // return 0 if unable to get the time
}

// Send an NTP request to the time server at the given address
void sendNTPpacket()
{
  // Set all bytes in the buffer to 0
  
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  
  // Initialize values needed to form NTP request

  packetBuffer[0] = 0b11100011;   // 0xE3, LI = 3 (Unsync'ed), Version = 4, Mode = 3 (Client)
  packetBuffer[1] = 0;            // Stratum = 0
  packetBuffer[2] = 6;            // Polling Interval = 6s
  packetBuffer[3] = 0xEC;         // Peer Clock Precision = 1us
  
  // 8 bytes of zero for Root Delay and Root Dispersion
  
  packetBuffer[12]  = 49;         // '1'
  packetBuffer[13]  = 0x4E;       // 'N'
  packetBuffer[14]  = 49;         // '1'
  packetBuffer[15]  = 52;         // '4'
  
  // Send a udp packet requesting a timestamp:
  
  udp.beginPacket("uk.pool.ntp.org", 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}
