const five = require('johnny-five');
const board = new five.Board();

board.on('ready', function() {
  // Create an Led on pin 13
  const led = new five.Led(13);
  // Blink every third of a second
  led.blink(300);
});
