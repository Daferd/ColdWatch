#include "TelemetrySerializer.h"

#include <stdio.h>

bool TelemetrySerializer::serializeTemperature(
  const TelemetryData& data,
  char* output,
  size_t outputSize) {
  char temperature[12];
  dtostrf(data.temperatureC, 0, 1, temperature);

  const int written = snprintf(
    output,
    outputSize,
    "{\"device_id\":\"%s\",\"temperature_c\":%s,\"uptime_ms\":%lu,\"wifi_rssi\":%d}",
    data.deviceId,
    temperature,
    data.uptimeMs,
    data.wifiRssi);

  return written > 0 && static_cast<size_t>(written) < outputSize;
}
