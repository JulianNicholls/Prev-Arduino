#include <Wire.h>

const byte MCP_BASE     = 0x20;  // MCP23017

// IOCON.BANK = 0 addressing

const byte IODIRA       = 0x00;
const byte IODIRB       = 0x01;
const byte GPIOA        = 0x12;
const byte GPIOB        = 0x13;

class MCP23017 {
public:

  void begin(byte address = 0) {
    m_address = MCP_BASE + (address & 0x07);

    Wire.begin();
  }

  void pinMode(byte pin, byte mode);
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

void MCP23017::write_IODIR(byte value, byte ab) {
  byte address = ab ? IODIRB : IODIRA;

  writeRegister(address, value);
}

void MCP23017::write_GPIO(byte value, byte ab) {
  byte address = ab ? GPIOB : GPIOA;

  writeRegister(address, value);
}

void MCP23017::pinMode(byte pin, byte mode) {
  byte ab       = pin >> 3;
  byte pin_bit  = pin & 7;

  updateRegisterBit(pin_bit, mode == INPUT, ab ? IODIRB : IODIRA);
}

void MCP23017::write_pin(byte pin, boolean set) {
  byte ab       = pin >> 3;
  byte pin_bit  = pin & 7;

  updateRegisterBit(pin_bit, set, ab ? GPIOB : GPIOA);
}

void MCP23017::updateRegisterBit(byte bit, boolean set, byte address) {
  byte cur = readRegister(address);

  bit = 1 << bit;
  
  if(set)
    cur |= bit;
  else
    cur &= ~bit;

  writeRegister(address, cur);
}

byte MCP23017::readRegister(byte address) {    
  Wire.beginTransmission(m_address);
  Wire.write(address);
  Wire.endTransmission();
  
  Wire.requestFrom((uint8_t) m_address, (uint8_t) 1);
  return Wire.read();
}

void MCP23017::writeRegister(byte address, byte value) {    
  Wire.beginTransmission(m_address);
  Wire.write(address);
  Wire.write(value);
  Wire.endTransmission();
}

//////////////////////////////////////////////////////////////////////////

MCP23017 mcp;

void setup() {
  mcp.begin();
  
//  mcp.write_IODIR(0, 0);

  mcp.pinMode(0, OUTPUT);
  mcp.pinMode(1, OUTPUT);
  mcp.pinMode(2, OUTPUT);
  mcp.pinMode(3, OUTPUT);
  mcp.pinMode(4, OUTPUT);
  mcp.pinMode(5, OUTPUT);
  mcp.pinMode(6, OUTPUT);
  mcp.pinMode(7, OUTPUT);
}

void loop() {
  static byte led_pin = 0;

  mcp.write_GPIO(0);
  mcp.write_pin(led_pin, 1);

  if(++led_pin > 7)
    led_pin = 0;
    
//  static byte led_bit = 0b00100100;
//
//  mcp.write_GPIO(led_bit, 0);
//  led_bit = led_bit ^ 0b11111111;
    
  delay(250);
}
