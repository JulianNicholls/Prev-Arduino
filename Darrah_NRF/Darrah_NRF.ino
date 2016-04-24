
//NRF24L01+ Test Code
// Kevin Darrah
// V2
//www.kevindarrah.com


//NRF      ARDUINO
//1 GND    GND
//2 VCC    3.3V
//3 CE     4  out
//4 CSN    5  out
//5 SCK    13 out
//6 MOSI   11 out
//7 MISO   12 in
//8 IRQ    2  in

// Define your pins here!
#include <SPI.h>
#define CE_pin 4
#define CSN_pin 5
#define IRQ_pin 2
#define MOSI_pin 11
#define MISO_pin 12
#define SCK_pin 13 

//The global variables used by everyone
byte data_in[5], data2, data3;

///////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200); // start Serial
  Serial.println("Setting Up");
  delay(100);
  
  NRF_Init();                   // set up the SPI and define pins
  NRF_set_RX_payload(0, 3);     // pipe 0-5, bytes 1-32
  NRF_get_address(7, 1);        // Get Reg7 Status, 1=print screen
  NRFwrite_bit_write(0, 0, 1);  // register#, bit#, and value 0 or 1, ::  0,0,1 RX Mode
  NRFwrite_bit_write(0, 1, 1);  // register, bit, and value 0,1,1 PowerUP
  NRFwrite_bit_write(0, 4, 1);  // RT Mask turns off the RT interrupt
  NRFwrite_bit_write(0, 5, 1);  // TX Mask turns off the TX interrupt
  
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(B11100010);//flush RX
  digitalWrite(CSN_pin, HIGH);
  
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(B11100001);//flush TX
  digitalWrite(CSN_pin, HIGH);
  
  NRF_ClearInterrupts();//clears any interrupts
  delay(100);
  attachInterrupt(0, get_data, FALLING);//kick things off by attachin the IRQ interrupt

  NRFpinmode(3, 0);   // pinmode routine :: pin number, 0=Input 1=Output
  NRFpinwrite(3, 1);  // pinwrite routine :: pin number, 0=LOW 1=HIGH
}

///////////////////////////////////////////////////////////////////////////
void loop() {
//  NRF_ping();//ping random byte out
//  Serial.println(NRFpinread(3));//NRF pin read is same as digital read returns 1 or 0


  if(NRFpinread(3)==0){
    NRFpinmode(7, 1);   // pinmode pin 7 as output
    NRFpinwrite(7, 1);  // pinwrite pin 7 HIGH
    delay(100);
    
    NRFpinwrite(7, 0);  // pinwrite pin 7 LOW
    delay(100);
  }

}

///////////////////////////////////////////////////////////////////////////
void transmit(byte mode, byte pin, byte value) {
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(B11100001); // flush TX, get rid of anything that might be in there
  digitalWrite(CSN_pin, HIGH);  
  
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(B10100000); // load TX payload
  data_in[1] = SPI.transfer(mode);      // action digital Read
  data_in[2] = SPI.transfer(pin);       // pin number
  data_in[3] = SPI.transfer(value);
  digitalWrite(CSN_pin, HIGH);
   
  digitalWrite(CE_pin, LOW);
  delay(1); // small delay
  NRFwrite_bit_write(0, 0, 0);  // go into TX mode
  delay(1); // small delay
  digitalWrite(CE_pin, HIGH);
  
  delay(1); // this is the time CE pin must be HIGH for before going back into RX mode
  //delay(1) seems to work best for this.  any longer or shorter doesn't work as well
  NRFwrite_bit_write(0, 0, 1);//go back into RX mode
}

