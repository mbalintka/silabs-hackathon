// thermostat_server.ino

#include <ezBLE.h>
#include "ArduinoLowPower.h"

String incomingBuffer;

struct ThermoheadStatus {
  String deviceId;
  float batteryVoltage;
  float localTemp;
  int valvePosition;
};

ThermoheadStatus parseThermoheadStatus(const String& msg) {
  ThermoheadStatus status;

  int start = 0;
  while (start < msg.length()) {
    int colon = msg.indexOf(':', start);
    int semicolon = msg.indexOf(';', start);

    if (colon == -1 || semicolon == -1 || semicolon < colon) break;

    String key = msg.substring(start, colon);
    String value = msg.substring(colon + 1, semicolon);

    if (key == "ID") {
      status.deviceId = value;
    } else if (key == "BAT") {
      status.batteryVoltage = value.toFloat();
    } else if (key == "TEMP") {
      status.localTemp = value.toFloat();
    } else if (key == "VALVE") {
      status.valvePosition = value.toInt();
    }

    start = semicolon + 1;
  }

  return status;
}

void ezBLE_on_receive(int bytes) {
  while (ezBLE.available()) {
    char c = (char)ezBLE.read();
    incomingBuffer += c;

    // Várunk teljes sort: \n jelre
    if (c == '\n') {
      Serial.println("Received full message:");
      Serial.println(incomingBuffer);

      ThermoheadStatus status = parseThermoheadStatus(incomingBuffer);

      Serial.println("Parsed Thermohead Status:");
      Serial.printf("ID: %s\n", status.deviceId.c_str());
      Serial.printf("Battery: %.2f V\n", status.batteryVoltage);
      Serial.printf("Temp: %.1f C\n", status.localTemp);
      Serial.printf("Valve: %d %%\n", status.valvePosition);

      // Válasz küldése
      float desiredTemp = 22.5;
      ezBLE.printf("SET_TEMP:%.1f\n", desiredTemp);
      Serial.println("Sent desired temperature to client");
      Serial.printf("\n\n");
      incomingBuffer = ""; // kiürítjük a buffert
    }
  }
}

void setup() {
  resetTFT();
  Serial.begin(115200);
  delay(1000);
  Serial.println("BLE Thermostat - Server");

  ezBLE.beginServer("BLE_THERMOSTAT");
  ezBLE.onReceive(ezBLE_on_receive);

  while (!ezBLE.connected()) {
    Serial.println("Waiting for client...");
    delay(1000);
  }

  Serial.println("BLE Server ready!");
}

void loop() {
  // nincs dolga, csak vár
  LowPower.idle(1000);
}

void resetTFT(){
  pinMode(PD2,OUTPUT);
  digitalWrite(PD2,LOW);
  delay(100);
  digitalWrite(PD2,HIGH);
  delay(100);
}
