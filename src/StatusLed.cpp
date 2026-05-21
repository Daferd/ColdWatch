#include "StatusLed.h"

StatusLed::StatusLed(uint8_t pin, uint32_t blinkIntervalMs)
  : pin(pin),
    blinkIntervalMs(blinkIntervalMs),
    lastToggleMs(0),
    ledOn(false) {
}

void StatusLed::begin() {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void StatusLed::update(bool wifiConnected) {
  if (wifiConnected) {
    if (!ledOn) {
      ledOn = true;
      digitalWrite(pin, HIGH);
    }

    return;
  }

  const unsigned long now = millis();

  if (now - lastToggleMs >= blinkIntervalMs) {
    lastToggleMs = now;
    ledOn = !ledOn;
    digitalWrite(pin, ledOn ? HIGH : LOW);
  }
}
