const five = require('johnny-five');
const Oled = require('oled-js');

const board = new five.Board();

board.on('ready', function() {
  console.log('Connected to Arduino, ready.');

  var opts = {
    width: 128,
    height: 64,
    address: 0x3c
  };

  var oled = new Oled(board, five, opts);
  oled.clearDisplay();
  oled.update();
  oled.drawPixel([[128, 1, 1], [128, 32, 1], [128, 16, 1], [64, 16, 1]]);
});
