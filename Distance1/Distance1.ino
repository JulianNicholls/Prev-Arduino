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

  // Measure the Echo pin high time

  int     elapsed   = pulseIn(ECHO_PIN, HIGH);

  double  fdistance = elapsed * 0.017163; // Manual says elapsed / 58
  int     idist     = int(fdistance + 0.5);
  int     idistance = (elapsed + 29) / 58;
  
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

