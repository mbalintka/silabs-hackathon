#include <Adafruit_Si7021.h>



Adafruit_Si7021 temp_humidity_sensor;

void measureSensors()
{
  
  float humidity = temp_humidity_sensor.readHumidity();
  float temp = temp_humidity_sensor.readTemperature();

  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" C | ");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("% | ");
}

void setup()
{
  Serial.begin(115200);
  // Enable power for the Si7210, Si7021 and VEML6035
  pinMode(PC9, OUTPUT);
  digitalWrite(PC9, HIGH);
  
  delay(200);

  
  // Init the Si7021 temperature and humidity sensor
  if (!temp_humidity_sensor.begin()) {
    Serial.println("Temperature and humidity sensor initialization failed");
  } else {
    Serial.println("Temperature and humidity sensor initialization successful");
  }
}

void loop()
{
  const uint32_t MEASUREMENT_INTERVAL_MS = 500u;
  static uint32_t previousMillis = 0u;
  uint32_t currentMillis = millis();
  if (currentMillis - previousMillis >= MEASUREMENT_INTERVAL_MS) {
    previousMillis = currentMillis;
    measureSensors();
  }

}
