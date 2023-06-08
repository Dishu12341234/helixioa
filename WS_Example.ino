#include <WiFiManager.h>
#include <WiFi.h>
#include <AsyncWebSocket.h>
#include <ESPAsyncWebSrv.h>
#include <SPIFFS.h>
#include <AsyncTCP.h>

#define LED 2
#define analogInput 33

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void setup() {
  Serial.begin(115200);
  WiFiManager wm;
  pinMode(LED, OUTPUT);
  pinMode(analogInput, INPUT_PULLDOWN);
  bool res = wm.autoConnect("Helixis");
  if (!res) {
    wm.resetSettings();
  }
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(50);
    digitalWrite(LED, HIGH);
    delay(10);
    digitalWrite(LED, LOW);
    delay(100);
  }

  Serial.println(WiFi.localIP());

  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *req) {
    req->send(SPIFFS,"/data/index.html", "text/html");
  });
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  int value = analogRead(analogInput);
  ws.textAll(String(value));
  ws.cleanupClients();
  delay(100);
}
