const serial    = require('serialport');
const portname  = '/dev/cu.usbmodem141411';
const port      = new serial(portname, {
  baudRate:     9600,
  dataBits:     8,
  parity:       'none',
  stopBits:     1,
  flowControl:  'false',
  parser:       serial.parsers.readline("\r\n")
});

port.on('data', function(input) {
  console.log(input);
});
