#include <Wire.h>

const uint8_t MCP_BASE   = 0x20;  // MCP23017

// IOCON.BANK = 0 addressing

const uint8_t IODIRA     = 0x00;
const uint8_t IODIRB     = 0x01;
const uint8_t GPPUA      = 0x0c;
const uint8_t GPPUB      = 0x0d;
const uint8_t GPIOA      = 0x12;
const uint8_t GPIOB      = 0x13;

class MCP23017 {
public:

  void begin(uint8_t address = 0) {
    m_address = MCP_BASE + (address & 0x07);

    Wire.begin();
  }

  void pinMode(uint8_t pin, uint8_t mode, bool pullup = false);
  uint8_t read_pin(uint8_t pin);
  void write_pin(uint8_t pin, bool set);

  void write_IODIR(uint8_t value, uint8_t ab = 0);
  void write_GPIO(uint8_t value, uint8_t ab = 0);
  uint8_t read_GPIO(uint8_t ab = 0);

private:

  uint8_t m_address;

  uint8_t readRegister(uint8_t address);
  void writeRegister(uint8_t address, uint8_t value);
  void updateRegisterBit(uint8_t bit, bool set, uint8_t address);
};

