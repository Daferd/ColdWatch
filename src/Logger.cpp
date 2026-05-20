#include "Logger.h"

void Logger::begin(uint32_t baudRate) {
  Serial.begin(baudRate);
}

void Logger::info(const char* message) {
  printPrefix("INFO");
  Serial.println(message);
}

void Logger::warn(const char* message) {
  printPrefix("WARN");
  Serial.println(message);
}

void Logger::error(const char* message) {
  printPrefix("ERROR");
  Serial.println(message);
}

void Logger::infoTemperature(float temperatureC) {
  printPrefix("INFO");
  Serial.print("temperature_c=");
  Serial.println(temperatureC, 1);
}

void Logger::printPrefix(const char* level) {
  Serial.print('[');
  Serial.print(millis());
  Serial.print("] [");
  Serial.print(level);
  Serial.print("] ");
}
