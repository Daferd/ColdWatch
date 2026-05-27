#pragma once

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include "WiFiManager.h"

class MQTTManager {
public:
  MQTTManager(
    const char* brokerHost,
    uint16_t brokerPort,
    const char* deviceId,
    const char* clientId,
    const char* username,
    const char* password,
    const char* temperatureTopic,
    uint32_t reconnectIntervalMs,
    WiFiManager& wifiManager);

  void begin();
  void update();
  bool isConnected();
  bool publishTemperature(float temperatureC);

private:
  void connect();
  bool canOpenTcpConnection();
  bool hasCredentials() const;
  void logConnectFailure(int state);

  WiFiClient networkClient;
  PubSubClient mqttClient;
  WiFiManager& wifiManager;
  const char* brokerHost;
  uint16_t brokerPort;
  const char* deviceId;
  const char* clientId;
  const char* username;
  const char* password;
  const char* temperatureTopic;
  uint32_t reconnectIntervalMs;
  unsigned long lastReconnectAttemptMs;
  bool wasConnected;
  bool reconnectLogged;
  bool waitingForWifiLogged;
};
