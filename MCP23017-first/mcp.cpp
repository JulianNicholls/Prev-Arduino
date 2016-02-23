#include "mcp.h"

void MCP23017::write_IODIR(byte value, byte ab) {
  byte address = ab ? IODIRB : IODIRA;

  writeRegister(address, value);
}

void MCP23017::write_GPIO(byte value, byte ab) {
  byte address = ab ? GPIOB : GPIOA;

  writeRegister(address, value);
}

void MCP23017::pinMode(byte pin, byte mode, boolean pullup) {
  byte ab       = pin >> 3;
  byte pin_bit  = pin & 7;

  updateRegisterBit(pin_bit, mode == INPUT, ab ? IODIRB : IODIRA);

  if(mode == INPUT && pullup) {
    updateRegisterBit(pin_bit, pullup, ab ? GPPUB : GPPUA);
  }
}

byte MCP23017::read_pin(byte pin) {
  byte ab       = pin >> 3;
  byte pin_bit  = pin & 7;
  byte value    = readRegister(ab ? GPIOB : GPIOA);

  return (value & (1 << pin_bit)) >> pin_bit;
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


