// thermofej_client.ino

#include <ezBLE.h>
#include "ArduinoLowPower.h"

#define DEVICE_ID "TERMFEJ_001"
#define SLEEP_DURATION_MS 5000

struct ThermoheadStatus {
  String deviceId;
  float batteryVoltage;
  float localTemp;
  int valvePosition; // 0-100%
};

float readBatteryVoltage() {
  // Dummy ADC helyett fix érték
  return 3.72;
}

float readLocalTemperature() {
  // Dummy temp
  return 21.5;
}

int readValvePosition() {
  // Dummy állapot
  return 70;
}

void updateThermoheadStatus(ThermoheadStatus& status) {
  status.batteryVoltage = readBatteryVoltage();
  status.localTemp = readLocalTemperature();
  status.valvePosition = readValvePosition();
}

void sendThermoheadStatus(const ThermoheadStatus& status) {
  String msg;
  msg += "ID:" + status.deviceId + ";";
  msg += "BAT:" + String(status.batteryVoltage, 2) + ";";
  msg += "TEMP:" + String(status.localTemp, 1) + ";";
  msg += "VALVE:" + String(status.valvePosition) + ";";

  Serial.println("Sending structured data:");
  Serial.println(msg);
  ezBLE.println(msg);
}

void ezBLE_on_receive(int bytes) {
  String response;
  while (ezBLE.available()) {
    response += (char)ezBLE.read();
  }

  Serial.print("Received from server: ");
  Serial.println(response);
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("BLE Thermofej - Client");

  ezBLE.beginClient("BLE_THERMOSTAT");
  ezBLE.onReceive(ezBLE_on_receive);

  Serial.println("Connecting to server...");
  unsigned long start = millis();
  while (!ezBLE.connected() && millis() - start < 10000) {
    Serial.println("Waiting for connection...");
    delay(500);
  }

  ThermoheadStatus status = {
    .deviceId = DEVICE_ID
  };

  if (ezBLE.connected()) {
    updateThermoheadStatus(status);
    sendThermoheadStatus(status);
    delay(1000); // válaszra várunk
  } else {
    Serial.println("Failed to connect to server.");
  }

  Serial.printf("Going to sleep for %d ms\n", SLEEP_DURATION_MS);
  LowPower.deepSleep(SLEEP_DURATION_MS);
}

void loop() {
  // Soha nem fut le, mert deepSleep után újraindul
}
