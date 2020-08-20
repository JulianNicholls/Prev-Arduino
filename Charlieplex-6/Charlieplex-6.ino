const byte A = 2;
const byte B = 3;
const byte C = 4;

const byte leds[][3] = {
  {A, B, C}, {A, C, B},
  {B, A, C}, {C, A, B},
  {B, C, A}, {C, B, A}
};

const int num_leds = sizeof(leds) / sizeof(leds[0]);

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  static int msecs = 500;
  
  // put your main code here, to run repeatedly:

  for(int i = 0; i < num_leds; ++i) {
    light(i);
    delay(msecs);
  }

  if(msecs >= 3) {
    msecs = msecs * 9 / 10;
  }
}

void light(int led) {
  pinMode(leds[led][0], OUTPUT);
  pinMode(leds[led][1], OUTPUT);
  pinMode(leds[led][2], INPUT);

  digitalWrite(leds[led][0], HIGH);
  digitalWrite(leds[led][1], LOW);
}

