#include <U8glib.h>

// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"

#define DHTPIN 2     // what digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
// #define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
// #define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

U8GLIB_ST7920_128X64_1X u8g(10);    // Hardware SPI, 10 = CS/CE, 11 = MOSI, 13 = SCK

float h, t, f, hif, hic;

void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");

  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(1000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print("*C ");
  Serial.print(f);
  Serial.print("*F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print("*C ");
  Serial.print(hif);
  Serial.println("*F");

  draw_loop();
}


////////////////////////////////////////////////////////////////////////

void draw_loop() {
  u8g.firstPage();
  
  do {
    draw();
  } 
  while(u8g.nextPage());
}

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here

  u8g.setFont(u8g_font_helvR10);
  u8g.drawStr(20, 12, "Temperature");

  u8g.setFont(u8g_font_helvB12);

  u8g.setPrintPos(0, 29);
  u8g.print(t);
  u8g.print("\260C");

  u8g.setPrintPos(70, 29);
  u8g.print(f);
  u8g.print("\260F");

  u8g.setPrintPos(0, 43);
  u8g.print(hic);
  u8g.print("\260C");

  u8g.setPrintPos(70, 43);
  u8g.print(hif);
  u8g.print("\260F");

  u8g.setFont(u8g_font_helvR10);
  u8g.drawStr(8, 62, "Humidity");

  u8g.setFont(u8g_font_helvB12);
  u8g.setPrintPos(70, 63);
  u8g.print(h);
  u8g.print("%");
}

