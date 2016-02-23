#include <Arduino.h>
#include "mcp.h"

void MCP23017::write_IODIR(uint8_t value, uint8_t ab) {
  uint8_t address = ab ? IODIRB : IODIRA;

  writeRegister(address, value);
}

void MCP23017::write_GPIO(uint8_t value, uint8_t ab) {
  uint8_t address = ab ? GPIOB : GPIOA;

  writeRegister(address, value);
}

void MCP23017::pinMode(uint8_t pin, uint8_t mode, bool pullup) {
  uint8_t ab       = pin >> 3;
  uint8_t pin_bit  = pin & 7;

  updateRegisterBit(pin_bit, mode == INPUT, ab ? IODIRB : IODIRA);

  if(mode == INPUT && pullup) {
    updateRegisterBit(pin_bit, pullup, ab ? GPPUB : GPPUA);
  }
}

uint8_t MCP23017::read_pin(uint8_t pin) {
  uint8_t ab       = pin >> 3;
  uint8_t pin_bit  = pin & 7;
  uint8_t value    = readRegister(ab ? GPIOB : GPIOA);

  return (value & (1 << pin_bit)) >> pin_bit;
}

void MCP23017::write_pin(uint8_t pin, bool set) {
  uint8_t ab       = pin >> 3;
  uint8_t pin_bit  = pin & 7;

  updateRegisterBit(pin_bit, set, ab ? GPIOB : GPIOA);
}

void MCP23017::updateRegisterBit(uint8_t bit, bool set, uint8_t address) {
  uint8_t cur = readRegister(address);

  bit = 1 << bit;

  if(set)
    cur |= bit;
  else
    cur &= ~bit;

  writeRegister(address, cur);
}

uint8_t MCP23017::readRegister(uint8_t address) {
  Wire.beginTransmission(m_address);
  Wire.write(address);
  Wire.endTransmission();

  Wire.requestFrom((uint8_t) m_address, (uint8_t) 1);
  return Wire.read();
}

void MCP23017::writeRegister(uint8_t address, uint8_t value) {
  Wire.beginTransmission(m_address);
  Wire.write(address);
  Wire.write(value);
  Wire.endTransmission();
}


