void setup() {
  // put your setup code here, to run once:

  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);

  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:

  byte inA = digitalRead(6);
  byte inB = digitalRead(7);
  byte inC = digitalRead(8);
  byte inD = digitalRead(9);

  if (inA) {
    Serial.println(" ***");
    Serial.println("*   *");
    Serial.println("*****");
    Serial.println("*   *");
    Serial.println("*   *");
  }

  if (inB) {
    Serial.println("****");
    Serial.println("*   *");
    Serial.println("****");
    Serial.println("*   *");
    Serial.println("****");
  }

  if (inC) {
    Serial.println("****");
    Serial.println("*   *");
    Serial.println("*");
    Serial.println("*   *");
    Serial.println("****");
  }

  if (inD) {
    Serial.println("***");
    Serial.println("*  *");
    Serial.println("*   *");
    Serial.println("*  *");
    Serial.println("***");
  }

  Serial.println("");
//  Serial.print("A: ");
//  Serial.print(inA);
//  Serial.print(", B: ");
//  Serial.print(inB);
//  Serial.print(", C: ");
//  Serial.print(inC);
//  Serial.print(", D: ");
//  Serial.println(inD);
  delay(500);
}