///////////////////////////////////////////////////////////////////////////
// this routine is called when the IRQ pin is pulled LOW by the NRF
void get_data() {
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(B01100001);//read the payload
  data_in[1] = SPI.transfer(B00000000);
  data_in[2] = SPI.transfer(B00000000);
  data_in[3] = SPI.transfer(B00000000);
  digitalWrite(CSN_pin, HIGH);

  if(data_in[1] == 1) { // data starting with '1' sets up the pinmode
    if(data_in[3] == 0) // data3 is the mode of the pin 0=in 1=out
      pinMode(data_in[2], INPUT); // data2 is the pin, so set the pin
      
    if(data_in[3] == 1)
      pinMode(data_in[2], OUTPUT);
      
    delay(10);  // very important delay - this lets the transmitter finish 
                // up what is was doing before sending data back
    transmit(3, data_in[2], data_in[3]);  // send the information back for verification
  }
  else if(data_in[1 ]== 2) {  // data starting with '2' sets writes to the pin
    if(data_in[3] == 0)//data3 is the value of the pin, 0=LOW 1=HIGH
      digitalWrite(data_in[2], LOW);
      
    if(data_in[3]==1)
      digitalWrite(data_in[2], HIGH);
      
    delay(10);
    transmit(3, data_in[2], data_in[3]);  // send back for verification
  }
  else if(data_in[1] == 3) {  // echo back used to verify the right data was sent
    data2 = data_in[2];
    data3 = data_in[3];
  }
  else if(data_in[1] == 4) {  // 4 is used to do digital reads
     delay(10);
     transmit(3,data_in[2],digitalRead(data_in[2]));//everything comes back with the echo
  }
  else if(data_in[1] == 5) {  // not yet implemented, will be for analog reads probably
  }  
  else if(data_in[1] == 6) {  // ping transmit
    delay(10);
    transmit(3,data_in[2],data_in[3]);  // send ping back
  }
  else if(data_in[1] == 7) {  // not yet implemented, will be for dedicated function like temp reads
  }
  else {
    Serial.println("No Mode Byte Identified!");//this is printed if a mode was not defined
    for(int i = 1; i < 4; ++i)
    Serial.print(char(data_in[i]));//just print out whatever was recieved
    Serial.println("  ");
  }
  
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(B11100010); // flush RX
  digitalWrite(CSN_pin, HIGH);

  NRFwrite_bit_write(7,6,1);//clear the RX interrupt flag

}

///////////////////////////////////////////////////////////////////////////
void NRF_Init() { 
  pinMode(CE_pin, OUTPUT);    // chip enable set as output
  pinMode(CSN_pin, OUTPUT);   // chip select pin as output
  pinMode(MOSI_pin, OUTPUT);  // SPI data out
  pinMode(MISO_pin, INPUT);   // SPI data in
  pinMode(SCK_pin, OUTPUT);   // SPI clock out
  
  Serial.println("NRF Pins Initialized");
  
  SPI.setBitOrder(MSBFIRST);            // SPI Most Significant Bit First
  SPI.setDataMode(SPI_MODE0);           // Mode 0 Rising edge of data, keep clock low
  SPI.setClockDivider(SPI_CLOCK_DIV2);  // Run the data in at 16MHz/2 - 8MHz
  
  digitalWrite(CE_pin, HIGH);           // RX mode
  digitalWrite(CSN_pin, HIGH);          // SPI idle
  SPI.begin();                          // start up the SPI library
  
  Serial.println("NRF Ready");
}

///////////////////////////////////////////////////////////////////////////
void NRF_set_RX_payload(byte pipe, byte bytes) {
  byte address = pipe + 32 + 16 + 1;  // a register write starts at 32, so add on the 1 and 16 to get you to at R17
  
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(address);//write register 11 RX_PW_P0
  data_in[1] = SPI.transfer(bytes);//3 bytes for now
  digitalWrite(CSN_pin, HIGH);
  
  Serial.print("RX Payload Set RX_PW_P");
  Serial.print(pipe);
  Serial.print(" for ");
  Serial.print(bytes);
  Serial.println(" bytes");
}

///////////////////////////////////////////////////////////////////////////
// This routine writes single bits of a register, without affecting the rest of the register
void NRFwrite_bit_write(byte address, byte bit_add, byte val) {
  NRF_get_address(address, 0);  // first read out the register
  if(val == 1)  // if we want to write a one to the bit then set the bit in the register we read
    bitSet(data_in[1],bit_add);
  else
    bitClear(data_in[1],bit_add); //clear it if not
  
  digitalWrite(CSN_pin, LOW);             // now we'll write the modified data back in
  data_in[0] = SPI.transfer(32+address);  // a write to a register adds 32
  data_in[1] = SPI.transfer(data_in[1]);  // write the modified register
  digitalWrite(CSN_pin, HIGH);
}

///////////////////////////////////////////////////////////////////////////
// there are three interrupt flags in the NRF.  Thsi routine checks them, and if set, it will clear them
void NRF_ClearInterrupts() { 
  NRF_get_address(7, 0);      // RT interrupt
  if(bitRead(data_in[1], 4))
  NRFwrite_bit_write(7, 4, 1);
  
  NRF_get_address(7, 0);      // TX interrupt
  if(bitRead(data_in[1], 5))
  NRFwrite_bit_write(7, 5, 1);
  
  NRF_get_address(7, 0);      // RX interrupt
  if(bitRead(data_in[1], 6))
  NRFwrite_bit_write(7, 6, 1);
}

