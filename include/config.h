#pragma once

#include <Arduino.h>
#include <DHT.h>

namespace Config {
  constexpr uint32_t SerialBaudRate = 115200;
  constexpr uint32_t TemperatureReadIntervalMs = 5000;

  constexpr uint8_t DhtPin = 4;
  constexpr uint8_t DhtType = DHT22;
}
