const int DATA  = 6;
const int CLK   = 7;

byte count = 0;

void setup() {
  pinMode(DATA, OUTPUT);
  pinMode(CLK, OUTPUT);
}

void loop() {
  shiftOut(DATA, CLK, MSBFIRST, count++);

  delay(30);
}
