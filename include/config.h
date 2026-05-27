#pragma once

#include <Arduino.h>
#include <DHT.h>

namespace Config
{
  constexpr uint32_t SerialBaudRate = 115200;
  constexpr uint32_t TemperatureReadIntervalMs = 5000;

  constexpr uint8_t DhtPin = 4;
  constexpr uint8_t DhtType = DHT22;

  constexpr const char *DeviceId = "device_001";

  // constexpr const char *WifiSsid = "FLIA MOSQUERA";
  // constexpr const char *WifiPassword = "jhonm261";
  constexpr const char *WifiSsid = "Moto_AH";
  constexpr const char *WifiPassword = "12345678";
  constexpr const char *WifiHostname = "coldwatch";
  constexpr uint32_t WifiReconnectIntervalMs = 10000;
  constexpr uint32_t WifiReadyDelayMs = 2000;

  constexpr uint8_t StatusLedPin = 2;
  constexpr uint32_t StatusLedBlinkIntervalMs = 500;

  constexpr const char *MqttBrokerHost = "10.76.111.1";
  constexpr uint16_t MqttBrokerPort = 1883;
  constexpr const char *MqttClientId = "coldwatch_device_001";
  constexpr const char *MqttUsername = "";
  constexpr const char *MqttPassword = "";
  constexpr const char *MqttTemperatureTopic = "coldwatch/device_001/temperature";
  constexpr uint32_t MqttReconnectIntervalMs = 5000;
  constexpr size_t TelemetryPayloadBufferSize = 160;
}
