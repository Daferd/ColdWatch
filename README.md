# ColdWatch

MVP IoT para monitoreo de temperatura en neveras comerciales.

## Stack

- ESP32
- PlatformIO
- MQTT (futuro)
- PostgreSQL (futuro)

## Objetivo actual

Leer temperatura usando DHT22 y mostrarla por Serial cada 5 segundos.

## Requisitos técnicos

- Código modular
- Evitar delay()
- Fácil de escalar
- Buenas prácticas

Resumen Técnico ColdWatch Firmware

ColdWatch es un firmware MVP IoT para ESP32 usando PlatformIO + Arduino Framework. La arquitectura está separada por módulos simples, sin FreeRTOS, sin callbacks complejos y sin sobreingeniería. El main.cpp solo orquesta begin() / update() de los módulos y la lectura periódica de temperatura.

Módulos actuales:

TemperatureSensor: encapsula DHT22, conserva último valor válido, timestamp de última lectura válida y evita lecturas demasiado frecuentes.
WiFiManager: gestiona conexión WiFi, hostname, reconexión automática, isConnected() e isReady(). MQTT solo se intenta cuando WiFi está conectado, tiene IP válida y pasó un delay de estabilización.
MQTTManager: usa PubSubClient, reconexión separada de WiFi, diagnóstico TCP previo a mqttClient.connect(), logs de éxito/fallo y mqttClient.state().
StatusLed: LED en GPIO 2. Fijo si WiFi está conectado, parpadeo no bloqueante si está desconectado.
Logger: logs por Serial con niveles INFO, WARN, ERROR.
TelemetryData: estructura de datos de telemetría.
TelemetrySerializer: serializer JSON minimalista con snprintf, sin librerías JSON pesadas ni String dinámicos grandes.
Tecnologías/librerías:

ESP32 Dev Module
PlatformIO
Arduino Framework
DHT sensor library
PubSubClient
WiFi.h
Flujo de telemetría:

main.cpp lee temperatura cada TemperatureReadIntervalMs usando millis().
Si la lectura DHT22 es válida, loguea temperatura.
Llama mqttManager.publishTemperature(temperatureC).
MQTTManager construye TelemetryData con:
device_id
temperature_c
uptime_ms
wifi_rssi
TelemetrySerializer genera JSON en buffer fijo.
Se loguea el payload generado.
Si MQTT está conectado, se publica al topic configurado.
Payload JSON actual:

{
"device_id": "device_001",
"temperature_c": 24.8,
"uptime_ms": 123456,
"wifi_rssi": -55
}
Topic MQTT actual:

coldwatch/device_001/temperature
Configuración central en include/config.h:

WiFi SSID/password/hostname
intervalos de reconexión WiFi/MQTT
delay de WiFi estable
broker MQTT host/port
MQTT client id/topic
device id
tamaño de buffer de payload
pin DHT22 y pin LED
Decisiones importantes:

Todo es no bloqueante con millis().
main.cpp no contiene lógica WiFi/MQTT/LED.
MQTT no conecta hasta que WiFi esté “ready”, no solo WL_CONNECTED.
Diagnóstico TCP separa problemas de red/socket de problemas MQTT.
JSON generado sin dependencia pesada.
El payload se loguea incluso si MQTT no está conectado, útil para avanzar backend/debug.
Estado actual:

Firmware compila correctamente.
Lectura DHT22, WiFi, LED, telemetría JSON y MQTT están implementados.
MQTT puede fallar por limitaciones de red/hotspot, pero el firmware ya genera y loguea telemetría válida para diseñar backend.
