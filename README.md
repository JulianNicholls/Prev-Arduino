# Arduino

My Arduino Sketches
 
## test-128x64ZW

Simple test of driving a 128x64 LCD display using U8Glib. Set up for a ST7920
via SPI, but any of the other U8Glibsupported types would work, of course.

## Distance1

Sinmple read of an ultrasonic distance sensor, giving a time of flight in 
microseconds and distance in centimetres over the serial interface.

## MCP23017-first

Drive an I2C connected MCP23017. Set up for 8 LEDs attached to GPA0-7, a button
attached to GPB0 and another LED to GPB1, controlled by the button.

### MCP library

A simple library to drive the MCP23017, which allows for setting pin direction 
individually and in bulk, and reading and writing of the pins indivdually and in bulk.

## Keypad1

Simple read of a 4x4 keypad, using the Keypad library, outputting the pressed
keys over the serial link.

## ReadJoysticks

Read 2 analog joysticks and output the raw values over the serial link.

## Stepper1

Drive a 28BYJ-48 stepper motor via its interface board continually one 
revolution clockwise, then counter-clockwise. The time for a revolution and
hence the RPM are output on the serial link.

## LCD-first

## WS1812A

## stepper-driver

## Badge1

## Badge2

## Joysticks-128x64

## Joysticks-128x64-Graphic

## Keypad-MC17-4x4

## Keypad-MC17-4x4-Multi

## LCD-Joysticks

## LDR-Reader-128x64

## Stepper-MCP

## Sweep

## conway-life-128x64

## keypad-MC17-stepper

## keypad-stepper

## meArm

## Oscilloscope1

Simple oscilloscope which outputs to a 128x64 display. Initially set to 1V per
division vertically and 100ms per division horizontally. Updates twice a second.

Tested with the output from a 2 transistor oscillator at about 4Hz.

