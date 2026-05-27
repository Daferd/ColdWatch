#pragma once

#include <Arduino.h>

struct TelemetryData {
  const char* deviceId;
  float temperatureC;
  unsigned long uptimeMs;
  int wifiRssi;
};
