#pragma once

#include <Arduino.h>

class WiFiManager {
public:
  WiFiManager(
    const char* ssid,
    const char* password,
    const char* hostname,
    uint32_t reconnectIntervalMs);

  void begin();
  void update();
  bool isConnected() const;

private:
  void connect();

  const char* ssid;
  const char* password;
  const char* hostname;
  uint32_t reconnectIntervalMs;
  unsigned long lastReconnectAttemptMs;
  bool wasConnected;
  bool reconnectLogged;
};