///////////////////////////////////////////////////////////////////////////
void NRF_ping() {
  int ping = random(256);//get a random byte
  Serial.print("Pinging with  ");
  Serial.print(ping);
  
  transmit(6, ping, ping);  // send the ping out starting with 6
  delay(15);                // give it a little
  if(data2 == ping)         //see if data2 came back with the ping
    Serial.println(" PING Successfull!! ");
  else
    Serial.println(" PING FAIL!! ");
  data2 = 0;        // reset
}

///////////////////////////////////////////////////////////////////////////
void NRFpinmode(byte pin, byte mode) {
  for(int i = 0; i < 10; ++i) { // We give it 10 tries to set the pinmode
    transmit(1, pin, mode);           // send out with '1' the pin and the mode
    delay(15);                        // give a little time to respond
    if(data2 == pin && mode == data3) // if we get the right data back,
      break;                          // force out of the for loop
 
  if(i == 9)                // if we ever get too far along, at == 9, then we never set the pin
    Serial.println("Failed to set mode");
  }
  
  data2 = 0;    //reset
  data3 = 0;
}

///////////////////////////////////////////////////////////////////////////
void NRFpinwrite(byte pin, byte val) {
  for(int i = 0; i < 10; ++i) {
    transmit(2, pin, val);//starts with 2 here
    delay(15);
    if(data2 == pin && val == data3)
      break;
  
  if( i== 9)
    Serial.println("Failed to write pin");
  }
  
  data2 = 0;
  data3 = 0;
}

///////////////////////////////////////////////////////////////////////////
byte NRFpinread(byte pin){
  for(int i = 0; i < 10; ++i) {   //same as teh other routines here
    transmit(4, pin, pin);  // Starts with 4, but we just want the digitalread of the pin
    delay(15);
    if(data2 == pin) {      // if we get the pin back, then we know data3 is the pin value
      return(data3);  // gives back 0 or 1
    }
    
  }

  Serial.println("Failed to read pin");

  data2 = 0;
  data3 = 0;

  return -1;
}


///////////////////////////////////////////////////////////////////////////
//     MASSIVE ROUTINE HERE - PUT ALL ROUTINES ABOVE THIS

