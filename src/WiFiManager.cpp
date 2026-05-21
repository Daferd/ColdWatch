#include "WiFiManager.h"

#include <WiFi.h>

#include "Logger.h"

WiFiManager::WiFiManager(
  const char* ssid,
  const char* password,
  const char* hostname,
  uint32_t reconnectIntervalMs)
  : ssid(ssid),
    password(password),
    hostname(hostname),
    reconnectIntervalMs(reconnectIntervalMs),
    lastReconnectAttemptMs(0),
    wasConnected(false),
    reconnectLogged(false) {
}

void WiFiManager::begin() {
  WiFi.mode(WIFI_STA);
  WiFi.persistent(false);
  WiFi.setAutoReconnect(true);
  WiFi.setHostname(hostname);

  Logger::info("WiFi connecting");
  connect();
}

void WiFiManager::update() {
  const bool connected = isConnected();

  if (connected) {
    if (!wasConnected) {
      Logger::infoWifiConnected(WiFi.localIP());
    }

    wasConnected = true;
    reconnectLogged = false;
    return;
  }

  if (wasConnected) {
    Logger::warn("WiFi disconnected");
    wasConnected = false;
    reconnectLogged = false;
  }

  const unsigned long now = millis();

  if (now - lastReconnectAttemptMs >= reconnectIntervalMs) {
    if (!reconnectLogged) {
      Logger::warn("WiFi reconnecting");
      reconnectLogged = true;
    }

    connect();
  }
}

bool WiFiManager::isConnected() const {
  return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::connect() {
  lastReconnectAttemptMs = millis();
  WiFi.disconnect(false);
  WiFi.begin(ssid, password);
}
