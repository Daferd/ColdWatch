#pragma once

#include <Arduino.h>
#include <DHT.h>

class TemperatureSensor {
public:
  TemperatureSensor(uint8_t pin, uint8_t type, uint32_t minReadIntervalMs = 2000);

  void begin();
  bool readTemperature(float& temperatureC);

  bool hasValidTemperature() const;
  float lastTemperatureC() const;
  unsigned long lastValidReadMs() const;

private:
  DHT dht;
  uint32_t minReadIntervalMs;
  unsigned long lastReadAttemptMs;
  unsigned long lastValidReadTimeMs;
  float lastTemperatureValueC;
  bool hasLastValidTemperature;
  bool hasReadAttempt;
};
