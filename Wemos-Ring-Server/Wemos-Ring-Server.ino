/*
   Copyright (c) 2015, Majenko Technologies
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

 * * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

 * * Redistributions in binary form must reproduce the above copyright notice, this
     list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.

 * * Neither the name of Majenko Technologies nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <FastLED.h>

#ifndef STASSID
#define STASSID "BTHub6-SZ8H"
#define STAPSK  "RwyNcn7qdmqi"

#endif

#define LED_DATA_PIN   D3
#define NUM_LEDS       12

const char *ssid = STASSID;
const char *password = STAPSK;

const char html[] PROGMEM = "<!DOCTYPE html>\
<html lang=\"en\">\
<head>\
  <meta charset=\"UTF-8\">\
  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
  <meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\">\
  <title>ESP8266 Ring Server</title>\
</head>\
<style>\
  body {\
    font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, \
    Ubuntu, Cantarell, 'Open Sans', 'Helvetica Neue', sans-serif;\
    font-size: 2rem;\
  }\
label {\
  display: inline-block;\
  width: 3.5em;\
  margin-bottom: 1rem;\
}\
input {\
  font-size: 2rem;\
}\
input[type=\"text\"] {\
  width: 6em;\
}\
input[type=\"number\"] {\
  width: 2.5em;\
}\
button {\
  font-size: 2rem;\
  padding: 1rem;\
}\
#working {\
  border: 1px solid black;\
  font-size: 10vh;\
  margin: 2rem auto;\
  padding: 0.5em;\
  text-align: center;\
  width: 70vw;\
}\
</style>\
<body>\
  <form id=\"ring-form\">\
    <p>\
    <label for=\"rgb\">RGB</label>\
    <input type=\"text\" id=\"rgb\" name=\"rgb\">\
    </p>\
    <label for=\"red\">Red</label> \
    <input type=\"number\" min=\"0\" max=\"255\" value=\"0\" id=\"red\" name=\"red\" autofocus><br />\
    <label for=\"green\">Green</label> \
    <input type=\"number\" min=\"0\" max=\"255\" value=\"0\" id=\"green\" name=\"green\"><br />\
    <label for=\"blue\">Blue</label> \
    <input type=\"number\" min=\"0\" max=\"255\" value=\"0\" id=\"blue\" name=\"blue\"><br />\
    <button>Colour the Ring</button>\
    <div id=\"working\">&nbsp;</div>\
  </form>\
 \
  <script>\
const ringForm = document.getElementById('ring-form');\
const redElement = document.getElementById('red');\
const greenElement = document.getElementById('green');\
const blueElement = document.getElementById('blue');\
const rgbElement = document.getElementById('rgb');\
const working = document.getElementById('working');\
ringForm.addEventListener('submit', (e) => {\
  e.preventDefault();\
  working.textContent = 'Working';\
  const formData = new FormData();\
  const rgbValue = rgbElement.value;\
  if(rgbValue && /#?[0-9a-f]{6}/i.test(rgbValue)) {\
    if (rgbValue[0] !== '#') rgbElement.value = `#${rgbValue}`;\
    \
    const rgbArray = rgbValue.match(/#?(..)(..)(..)/);\
    formData.append('red', parseInt(rgbArray[1], 16));\
    formData.append('green', parseInt(rgbArray[2], 16));\
    formData.append('blue', parseInt(rgbArray[3], 16));\
  }\
  else {\
    formData.append('red', redElement.value);\
    formData.append('green', greenElement.value);\
    formData.append('blue', blueElement.value);\
  }\
  fetch('/ring', {\
    body: formData,\
    method: 'post',\
  }).then(response => { working.innerHTML = '&nbsp;'; });\
});\
  </script>\
</body>\
</html>";


ESP8266WebServer server(80);
CRGB leds[NUM_LEDS];

byte red = 40;
byte green = 40;
byte blue = 40;

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/ring", HTTP_POST, handlePOST);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");

  FastLED.addLeds<WS2812B, LED_DATA_PIN, GRB>(leds, NUM_LEDS);
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}

void handleRoot() {
  server.send(200, "text/html", html);

//  char temp[400];
//
//  snprintf(temp, 400,
//"<html>\
//  <head>\
//    <meta http-equiv='refresh' content='5'/>\
//    <title>ESP8266 LED</title>\
//    <style>\
//      body { background-color: #1e4147; font-family: Arial, Helvetica, sans-serif; color: #f1e3b4; }\
//    </style>\
//  </head>\
//  <body>\
//    <h1>Hello from ESP8266 Ring Server</h1>\
//  </body>\
//</html>");
//
//  server.send(200, "text/html", temp);
}

void handlePOST() {
  int args = server.args();
//  int headers = server.headers();
//  
//  Serial.print("/post, headers: ");
//  Serial.print(headers);
//  Serial.print(", args: ");
//  Serial.println(args);
// 
//  if (headers) {
//    for(int i = 0; i < headers; ++i) {
//      Serial.println("  " + server.headerName(i) + ": " + server.header(i));
//    }
//  }
//  
//  if (args) {
//    for(int i = 0; i < args; ++i) {
//      Serial.println("  " + server.argName(i) + ": " + server.arg(i));
//    }
//  }

  if (args) {
    if (server.hasArg("red")) {
      red = server.arg("red").toInt();
    }

    if (server.hasArg("green")) {
      green = server.arg("green").toInt();
    }

    if (server.hasArg("blue")) {
      blue = server.arg("blue").toInt();
    }
    
    for(int i = 0; i < NUM_LEDS; ++i) {
      leds[i] = CRGB(red, green, blue);
    }

    FastLED.show();

    return server.send(200, "application/json", "{ \"OK\": true }");
  }
  
  server.send(200, "application/json", "{ \"OK\": false }");
}

void handleNotFound() {
//  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}
