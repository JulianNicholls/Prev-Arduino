const byte TRIG_PIN     = 7;
const byte ECHO_PIN     = 8;

void setup() {
  // put your setup code here, to run once:

  pinMode(TRIG_PIN, OUTPUT);  // Trigger
  pinMode(ECHO_PIN, INPUT);   // Echo

  Serial.begin(9600);
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);

  // Send a 10uS pulse to the trigger pin

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

//  // Wait for the Echo pin to go high
//
//  while(digitalRead(ECHO_PIN) == LOW)
//    ;
//
//  int start = micros();
//
//  // Flight time is the length of time that the Echo pin is high
//
//  while(digitalRead(ECHO_PIN) == HIGH)
//    ;
//
//  long    elapsed   = micros() - start;

  long    elapsed   = pulseIn(ECHO_PIN, HIGH);
  double  fdistance = elapsed * 0.017163; // Manual says elapsed / 58
  int     idist     = int(fdistance + 0.5);
  long    idistance = (elapsed + 29) / 58;
  
  Serial.print("Elapsed us: ");
  Serial.print(elapsed);
  Serial.print(", Distance: f: ");
  Serial.print(fdistance);
  Serial.print(" r: ");
  Serial.print(idist);
  Serial.print(", i: ");
  Serial.println(idistance);

  delay(200);
}

