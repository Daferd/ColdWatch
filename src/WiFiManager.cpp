#include "WiFiManager.h"

#include <WiFi.h>

#include "Logger.h"

WiFiManager::WiFiManager(
  const char* ssid,
  const char* password,
  const char* hostname,
  uint32_t reconnectIntervalMs,
  uint32_t readyDelayMs)
  : ssid(ssid),
    password(password),
    hostname(hostname),
    reconnectIntervalMs(reconnectIntervalMs),
    readyDelayMs(readyDelayMs),
    lastReconnectAttemptMs(0),
    connectedSinceMs(0),
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
      connectedSinceMs = millis();
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
    connectedSinceMs = 0;
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

bool WiFiManager::isReady() const {
  if (!isConnected() || connectedSinceMs == 0) {
    return false;
  }

  if (WiFi.localIP() == IPAddress(0, 0, 0, 0)) {
    return false;
  }

  return millis() - connectedSinceMs >= readyDelayMs;
}

void WiFiManager::connect() {
  lastReconnectAttemptMs = millis();
  WiFi.disconnect(false);
  WiFi.begin(ssid, password);
}
