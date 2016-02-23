#include <Wire.h>

const byte MCP_BASE   = 0x20;  // MCP23017

// IOCON.BANK = 0 addressing

const byte IODIRA     = 0x00;
const byte IODIRB     = 0x01;
const byte GPPUA      = 0x0c;
const byte GPPUB      = 0x0d;
const byte GPIOA      = 0x12;
const byte GPIOB      = 0x13;

class MCP23017 {
public:

  void begin(byte address = 0) {
    m_address = MCP_BASE + (address & 0x07);

    Wire.begin();
  }

  void pinMode(byte pin, byte mode, boolean pullup = false);
  byte read_pin(byte pin);
  void write_pin(byte pin, boolean set);

  void write_IODIR(byte value, byte ab = 0);
  void write_GPIO(byte value, byte ab = 0);
  byte read_GPIO(byte ab = 0);
    
private:

  byte m_address;

  byte readRegister(byte address);
  void writeRegister(byte address, byte value);
  void updateRegisterBit(byte bit, boolean set, byte address);
};

