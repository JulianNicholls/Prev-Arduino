void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  int reading = analogRead(A0);

  Serial.print("Reading: ");
  Serial.print(reading);
  Serial.print(", ");
  Serial.println((reading * 5) / 1023.0);

  delay(500);
}
