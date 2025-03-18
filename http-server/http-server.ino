#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "FS.h"
#include <LittleFS.h>

const char* ssid = "SSID";        // Reemplaza con el nombre de tu red Wi-Fi
const char* password = "PASS";       // Reemplaza con tu contraseña

const int ledPin = 2;
String ledState;

AsyncWebServer server(8080);

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado. IP: " + WiFi.localIP().toString());

  if (!LittleFS.begin(true)) {  // true para formatear
    Serial.println("Error al montar LittleFS");
    return;
  }
  Serial.println("LittleFS montado correctamente");

  File root = LittleFS.open("/");
  File file = root.openNextFile();
  while (file) {
    Serial.print("Archivo: ");
    Serial.println(file.name());
    file = root.openNextFile();
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Solicitud recibida en /");
    request->send(LittleFS, "/index.html", "text/html");
  });

  server.begin();
  Serial.println("Servidor iniciado");
}

void loop() {}