#include "SHTC3.h"

SHTC3::SHTC3(TwoWire &wirePort) {
  wire = &wirePort;
}

bool SHTC3::begin() {
  wire->begin();
  return sendCommand(CMD_WAKEUP);
}

bool SHTC3::sendCommand(uint16_t cmd) {
  wire->beginTransmission(SHTC3_ADDR);
  wire->write(cmd >> 8);
  wire->write(cmd & 0xFF);
  return wire->endTransmission() == 0;
}

float SHTC3::rawToCelsius(uint16_t raw) {
  return -45.0 + (175.0 * raw / 65535.0);
}

float SHTC3::rawToHumidity(uint16_t raw) {
  return 100.0 * raw / 65535.0;
}

bool SHTC3::read(float &temperature, float &humidity) {
  if (!sendCommand(CMD_WAKEUP)) return false;
  delay(1);

  if (!sendCommand(CMD_MEASURE_TF)) return false;
  delay(15);

  wire->requestFrom(SHTC3_ADDR, 6);
  if (wire->available() != 6) {
    sendCommand(CMD_SLEEP);
    delay(1);
    return false;
  }

  uint8_t t_msb = wire->read();
  uint8_t t_lsb = wire->read();
  wire->read(); // CRC
  uint8_t h_msb = wire->read();
  uint8_t h_lsb = wire->read();
  wire->read(); // CRC

  uint16_t t_raw = (t_msb << 8) | t_lsb;
  uint16_t h_raw = (h_msb << 8) | h_lsb;

  temperature = rawToCelsius(t_raw);
  humidity = rawToHumidity(h_raw);

  sendCommand(CMD_SLEEP);
  return true;
}
