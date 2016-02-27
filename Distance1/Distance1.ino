const byte TRIG_PIN     = 13;
const byte ECHO_PIN     = 12;

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

  // Wait for the Echo pin to go high

  while(digitalRead(ECHO_PIN) == LOW)
    ;

  int start = micros();

  // Flight time is the length of time that the Echo pin is high

  while(digitalRead(ECHO_PIN) == HIGH)
    ;

  int     elapsed   = micros() - start;
  double  distance  = elapsed * 0.017163;
  int     idist     = int(distance + 0.5);
  
  Serial.print("Elapsed us: ");
  Serial.print(elapsed);
  Serial.print(", Distance: ");
  Serial.print(idist);
  Serial.print(" - ");
  Serial.println(distance);

  delay(500);
}

