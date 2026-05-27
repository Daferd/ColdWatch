#pragma once

#include <Arduino.h>

#include "TelemetryData.h"

class TelemetrySerializer {
public:
  static bool serializeTemperature(
    const TelemetryData& data,
    char* output,
    size_t outputSize);
};
