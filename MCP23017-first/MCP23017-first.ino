#include "mcp.h"

//////////////////////////////////////////////////////////////////////////

MCP23017 mcp;

void setup() {
  mcp.begin();

  mcp.write_IODIR(0);           // Set all of GPIOA as outputs

  mcp.pinMode(8, INPUT, true);  // Set pull-up
  mcp.pinMode(9, OUTPUT);
}

void loop() {
  static uint8_t led_pin = 0;

  mcp.write_GPIO(0);
  mcp.write_pin(led_pin, 1);

  if(++led_pin > 7)
    led_pin = 0;

  mcp.write_pin(9, !mcp.read_pin(8));

  delay(150);
}

