#ifndef SHTC3_H
#define SHTC3_H

#include <Wire.h>

class SHTC3 {
public:
  SHTC3(TwoWire &wirePort = Wire);

  bool begin();
  bool read(float &temperature, float &humidity);

private:
  TwoWire *wire;
  static const uint8_t SHTC3_ADDR = 0x70;
  static const uint16_t CMD_WAKEUP = 0x3517;
  static const uint16_t CMD_SLEEP = 0xB098;
  static const uint16_t CMD_MEASURE_TF = 0x7866;

  bool sendCommand(uint16_t cmd);
  float rawToCelsius(uint16_t raw);
  float rawToHumidity(uint16_t raw);
};

#endif
