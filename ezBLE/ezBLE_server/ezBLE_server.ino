/*
   ezBLE send and receive example

   Starts an ezBLE server and waits for a  client to connect.
   Once the client is connected the server will periodically send the CPU ID and the CPU temperature to the client
   while also simultaneously receiving any data sent by the client.

   Compatible with all Silicon Labs BLE hardware with the 'BLE (Silabs)' protocol stack variant selected.

   Author: Tamas Jozsi (Silicon Labs)
 */

#include <ezBLE.h>

bool pauseCPUTemp = false;
String receivedMsg;

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
  delay(5000);
  pauseCPUTemp = true;
  (void)bytes;
  while (ezBLE.available()) {
    receivedMsg.concat((char)ezBLE.read());
    Serial.println(receivedMsg);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ezBLE send and receive example");

  ezBLE.beginServer("faszgeci");
  ezBLE.onReceive(ezBLE_on_receive);
  while (!ezBLE.connected()) {
    delay(1000);
    checkReset();
    Serial.println("ezBLE not connected.");
  }
  Serial.println("ezBLE ready!");
}

void loop() {

  checkReset();

  if (ezBLE.connected() && pauseCPUTemp == false)
  {
    ezBLE.printf("Hello! CPU ID: %s\n", getDeviceUniqueIdStr().c_str());
    ezBLE.printf("CPU temp: %.02f C\n", getCPUTemp());
    Serial.println("Sending data");
  }
  else if (pauseCPUTemp == false)
  {
    Serial.println("Couldn't send data, ezBLE not connected!");
  }

  delay(5000);
}
