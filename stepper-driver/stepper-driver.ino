#include <U8glib.h>
#include <Keypad_MC17.h>

const byte ROWS = 4;
const byte COLS = 4;

const char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
};

const int  MCP_addr   = 0x20;         // MCP23017 base address

const int  max_digits = 4;

const int  motor_delay     = 1250;   // us delay between steps, 1250 is absolute minimum
const long counts_per_rev  = 512;    // Number of complete steps per full revolution
const int  motor_bits[]    = {0b1100, 0b0110, 0b0011, 0b1001};

byte rowPins[ROWS]   = { 0, 1, 2, 3 };  // GPA0-3 for rows
byte colPins[COLS]   = { 4, 5, 6, 7 };  // GPA4-7 for columns

Keypad_MC17             keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS, MCP_addr);
U8GLIB_ST7920_128X64_1X u8g(10);    // Hardware SPI, 10 = CS/CE, 11 = MOSI, 13 = SCK

int   steps       = 0;
int   motor_steps = 0;
byte  digits      = 0;
char  key         = ' ';;
bool  clockwise   = true;
bool  degrees     = true;

void setup() {
  keypad.begin();

  keypad.iodir_write(keypad.iodir_read() & 0x00ff); // Set Port B as outputs
  
  u8g.setFont(u8g_font_helvB10);
  draw_loop();
}

void loop() {
  key = keypad.getKey();

  if (key != NO_KEY) {
    if (key >= '0' && key <= '9') {
      int value = key - '0';

      if (digits < max_digits) {
        steps = (steps * 10) + value;
        ++digits;
      }
    }
    else {
      switch (key) {
        case '*':   // Backspace
          if (digits > 0) {
            --digits;
            steps /= 10;
          }
          break;

        case '#':   // Enter
          if (steps != 0) {
            rotate(steps);
            steps = 0;
            digits = 0;
          }
          break;

        case 'C':   // Swap 'C'lockwise / Anti
          clockwise = !clockwise;
          break;

        case 'D':   // Swap 'D'egrees / Steps
          degrees = !degrees;
          break;
      }
    }

    draw_loop();
  }
}

//////////////////////////////////////////////////////////////////////////////
// Rotate the stepper

void rotate(int steps) {
  motor_steps = degrees ? (counts_per_rev * steps / 360) : steps;

  for(; motor_steps > 0; --motor_steps) {
    clockwise ? turn_clockwise() : turn_anticlockwise();

//    if(motor_steps % 20 == 0)
//      draw_loop();
  }
}

//////////////////////////////////////////////////////////////////////////////
// Set pins to ULN2003 high in sequence from 1 to 4
// delay "motor_delay" between each pin setting (to determine speed)

void turn_anticlockwise()
{
  for (int i = 0; i < 4; ++i)
  {
    setOutput(i);
    delayMicroseconds(motor_delay);
  }
}

void turn_clockwise()
{
  for (int i = 3; i >= 0; --i)
  {
    setOutput(i);
    delayMicroseconds(motor_delay);
  }
}

// Conservatively update Port B

void setOutput(int out)
{
  int prev = keypad.pinState_set() & 0x00ff;
  int newval = prev | (motor_bits[out] << 8);
  
  keypad.port_write(newval);
}

////////////////////////////////////////////////////////////////////////////

void draw_loop() {
  u8g.firstPage();
  
  do {
    draw();
  } 
  while(u8g.nextPage());
}

void draw() {
  u8g.setFont(u8g_font_helvB10);
  u8g.setPrintPos(0, 14);
  u8g.print(steps);

  u8g.setFont(u8g_font_6x10);
  u8g.drawStr(40, 8, degrees ? "Degrees" : "Steps");
  u8g.drawStr(40, 18, clockwise ? "Clockwise" : "Anti-Clockwise");

  if(motor_steps > 0) {
    u8g.drawStr(0, 40, "Steps To Go:");
    u8g.setPrintPos(80, 40);
    u8g.print(motor_steps);
  }
}

