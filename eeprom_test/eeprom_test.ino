/*
 * EEPROM tesztelese
 *
 */

#include <EEPROM.h>

/** 0 cimtol kezdunk **/
int addr = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  randomSeed(analogRead(A0));
}

void loop() {
  /***
    gecike csak 0 es 255 kozotti ertekeket tud
  ***/
  int writeValue = random(0, 256);

  /***
    permanensen ottmarad!!! nem bantsa senki ha kikapcsoljak a manokat
  ***/
  EEPROM.write(addr, writeValue);

  Serial.println("Write:");
  Serial.print("Address: ");
  Serial.print(addr);
  Serial.print("   |   writeValue: ");
  Serial.print(writeValue, DEC);
  Serial.println();

  delay(100);
  int readValue = EEPROM.read(addr);

  Serial.println("Readback:");
  Serial.print("Address: ");
  Serial.print(addr);
  Serial.print("   |   readValue: ");
  Serial.print(readValue, DEC);
  Serial.println();

  /***
    increment address
  ***/
  addr = addr + 1;
  if (addr == EEPROM.length()) {
    addr = 0;
  }

  delay(1000);
}
