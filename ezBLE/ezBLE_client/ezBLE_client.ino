/*
   ezBLE simple client example

   Starts an ezBLE client which will connect to a server.
   Once it's connected the device will receive any data sent by the server.

   Compatible with all Silicon Labs BLE hardware with the 'BLE (Silabs)' protocol stack variant selected.

   Author: Tamas Jozsi (Silicon Labs)
 */

#include <ezBLE.h>

void checkReset() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'r') {
      Serial.println("Resetting device...");
      delay(100);
      systemReset();
    }
  }
}

void ezBLE_on_receive(int bytes) {
  (void)bytes;
  while (ezBLE.available()) {
    Serial.print((char)ezBLE.read());
  }
}

void send_ezBLE(const char* msg, size_t len) {

  Serial.println("Sending message:");

  for (size_t i = 0; i < len; i++) {
    Serial.print(msg[i]);
    ezBLE.printf("%c", msg[i]);
  }

  Serial.println();
  Serial.println("Message sent!");
}

void handleInput() {
  checkReset();

  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    send_ezBLE(input.c_str(), input.length());
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  Serial.println("ezBLE simple client example");

  ezBLE.beginClient("faszgeci");
  ezBLE.onReceive(ezBLE_on_receive);
  while (!ezBLE.connected()) {
    delay(1000);
    checkReset();
    Serial.println("ezBLE not connected.");
  }
  Serial.println("ezBLE ready!");
}

void loop() {
  handleInput();
}
