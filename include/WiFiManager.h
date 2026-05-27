#pragma once

#include <Arduino.h>

class WiFiManager {
public:
  WiFiManager(
    const char* ssid,
    const char* password,
    const char* hostname,
    uint32_t reconnectIntervalMs,
    uint32_t readyDelayMs);

  void begin();
  void update();
  bool isConnected() const;
  bool isReady() const;

private:
  void connect();

  const char* ssid;
  const char* password;
  const char* hostname;
  uint32_t reconnectIntervalMs;
  uint32_t readyDelayMs;
  unsigned long lastReconnectAttemptMs;
  unsigned long connectedSinceMs;
  bool wasConnected;
  bool reconnectLogged;
};
