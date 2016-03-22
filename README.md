# Arduino

My Arduino Sketches.

If anyone is interested enough to read a blog article about how I did any 
of these, please contact me and I'll write one.
 
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

Very minimal check of a connected LCD display.

## WS1812B

Use the Adafruit NeoPixel library to drive my 8-LCD pixel bar with a rainbow pattern.
Updated to 24 LCDs.

## stepper-driver

## Badge1

First version of the sketch for my proximity badge. This one just displays the
distance from an ultrasonic distance sensor in different colours of the NeoPixel strip.

## Badge2

Second version of the proximity badge, which has the added 4x20 LCD. The LCD is cleared
when there is no one around. Now that I know how to turn the backlight on and off 
under program control, I will do that next time.

## Joysticks-128x64

## Joysticks-128x64-Graphic

## Keypad-MC17-4x4

Read a 4x4 keypad connected to a MCP23017 i2c interface.

## Keypad-MC17-4x4-Multi

## LCD-Joysticks

## LDR-Reader-128x64

## Stepper-MCP

## Sweep

Sweep a servo from one end to the other, using the Servo library.

## conway-life-128x64

[John Conway's game of life](http://www.conwaylife.com/) implemented on the
128x64 LCD. Updated to use the newly discovered 
[Copperhead](http://conwaylife.com/wiki/Copperhead)
spaceship which goes across the screen in a little over 500 generations.

## keypad-MC17-stepper

## keypad-stepper

## meArm

## Oscilloscope1

Simple oscilloscope which outputs to a 128x64 display. Initially set to 1V per
division vertically and 100ms per division horizontally. Updates twice a second.

Tested with the output from a 2 transistor oscillator at about 4Hz.

