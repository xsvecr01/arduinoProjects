/*  
    Sketch uses 246.277 bytes (56%) of program storage space. Maximum is 434.160 bytes.
    Global variables use 37.284 bytes (45%) of dynamic memory, leaving 44.636 bytes for local variables. Maximum is 81.920 bytes.
    file : led_control_02.ino 

    Notas. 
    - delay send to esp8266 working
    - add save delay to eeprom
    - add static IPAddress ip(192,168,0,101);
*/
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "./javascript.h"
#include "./website.h"

unsigned long wait000 = 0UL, wait001 = 1000UL;

ESP8266WebServer server(80);

const char* ssid = "myModemRouter";
const char* password = "mypass";

const int LED = 4;
int addDura = 10; //delay address
String startS;
int duraI = 1;
/************************************************************************/
String elapsedTime() {
  String Time = "";
  unsigned long ss;
  byte mm, hh;
  ss = millis() / 1000;
  hh = ss / 3600;
  mm = (ss - hh * 3600) / 60;
  ss = (ss - hh * 3600) - mm * 60;
  if (hh < 10)Time += "0";
  Time += (String)hh + ":";
  if (mm < 10)Time += "0";
  Time += (String)mm + ":";
  if (ss < 10)Time += "0";
  Time += (String)ss;
  return Time;
}
/************************************************************************/
void buildXML() {

  XML = "<?xml version='1.0'?>";
  XML += "<xml>";
  XML += "<millistime>";
  XML += elapsedTime();
  XML += "</millistime>";
  XML += "<duracion>";
  XML += duraI;
  XML += "</duracion>";
  XML += "</xml>";
}
/************************************************************************/
void handleWebsite() {
  buildWebsite();
  server.send(200, "text/html", webSite);
}
/************************************************************************/
void handleXML() {
  buildXML();
  server.send(200, "text/xml", XML);
}
/***********************************************************************/
void handleESPval() {
  startS = server.arg("Start");
  if (startS == "on")
  {
    digitalWrite(LED, HIGH);
    delay(duraI * 1000);
    digitalWrite(LED, LOW);
  }
  else
    digitalWrite(LED, LOW);

  //Serial.print("start=");
  //Serial.println(startS);
}
/************************************************************************/
void handle_VAL()
{

  int valorI = server.arg("Valor").toInt();
  if (valorI < 0)
    valorI = 1;
  duraI = valorI;
  EEPROM.write(addDura, duraI);
  delay(100);
  EEPROM.commit();
  //Serial.print("Valor = ");
  //Serial.println(valorI);
  //Serial.print("Valor = ");
  //Serial.println(EEPROM.read(addDura));

}
/***********************************************************************/
void setup() {
  Serial.begin(115200); delay(100);
  pinMode(LED, OUTPUT);
  EEPROM.begin(512);
  duraI = EEPROM.read(addDura);
  if (duraI < 1 || duraI > 10) duraI = 1;
  
  Serial.println("... Attempting to log into router... \r\n");

  WiFi.begin(ssid, password);
  
  IPAddress ip(192,168,0,101);
  IPAddress gateway(192,168,0,1);   
  IPAddress subnet(255,255,255,0);   
  WiFi.config(ip, gateway, subnet);
  
  while (WiFi.status() != WL_CONNECTED) delay(500);
  WiFi.mode(WIFI_STA);

  Serial.println(F("\n\nBOOTING ESP8266 ..."));
  Serial.print(F("Connected to "));
  Serial.println(ssid);
  Serial.print(F("Station IP address: "));
  Serial.println(WiFi.localIP());

  server.on("/",          handleWebsite);
  server.on("/xml",       handleXML);
  server.on("/set1ESPval", handleESPval);
  server.on("/setDuraVal", handle_VAL);
  server.begin();

}
/*************************************************************************/
void loop() {

  server.handleClient();
  if (millis() > wait000) {
    buildXML();
    wait000 = millis() + 500UL;
  }
}
/************************************************************************/

