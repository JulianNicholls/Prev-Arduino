
/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <SPI.h>
#include "RF24.h"

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 0;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
/**********************************************************/

byte addresses[][6] = {"PIPE1","PIPE2"};

// Used to control whether this node is sending or receiving
bool role = 0;

int r,g,b;

void setup() {
  Serial.begin(115200);
  Serial.println("WirelessRGBPixel Control v1");
  Serial.println("Commands available:");
  Serial.println("Change colour: CR,G,B");
  Serial.println("Random colour: R");
  Serial.println("Turn off LEDs: O");
  Serial.println("Show this help info: H/?");
  Serial.println("Fade to black: Not implemented");
  Serial.println("Blend to colour: Not implemented");
  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
  
  // Start the radio listening for data
  radio.startListening();
}

void loop() 
{
  if ( Serial.available() )
  {
    char firstReceivedByte = toupper(Serial.read());
    
    switch(firstReceivedByte)
    {
      case 'C': //If the first char received is a 'C' that indicates a change colour CRGB command so get the rest of it and send it
      {
        Serial.println("Colour change command: Cn,n,n,a");
        byte sendRGBData[5] = {'C',0,0,0,0};  //Change colour command: C,R,G,B,a (defaults to off) where a is the pixel address 0-255 (0 is default and address all pixels)
        if (Serial.available() >= 2) 
        {
          for (int i=1; i<5; i++) 
          {
            sendRGBData[i] = Serial.parseInt(); 
            //sendRGBData[i] = Serial.read();
          }
        }

        for (int i=1; i<5; i++) 
        {
          Serial.print("Sending :");
          Serial.println(sendRGBData[i]);
        }
        
        radio.stopListening();
        // Send data over radio. Blocks until done
        if (!radio.write( &sendRGBData, sizeof(sendRGBData) ))
        {
          //Serial.println(F("failed"));
        }
        break; 
      }
      
      case 'R': //If the first char received is a 'R' that indicates a RANDOM change colour CRGB command so generate the rest of it and send it
      {
        Serial.println("Random Colour change command: R");
        byte sendRGBData[5] = {'C',0,0,0,0};  //Change colour command data (defaults to off)

        for (int i=1; i<4; i++) 
        {
          sendRGBData[i] = random(0,256); 
          //sendRGBData[i] = Serial.read();
        }

        for (int i=1; i<5; i++) 
        {
          Serial.print("Sending :");
          Serial.println(sendRGBData[i]);
        }

        radio.stopListening();
        // Send data over radio. Blocks until done
        if (!radio.write( &sendRGBData, sizeof(sendRGBData) ))
        {
          //Serial.println(F("failed"));
        }
        break; 
      }
      
      case 'O': //If the first char received is a 'O' turn off LEDs
      {
        Serial.println("Turn off LEDs command: O");
        byte sendRGBData[5] = {'C',0,0,0,0};  //Change colour command data (defaults to off)

        radio.stopListening();
        // Send data over radio. Blocks until done
        if (!radio.write( &sendRGBData, sizeof(sendRGBData) ))
        {
          //Serial.println(F("failed"));
        }
        break; 
      }
      
      case 'F': //Fade to black FN(F = fade command, N = amount to fade by)
      {
        Serial.println("Not yet implemented");
        break; 
      }
      
      case 'B': //Blend colour from current colour to BRGB
      {
        Serial.println("Not yet implemented"); 
        break; 
      }
      
      case '\r': //Control char, do nothing
      {
        //Serial.println("Control char"); 
        break; 
      }
      
      case '\n': //Control char, do nothing
      {
        //Serial.println("Control char"); 
        break; 
      }
      
      case 'H': //Show help
      case '?': //Show help
      {
        Serial.println("WirelessRGBPixel Control v1");
        Serial.println("Commands available:");
        Serial.println("Change colour: CR,G,B");
        Serial.println("Random colour: R");
        Serial.println("Turn off LEDs: O");
        Serial.println("Show this help info: H/?");
        Serial.println("Fade to black: Not implemented");
        Serial.println("Blend to colour: Not implemented");
        Serial.println("");
        break;
      }
     
      default: //Command not recoginsed, do nothing
      {
        Serial.println("Input not recognised");
        break;  
      }
    }
  }
} // Loop

