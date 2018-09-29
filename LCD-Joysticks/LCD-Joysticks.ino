#include <Servo.h>
#include <LiquidCrystal.h>

const int lr1 = A0;   // Joystick 1 Left-Right
const int ud1 = A1;   // Joystick 1 Up-Down
const int lr2 = A2;   // Joystick 1 Left-Right
const int ud2 = A3;   // Joystick 1 Up-Down

const int centre1 = 6; // Pin 6 centres

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int pots[] = { A0, A1, A2, A3 };
int poss[] = { 90, 90, 90, 90 };

Servo base, shoulder, elbow, grip;

void setup() {
  // put your setup code here, to run once:

  lcd.begin(20, 4);

  base.attach(10);
  elbow.attach(9);
  shoulder.attach(8);
  grip.attach(7);
}

void loop() {
  for(int i = 0; i < 4; ++i)
  {
    int reading = analogRead(pots[i]);
    int value   = map(reading, 0, 1023, -5, 5);

    lcd.setCursor(0, i);
    lcd.print(i);
    
    lcd.setCursor(2, i);
    lcd.print(reading);
    lcd.write(' ');

    lcd.setCursor(8, i);
    lcd.print(value);
    lcd.write(' ');

    lcd.setCursor(14, i);
    lcd.print(poss[i]);
    lcd.write(' ');

    if(value < -2)
    {
      poss[i] -= (value == -5) ? 2 : 1;
      if(poss[i] < 5)
        poss[i] = 5;
    }

    if(value > 2)
    {
      poss[i] += (value == 5) ? 2 : 1;
      
      if(poss[i] > 175)
        poss[i] = 175;
    }

//    if(digitalRead(centre1) == 1)
//      poss[i] = 90;
  }

  base.write(poss[0]);
  grip.write(poss[1]);
  elbow.write(poss[2]) 
  ;
  shoulder.write(poss[3]);
  delay(10);
}

