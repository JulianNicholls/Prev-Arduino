void setup() {
  // Set up the two digital inputs
  pinMode(32, INPUT);   // Bottom input
  pinMode(33, INPUT);   // Top input

  Serial.begin(115200);
}

void loop() {
  Serial.printf("Pin 32: %d, Pin 33: %d, analogue pin 34: %d\n", digitalRead(32), digitalRead(33), analogRead(34));
  delay(500);
}
