// Include these libraries
#include "RF24.h"
#include "FastLED.h"
#include "SoftwareSerial.h"
SoftwareSerial mySerial(-1,3);

// Define LED, CE and CSN pin values for ATtiny85
#define LED_PIN 4 //Physical pin 1
#define CE_PIN 3 //Physical pin 2
#define CSN_PIN 3 //Physical pin 3

// Define the number of LEDs
#define NUM_LEDS 1 //How many LEDs are we driving

// Define the pixel address (an integer where 0 addresses all pixels, and 1-255 addresses individual pixels)
int pixelAddress = 2;

// Define the array of leds
CRGB leds[NUM_LEDS];

// Define the radio object
RF24 radio(CE_PIN, CSN_PIN);

// Define radio addresses - this is an array of addresses, each address can be 40bits (5bytes) long so why are they 6 bytes in the example?
byte addresses[][6] = {"PIPE1","PIPE2"};

byte receivedData[5] = {0,0,0,0,0}; // Array to hold data received by radio

bool processLEDData = false;

void setup() {

  mySerial.begin( 9600 );
  mySerial.println("Starting setup");
  
  // Setup and configure rf radio
  radio.begin(); // Start up the radio
  //radio.setAutoAck(1); // Ensure autoACK is enabled //enabled by default so this is redundant
  radio.setRetries(15,15); // Max delay between retries & number of retries //defaults are setRetries(5,15); (Line 635 RF24.cpp)
  radio.openWritingPipe(addresses[1]); // Write to device address '2' //void openWritingPipe (uint64_t address)
  radio.openReadingPipe(1,addresses[0]); // Read on pipe 1 for device address '1' //void openReadingPipe (uint8_t number, uint64_t address)
  radio.startListening(); // Start listening
  //radio.printDetails(); //Does this output debug to software serial? Nope :( How do we define where this prints to?
  // Setup LED
  // Choose your LED type - most likey it will be WS2812B
  // but am including a few others for convenience as they
  // might work as well.
  FastLED.addLeds<WS2811, LED_PIN, RGB>(leds, NUM_LEDS);
  //FastLED.addLeds<WS2812, LED_PIN, RGB>(leds, NUM_LEDS);
  //FastLED.addLeds<WS2812B, LED_PIN, RGB>(leds, NUM_LEDS);
  //FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  mySerial.println("Setup complete");



}

void loop() {

  unsigned long started_waiting_at = micros(); // Set up a timeout period, get the current microseconds
  boolean timeout = false; // Set up a variable to indicate if a response was received or not

  while( !radio.available() ) // While nothing is received
  { 
    //mySerial.println("Nothing received");
    if (micros() - started_waiting_at > 200000 ) // If waited longer than 200ms, indicate timeout and exit while loop
    { 
      timeout = true;
      //mySerial.println("Timing out");
      break;
    }

  }

  
  
  if ( !timeout )
  { 
    //mySerial.println("Received something...");
    radio.read( &receivedData, sizeof(receivedData) );
    for(int i=0; i < 5; i++)
    {
      mySerial.print(i);
      mySerial.print(":");
      mySerial.println(receivedData[i]);
    }
    processLEDData = true;
    
  }
  
  if (receivedData[4] == pixelAddress || receivedData[4] == 0 && processLEDData) //if the address received matches our assigned address or the default address, set the pixel to the received values
  {
    //byte receivedData[4] = {100,100,100,0};
    mySerial.println("Setting the LED to received values...");
    // Turn the LED on
    leds[0].red = receivedData[1];
    leds[0].green = receivedData[2];
    leds[0].blue = receivedData[3];
    FastLED.show();
    processLEDData = false;
  }
  //delay(50);

  //leds[0] = CRGB::Blue;
  //FastLED.show();
  //delay(500);
  
  // Turn the LED off
  //leds[0] = CRGB::Black;
  //FastLED.show();
  //delay(500);

}