void NRF_get_address(byte address, byte info){// START Get Address   START Get Address   START Get Address  
  //send the address and either a 1 or 0 if you want to do a serial print of the address
  //after a call to this routine, data_in[1] will equal the address you called
  
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(address);
  data_in[1] = SPI.transfer(B00000000);
  digitalWrite(CSN_pin, HIGH);
  if(info==1){// if the user wanted it, you will get a print out of the register - good fo debugging
  Serial.print("R");
  Serial.print(address);
  switch (address) {
  case 0:
   Serial.print(" CONFIG REGISTER: ");
   Serial.println(data_in[1]);
   Serial.print("PRIM_RX: ");
   if(bitRead(data_in[1],0))
   Serial.println("PRX");
   else
   Serial.println("PTX");
   
   Serial.print("PWR_UP: ");
   if(bitRead(data_in[1],1))
   Serial.println("ON");
   else
   Serial.println("OFF");  
   
    Serial.print("CRCO: ");
   if(bitRead(data_in[1],2))
   Serial.println("2Bytes");
   else
   Serial.println("1Byte");   
   
   Serial.print("EN_CRC: ");
   if(bitRead(data_in[1],3))
   Serial.println("En");
   else
   Serial.println("Dis");  
 
    Serial.print("MASK_MAX_RT: ");
   if(bitRead(data_in[1],4))
   Serial.println("No IRQ");
   else
   Serial.println("Active Low");  
 
   Serial.print("MASK_TX_DS: ");
   if(bitRead(data_in[1],5))
   Serial.println("No IRQ");
   else
   Serial.println("Active Low");  
   
   Serial.print("MASK_RX_DR: ");
   if(bitRead(data_in[1],6))
   Serial.println("No IRQ");
   else
   Serial.println("Active Low");  
   break;//0
case 1:
   Serial.print(" EN_AA REGISTER: ");
   Serial.println(data_in[1]);
break;//1
case 2:
   Serial.print(" EN_RXADDR: ");
   Serial.println(data_in[1]);
break;//2
case 3:
   Serial.print(" SETUP_AW: ");
   Serial.println(data_in[1]);
break;//3
case 4:
   Serial.print(" SETUP_RETR: ");
   Serial.println(data_in[1]);
break;//4
case 5:
   Serial.print(" RF_CH: ");
   Serial.println(data_in[1]);
break;//5
case 6:
   Serial.print(" RF_SETUP: ");
   Serial.println(data_in[1]);
   Serial.print("Power: ");
   Serial.print(bitRead(data_in[1],2));
   Serial.println(bitRead(data_in[1],1));
   Serial.print("RF_DR_HIGH: ");
   Serial.println(bitRead(data_in[1],3));
   Serial.print("PLL_LOCK: ");
   Serial.println(bitRead(data_in[1],4));
   Serial.print("RF_DR_LOW: ");
   Serial.println(bitRead(data_in[1],5));  
   Serial.print("CONT_WAVE: ");
   Serial.println(bitRead(data_in[1],7));   
break;//6
case 7:
   Serial.print(" STATUS: ");
   Serial.println(data_in[1]);
   Serial.print("TX_FULL: ");
   if(bitRead(data_in[1],0))
   Serial.println("Full");
   else
   Serial.println("Not full");
   
   Serial.print("RX_P_NO: ");
   if(bitRead(data_in[1],1) && bitRead(data_in[1],2) && bitRead(data_in[1],3))
    Serial.println("RX FIFO Empty");
   else
   Serial.println(bitRead(data_in[1],1)+(bitRead(data_in[1],2)<<1)+(bitRead(data_in[1],2)<<2)); 
   Serial.print("MAX_RT IRQ: ");
   Serial.println(bitRead(data_in[1],4));
   Serial.print("TX_DS IRQ: ");
   Serial.println(bitRead(data_in[1],5));
   Serial.print("RX_DR IRQ: ");
   Serial.println(bitRead(data_in[1],6));
break;//7
case 8:
   Serial.print(" OBSERVE_TX: ");
   Serial.println(data_in[1]);
   Serial.print("ARC_CNT: "); 
   Serial.println(bitRead(data_in[1],0)+(bitRead(data_in[1],1)<<1)+(bitRead(data_in[1],2)<<2)+(bitRead(data_in[1],3)<<3));
   Serial.print("PLOS_CNT: "); 
   Serial.println(bitRead(data_in[1],4)+(bitRead(data_in[1],5)<<1)+(bitRead(data_in[1],6)<<2)+(bitRead(data_in[1],7)<<3));
break;//8
case 9:
   Serial.print(" RPD REGISTER: ");
   Serial.println(bitRead(data_in[1],0));
break;//9
case 10:
   Serial.print(" RX_ADDR_P0 LSB: ");
   Serial.println(data_in[1]);
break;//10
case 11:
   Serial.print(" RX_ADDR_P1 LSB: ");
   Serial.println(data_in[1]);
break;//11
case 12:
   Serial.print(" RX_ADDR_P2 LSB: ");
   Serial.println(data_in[1]);
break;//12
case 13:
   Serial.print(" RX_ADDR_P3 LSB: ");
   Serial.println(data_in[1]);
break;//13
case 14:
   Serial.print(" RX_ADDR_P4 LSB: ");
   Serial.println(data_in[1]);
break;//14
case 15:
   Serial.print(" RX_ADDR_P5 LSB: ");
   Serial.println(data_in[1]);
break;//15
case 16:
   Serial.print(" TX_ADDR LSB: ");
   Serial.println(data_in[1]);
break;//16
case 17:
   Serial.print(" RX_PW_P0 RX payload: ");
   Serial.println(data_in[1]);
break;//17
case 18:
   Serial.print(" RX_PW_P1 RX payload: ");
   Serial.println(data_in[1]);
break;//18
case 19:
   Serial.print(" RX_PW_P2 RX payload: ");
   Serial.println(data_in[1]);
break;//19
case 20:
   Serial.print(" RX_PW_P3 RX payload: ");
   Serial.println(data_in[1]);
break;//20
case 21:
   Serial.print(" RX_PW_P4 RX payload: ");
   Serial.println(data_in[1]);
break;//21
case 22:
   Serial.print(" RX_PW_P5 RX payload: ");
   Serial.println(data_in[1]);
break;//22

case 23:
   Serial.print(" FIFO_STATUS: ");
   Serial.println(data_in[1]);
   Serial.print("RX_EMPTY: ");
   if(bitRead(data_in[1],0))
   Serial.println("Empty");
   else
   Serial.println("Data");
   
   Serial.print("RX_FULL: ");
   if(bitRead(data_in[1],1))
   Serial.println("Full");
   else
   Serial.println("Available");
 
    Serial.print("TX_EMPTY: ");
   if(bitRead(data_in[1],4))
   Serial.println("Empty");
   else
   Serial.println("Data");  
  
      Serial.print("TX_FULL: ");
   if(bitRead(data_in[1],5))
   Serial.println("Full");
   else
   Serial.println("Available"); 
   Serial.print("TX_REUSE: ");
   Serial.println(bitRead(data_in[1],6));
break;//23
  }//switch
  }//if 1
}// END get_address END get_address END get_address END get_address END get_address END get_address END get_address
  
  

  
  

