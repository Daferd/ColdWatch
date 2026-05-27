#include <Arduino.h>

#include "config.h"
#include "Logger.h"
#include "MQTTManager.h"
#include "StatusLed.h"
#include "TemperatureSensor.h"
#include "WiFiManager.h"

TemperatureSensor temperatureSensor(Config::DhtPin, Config::DhtType);
WiFiManager wifiManager(
  Config::WifiSsid,
  Config::WifiPassword,
  Config::WifiHostname,
  Config::WifiReconnectIntervalMs,
  Config::WifiReadyDelayMs);
StatusLed statusLed(Config::StatusLedPin, Config::StatusLedBlinkIntervalMs);
MQTTManager mqttManager(
  Config::MqttBrokerHost,
  Config::MqttBrokerPort,
  Config::DeviceId,
  Config::MqttClientId,
  Config::MqttUsername,
  Config::MqttPassword,
  Config::MqttTemperatureTopic,
  Config::MqttReconnectIntervalMs,
  wifiManager);

unsigned long lastTemperatureReadMs = 0;

void setup() {
  Logger::begin(Config::SerialBaudRate);
  Logger::info("ColdWatch started");

  temperatureSensor.begin();
  statusLed.begin();
  wifiManager.begin();
  mqttManager.begin();
}

void loop() {
  wifiManager.update();
  mqttManager.update();
  statusLed.update(wifiManager.isConnected());

  const unsigned long now = millis();

  if (now - lastTemperatureReadMs >= Config::TemperatureReadIntervalMs) {
    lastTemperatureReadMs = now;

    float temperatureC = 0.0f;

    if (temperatureSensor.readTemperature(temperatureC)) {
      Logger::infoTemperature(temperatureC);
      mqttManager.publishTemperature(temperatureC);
    } else {
      Logger::error("DHT22 read failed");
    }
  }
}
