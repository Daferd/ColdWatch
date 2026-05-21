#pragma once

#include <Arduino.h>

class StatusLed {
public:
  StatusLed(uint8_t pin, uint32_t blinkIntervalMs);

  void begin();
  void update(bool wifiConnected);

private:
  uint8_t pin;
  uint32_t blinkIntervalMs;
  unsigned long lastToggleMs;
  bool ledOn;
};
