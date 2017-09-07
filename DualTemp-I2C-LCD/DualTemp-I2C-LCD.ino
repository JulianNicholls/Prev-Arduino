// Include the libraries we need
#include <avr/sleep.h>

#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"
#include <LiquidCrystal_I2C.h>

#define BUTTON        2     // Pushbutton for wakeup
#define DHT11_PIN     8     // DHT11 is on this pin
#define DS18B20_PIN  10     // Dallas 18B20 is on this pin

#define DHTTYPE      DHT11  // DHT 11

// Set up for the Dallas DS18B20
OneWire oneWire(DS18B20_PIN);
DallasTemperature ds18B20(&oneWire);

// Set up for the DHT11
DHT dht11(DHT11_PIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON, INPUT);
  
  dht11.begin();
  ds18B20.begin();
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);    // Turn the LED on
  lcd.init();

  // Both sensors return Celsius by default
  float d11h  = dht11.readHumidity();
  float d11t  = dht11.readTemperature();
  float d11hi = dht11.computeHeatIndex(d11t, d11h, false);  // isFahrenheit = false
  
  ds18B20.requestTemperatures();
  float ds18bt = ds18B20.getTempCByIndex(0);

  digitalWrite(LED_BUILTIN, LOW);     // Turn the LED off

  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("O: ");
  lcd.print(ds18bt, 1);
  lcd.print("C I: ");
  lcd.print(d11t, 1);
  lcd.print("C ");

  lcd.setCursor(0, 1);
  lcd.print("H: ");
  lcd.print(d11h, 1);
  lcd.print("% F: ");
  lcd.print(d11hi, 1);

  delay(5000);  // Show data for 5s
  lcd.noBacklight();
//  powerDown();
}

void powerDown() {
  sleep_enable();

  attachInterrupt(digitalPinToInterrupt(BUTTON), wakeUp, LOW);

  set_sleep_mode(SLEEP_MODE_ADC);

  sleep_mode();
  // At this point we're asleep...

  // ...On waking

  sleep_disable();
}

// Stop sleeping and detach the interrupt to stop it from 
// repeatedly occurring
void wakeUp() {
  sleep_disable();
  detachInterrupt(digitalPinToInterrupt(BUTTON));  
}

