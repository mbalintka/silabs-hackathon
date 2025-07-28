#include <SHTC3.h>

SHTC3 sensor(Wire1);

void setup() {
  Serial.begin(115200);
  if (!sensor.begin()) {
    Serial.println("Failed to initialize SHTC3 sensor");
    while (1);
  }
}

void loop() {
  float temperature, humidity;
  if (sensor.read(temperature, humidity)) {
    Serial.print("Temperature: ");
    Serial.print(temperature, 2);
    Serial.print(" C, Humidity: ");
    Serial.print(humidity, 2);
    Serial.println(" %");
  } else {
    Serial.println("Failed to read from sensor");
  }
  delay(1000);
}
