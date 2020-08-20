// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"

#define DHT11_PIN    8     // DHT11 is on this pin
#define DS18B20_PIN  10    // Dallas 18B20 is on this pin

#define DHTTYPE     DHT11 // DHT 11

// Set up for the Dallas
OneWire oneWire(DS18B20_PIN);
DallasTemperature ds18B20(&oneWire);

// Set up for the DHT11
DHT dht11(DHT11_PIN, DHTTYPE);

void setup() {
  Serial.begin(9600);

  dht11.begin();
  ds18B20.begin();
}

void loop() {
  // Both sensors return Celsius by default

  float d11h  = dht11.readHumidity();
  float d11t  = dht11.readTemperature();
  float d11hi = dht11.computeHeatIndex(d11t, d11h, false);  // isFahrenheit = false
  
  ds18B20.requestTemperatures();
  float ds18bt = ds18B20.getTempCByIndex(0);

  Serial.print(d11t, 1);
  Serial.print(" ");
  Serial.print(ds18bt, 1);
  Serial.print(" ");
  Serial.print(d11h, 1);
  Serial.print(" ");
  Serial.println(d11hi, 1);

  delay(2000);  // Wait 2 seconds for more readings
}

