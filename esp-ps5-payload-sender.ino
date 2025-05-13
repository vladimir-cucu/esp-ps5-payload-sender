#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <LittleFS.h> 

const char* ssid = "PS5_WEB_AP";
const char* password = "password";

IPAddress ip(10,1,1,1);
IPAddress gateway(10,1,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

void handleIndexPage() {
  server.send(200, "text/html",
    "<form method='POST' action='/send-payload'>"
    "PS5 IP: <input name='ip' value='10.1.1.100'><br>"
    "elfldr port: <input name='port' value='9021'><br>"
    "<input type='submit' value='Send kstuff-toggle'>"
    "</form>");
}

void handleSendPayload() {
  String ps5IP = server.arg("ip").c_str();
  uint16_t elfldrPort = server.arg("port").toInt();

  File kstuffToggleElf = LittleFS.open("/kstuff-toggle.elf", "r");
  if (!kstuffToggleElf) {
    server.send(500, "text/plain", "Failed to open payload.");
    return;
  }

  WiFiClient client;
  if (!client.connect(ps5IP, elfldrPort)) {
    server.send(500, "text/plain", "Failed to connect. Check PS5 IP and elfldr port.");
  }

  while (kstuffToggleElf.available()) {
    uint8_t buf[512];
    size_t len = kstuffToggleElf.read(buf, sizeof(buf));
    client.write(buf, len);
  }

  kstuffToggleElf.close();
  client.stop();
  server.send(200, "text/plain", "kstuff-toggle sent successfully.");
}

void setup() {
  LittleFS.begin();
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(ssid, password);

  server.on("/", handleIndexPage);
  server.on("/index.html", handleIndexPage);
  server.on("/send-payload", HTTP_POST, handleSendPayload);
  server.begin();
}

void loop() {
  server.handleClient();
}
