/*  ArduinoIDE 1.6.12 compiled on Linux Mint 17.3
    Sketch uses 245,617 bytes (23%) of program storage space. Maximum is 1,044,464 bytes.
    Global variables use 37,048 bytes (45%) of dynamic memory, leaving 44,804 bytes for local variables. Maximum is 81,920 bytes.
*/

#include "./functions.h"
#include <Streaming.h>                  // http://arduiniana.org/libraries/streaming/

const char* ssid     = "**SSID*";       // ERASE prior to publishing
const char* password = "wifipasswd";    //   "" ditto

int LED = 2;                            // NodeMCU by LoLin

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(19200); delay(100);
  Serial << (F("... Attempting to log into router... \r\n"));

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  WiFi.mode(WIFI_STA);

  Serial << (F("\n\nBOOTING ESP8266 ..."));
  Serial << (F("Connected to ")) << ssid << endl;
  Serial << (F("Station IP address: ")) << WiFi.localIP() << endl;

  server.on("/",          handleWebsite);
  server.on("/xml",       handleXML);
  server.on("/setESPval", handleESPval);
  server.begin();
}

void loop() {
  server.handleClient();
  if (millis() > wait000) {
    buildXML();
    wait000 = millis() + 1000UL;
  }
  if (millis() > wait001) {             // slider0 controls LED blink rate
    digitalWrite(LED, !digitalRead(LED));
    wait001 = millis() + ESPval[0];
  }
}

