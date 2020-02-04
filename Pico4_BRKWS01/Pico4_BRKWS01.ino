/* Passthrough from USB Serial to UART 1, connecting to a BRKSW01 on pins 9 & 10  */

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1);
}

void loop() {
  if (Serial.available()) {         // If anything comes in from USB,
    Serial1.write(Serial.read());   // Read it and send it out Serial1
  }

  if (Serial1.available()) {        // If anything comes in Serial1
    Serial.write(Serial1.read());   // Read it and send it out to USB
  }
}
