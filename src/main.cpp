#include <Arduino.h>

#include "config.h"
#include "Logger.h"
#include "StatusLed.h"
#include "TemperatureSensor.h"
#include "WiFiManager.h"

TemperatureSensor temperatureSensor(Config::DhtPin, Config::DhtType);
WiFiManager wifiManager(
  Config::WifiSsid,
  Config::WifiPassword,
  Config::WifiHostname,
  Config::WifiReconnectIntervalMs);
StatusLed statusLed(Config::StatusLedPin, Config::StatusLedBlinkIntervalMs);

unsigned long lastTemperatureReadMs = 0;

void setup() {
  Logger::begin(Config::SerialBaudRate);
  Logger::info("ColdWatch started");

  temperatureSensor.begin();
  statusLed.begin();
  wifiManager.begin();
}

void loop() {
  wifiManager.update();
  statusLed.update(wifiManager.isConnected());

  const unsigned long now = millis();

  if (now - lastTemperatureReadMs >= Config::TemperatureReadIntervalMs) {
    lastTemperatureReadMs = now;

    float temperatureC = 0.0f;

    if (temperatureSensor.readTemperature(temperatureC)) {
      Logger::infoTemperature(temperatureC);
    } else {
      Logger::error("DHT22 read failed");
    }
  }
}
