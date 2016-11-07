// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"

#define DHT11_PIN   8     // DHT11 is on this pin
#define DS8820_PIN  10    // Dallas 8820 is on this pin

#define DHTTYPE     DHT11 // DHT 11

// Set up for the Dallas
OneWire oneWire(DS8820_PIN);
DallasTemperature ds8820(&oneWire);

// Set up for the DHT11
DHT dht11(DHT11_PIN, DHTTYPE);

void setup() {
  Serial.begin(9600);

  dht11.begin();
  ds8820.begin();
}

void loop() {
  // Both sensors return Celsius by default

  float d11h  = dht11.readHumidity();
  float d11t  = dht11.readTemperature();
  float d11hi = dht11.computeHeatIndex(d11t, d11h, false);  // isFahrenheit = false
  
  ds8820.requestTemperatures();
  float ds8t = ds8820.getTempCByIndex(0);

  Serial.print(d11t, 1);
  Serial.print(" ");
  Serial.print(ds8t, 1);
  Serial.print(" ");
  Serial.print(d11h, 1);
  Serial.print(" ");
  Serial.println(d11hi, 1);

  delay(2000);  // Wait 2 seconds for more readings
}

