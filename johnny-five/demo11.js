const five = require('johnny-five');
const board = new five.Board();
let led;

// Do we want the sequence to loop?
const loop = true;

// Create a simple demo sequece that calls various
// five.Led methods with specified arguments and
// let it run for the given duration (defaults to 3 seconds).
const demoSequence = [
  {
    method: 'pulse',
    args: [500],
    duration: 5000
  },
  {
    method: 'strobe',
    args: [40],
    duration: 3000
  },
  {
    method: 'fadeIn',
    args: [
      2000,
      function() {
        console.log('fadeIn complete!');
      }
    ],
    duration: 2500
  },
  {
    method: 'fadeOut',
    args: [
      3000,
      function() {
        console.log('fadeOut complete!');
      }
    ],
    duration: 5500
  },
  {
    method: 'brightness',
    args: [20],
    duration: 2000
  },
  {
    method: 'off'
  }
];

// Execute a method in the demo sequence
function execute(step) {
  // Grab everything we need for this step
  const { method, args, duration } = demoSequence[step];

  // Make the actual call to the LED
  five.Led.prototype[method].apply(led, args);

  // If we're at the end, start over (loop==true) or exit
  if (++step === demoSequence.length) {
    if (loop) {
      step = 0;
    } else {
      // We're done!
      process.exit(0);
    }
  }

  // Recursively call the next step after specified duration
  board.wait(duration, function() {
    execute(step);
  });
}

board.on('ready', function() {
  // Defaults to pin 11 (must be PWM)
  led = new five.Led(process.argv[2] || 11);

  // Kick off the first step
  execute(0);
});
