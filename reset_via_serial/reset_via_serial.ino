void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Serial available");

}

void loop() {
    if (Serial.available()) {
    char c = Serial.read();
    if (c == 'r') {
      Serial.println("Resetting device...");
      delay(100);
      systemReset();
    }
  }

  Serial.println("Buzi vagyok");

  delay(50);

}
