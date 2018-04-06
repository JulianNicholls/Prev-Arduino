const five = require('johnny-five');
const Oled = require('oled-js');

const board = new five.Board();

const width = 128;
const height = 64;
const tileSize = 6;

class Vector {
  constructor(x = 0, y = 0) {
    this.x = x;
    this.y = y;
  }
}

function createTile(tile, screen) {
  const num = Math.random();
  const start = new Vector(tile.x, tile.y + tileSize - 1);
  const end = new Vector(tile.x + tileSize - 1, tile.y);

  if (num > 0.5) {
    start.y = tile.y;
    end.y = tile.y + tileSize - 1;
  }

  screen.drawLine(start.x, start.y, end.x, end.y, 1, false);
}

function createArt(screen) {
  for (let i = 0; i < width; i += tileSize) {
    for (let j = 0; j < height; j += tileSize) {
      const tile = new Vector(i, j);
      createTile(tile, screen);
    }
  }

  screen.update();
}

board.on('ready', function() {
  console.log('Connected to Arduino, ready.');

  var opts = {
    width,
    height,
    address: 0x3c
  };

  var oled = new Oled(board, five, opts);
  oled.clearDisplay();
  createArt(oled);
});
