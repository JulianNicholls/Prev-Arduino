const serial    = require('serialport');

const plotly    = require('plotly')('juliannicholls', 'fxu2war17w')
const ittoken   = 'vvz7t8jo7h';
const ottoken   = 'gejsual6f8';
const ihtoken   = 'nuqwjw035q';

const portname  = '/dev/cu.usbmodem141411';
const settings  = {
  baudRate:     9600,
  dataBits:     8,
  parity:       'none',
  stopBits:     1,
  flowControl:  'false',
  parser:       serial.parsers.readline("\r\n")
};
const port      = new serial(portname, settings);

function getDateString() {
  const time    = new Date().getTime();

  // I'm in the GMT/BST timezone, so I'm not making any adjustment
  return new Date(time).toISOString().replace(/T/, ' ').replace(/Z/, '');
}

const initData = [
  { x: [], y: [], stream: {
      token: ittoken,
      maxpoints: 500
    }
  },
  { x: [], y: [], stream: {
      token: ottoken,
      maxpoints: 500
    }
  },
  { x: [], y: [], stream: {
      token: ihtoken,
      maxpoints: 500
    }
  }
];
const initLayout = {
  fileopt: 'extend',
  filename: 'dual-temperature',
  title: 'Temperature and Humidity',
  yaxis: { title: 'Inside Temperature' },
  yaxis2: { title: 'Outside Temperature' },
  yaxis3: { title: 'Humidity' }
};

plotly.plot(initData, initLayout, function(err, msg) {
  if (err) return console.log(err);

  console.log('Setup:', msg);

  const itStream = plotly.stream(ittoken, function(err, res) {
    console.log('itStream:', err, res);
  });

  const otStream = plotly.stream(ottoken, function(err, res) {
    console.log('otStream:', err, res);
  });

  const ihStream = plotly.stream(ihtoken, function(err, res) {
    console.log('ihStream:', err, res);
  });

  port.on('data', function(input) {
    console.log('data:', input);

    const values = input.split(' ').map(str => parseFloat(str));

//    console.log('values:', values);

    const stamp = getDateString();
    const itObj = JSON.stringify({ x: stamp, y: values[0] });
    const otObj = JSON.stringify({ x: stamp, y: values[1] });
    const ihObj = JSON.stringify({ x: stamp, y: values[2] });

//    console.log('pl-so:', streamObj);

    itStream.write(itObj + '\n');
    otStream.write(otObj + '\n');
    ihStream.write(ihObj + '\n');
  });
});
