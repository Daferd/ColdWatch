#pragma once

#include <Arduino.h>
#include <DHT.h>

namespace Config
{
  constexpr uint32_t SerialBaudRate = 115200;
  constexpr uint32_t TemperatureReadIntervalMs = 5000;

  constexpr uint8_t DhtPin = 4;
  constexpr uint8_t DhtType = DHT22;

  constexpr const char *WifiSsid = "Moto_AH";
  constexpr const char *WifiPassword = "12345678";
  constexpr const char *WifiHostname = "coldwatch";
  constexpr uint32_t WifiReconnectIntervalMs = 10000;

  constexpr uint8_t StatusLedPin = 2;
  constexpr uint32_t StatusLedBlinkIntervalMs = 500;
}
