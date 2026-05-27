#include "MQTTManager.h"

#include "config.h"
#include "Logger.h"
#include "TelemetryData.h"
#include "TelemetrySerializer.h"

MQTTManager::MQTTManager(
  const char* brokerHost,
  uint16_t brokerPort,
  const char* deviceId,
  const char* clientId,
  const char* username,
  const char* password,
  const char* temperatureTopic,
  uint32_t reconnectIntervalMs,
  WiFiManager& wifiManager)
  : mqttClient(networkClient),
    wifiManager(wifiManager),
    brokerHost(brokerHost),
    brokerPort(brokerPort),
    deviceId(deviceId),
    clientId(clientId),
    username(username),
    password(password),
    temperatureTopic(temperatureTopic),
    reconnectIntervalMs(reconnectIntervalMs),
    lastReconnectAttemptMs(0),
    wasConnected(false),
    reconnectLogged(false),
    waitingForWifiLogged(false) {
}

void MQTTManager::begin() {
  mqttClient.setServer(brokerHost, brokerPort);
  Logger::info("MQTT ready");
}

void MQTTManager::update() {
  if (!wifiManager.isReady()) {
    if (wasConnected) {
      Logger::warn("MQTT disconnected: WiFi not ready");
      wasConnected = false;
    }

    reconnectLogged = false;
    lastReconnectAttemptMs = millis();

    if (!waitingForWifiLogged) {
      Logger::info("MQTT waiting for stable WiFi");
      waitingForWifiLogged = true;
    }

    return;
  }

  waitingForWifiLogged = false;

  if (mqttClient.connected()) {
    mqttClient.loop();

    if (!wasConnected) {
      Logger::info("MQTT connected");
    }

    wasConnected = true;
    reconnectLogged = false;
    return;
  }

  if (wasConnected) {
    Logger::warn("MQTT disconnected");
    wasConnected = false;
    reconnectLogged = false;
  }

  const unsigned long now = millis();

  if (now - lastReconnectAttemptMs >= reconnectIntervalMs) {
    if (!reconnectLogged) {
      Logger::warn("MQTT connecting");
      reconnectLogged = true;
    }

    connect();
  }
}

bool MQTTManager::isConnected() {
  return mqttClient.connected();
}

bool MQTTManager::publishTemperature(float temperatureC) {
  TelemetryData data = {
    deviceId,
    temperatureC,
    millis(),
    WiFi.RSSI()
  };

  char payload[Config::TelemetryPayloadBufferSize];

  if (!TelemetrySerializer::serializeTemperature(data, payload, sizeof(payload))) {
    Logger::error("Telemetry serialization failed");
    return false;
  }

  Logger::infoTelemetryPayload(payload);

  if (!isConnected()) {
    return false;
  }

  if (!mqttClient.publish(temperatureTopic, payload)) {
    Logger::warn("MQTT publish failed");
    return false;
  }

  return true;
}

void MQTTManager::connect() {
  lastReconnectAttemptMs = millis();

  if (!canOpenTcpConnection()) {
    return;
  }

  bool connected = false;

  if (hasCredentials()) {
    connected = mqttClient.connect(clientId, username, password);
  } else {
    connected = mqttClient.connect(clientId);
  }

  if (connected) {
    Logger::info("MQTT connect success");
    return;
  }

  logConnectFailure(mqttClient.state());
}

bool MQTTManager::canOpenTcpConnection() {
  WiFiClient tcpClient;

  if (!tcpClient.connect(brokerHost, brokerPort)) {
    Logger::error("TCP connection failed");
    return false;
  }

  Logger::info("TCP connection success");
  tcpClient.stop();
  return true;
}

bool MQTTManager::hasCredentials() const {
  return username != nullptr && username[0] != '\0';
}

void MQTTManager::logConnectFailure(int state) {
  Logger::error("MQTT connect failed");
  Logger::errorMqttState(state);
}
