#include "TemperatureSensor.h"

#include <Arduino.h>

TemperatureSensor::TemperatureSensor(uint8_t pin, uint8_t type, uint32_t minReadIntervalMs)
  : dht(pin, type),
    minReadIntervalMs(minReadIntervalMs),
    lastReadAttemptMs(0),
    lastValidReadTimeMs(0),
    lastTemperatureValueC(0.0f),
    hasLastValidTemperature(false),
    hasReadAttempt(false) {
}

void TemperatureSensor::begin() {
  dht.begin();
}

bool TemperatureSensor::readTemperature(float& temperatureC) {
  const unsigned long now = millis();

  if (hasReadAttempt && now - lastReadAttemptMs < minReadIntervalMs) {
    if (hasLastValidTemperature) {
      temperatureC = lastTemperatureValueC;
      return true;
    }

    return false;
  }

  lastReadAttemptMs = now;
  hasReadAttempt = true;

  const float value = dht.readTemperature();

  if (isnan(value)) {
    return false;
  }

  lastTemperatureValueC = value;
  lastValidReadTimeMs = now;
  hasLastValidTemperature = true;

  temperatureC = value;
  return true;
}

bool TemperatureSensor::hasValidTemperature() const {
  return hasLastValidTemperature;
}

float TemperatureSensor::lastTemperatureC() const {
  return lastTemperatureValueC;
}

unsigned long TemperatureSensor::lastValidReadMs() const {
  return lastValidReadTimeMs;
}
