#include <Arduino.h>

#include "config.h"
#include "Logger.h"
#include "TemperatureSensor.h"

TemperatureSensor temperatureSensor(Config::DhtPin, Config::DhtType);

unsigned long lastTemperatureReadMs = 0;

void setup() {
  Logger::begin(Config::SerialBaudRate);
  temperatureSensor.begin();

  Logger::info("ColdWatch started");
}

void loop() {
  const unsigned long now = millis();

  if (now - lastTemperatureReadMs >= Config::TemperatureReadIntervalMs) {
    lastTemperatureReadMs = now;

    float temperatureC = 0.0f;

    if (temperatureSensor.readTemperature(temperatureC)) {
      Logger::infoTemperature(temperatureC);
    } else {
      Logger::error("DHT22 read failed");
    }
  }
}
