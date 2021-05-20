// ========================================================================
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "./javascript.h"

unsigned long wait000 = 0UL, wait001 = 1000UL;

ESP8266WebServer server(80);

void buildWebsite() {
  buildJavascript();
  webSite = "<!DOCTYPE HTML>\n";
  webSite += "<META name='viewport' content='width=device-width, initial-scale=1'>\n";
  webSite += javaScript;
  webSite += "<BODY onload='process()'>\n";
  webSite += "<BR>Change slider0 to control LED blink...<BR>\n";
  webSite += "Runtime = <A ID='runtime'></A><BR><BR>\n";
  webSite += "<TABLE BORDER=1 style='text-align:center;border-collapse:collapse'>\n";
  for (int i = 0; i < sliderMAX; i++) {
    webSite += "<TR>\n";
    webSite += "<TD><BR><INPUT ID='slider" + (String)i + "' TYPE='range' ONCHANGE='Slider(" + (String)i + ")'></TD>\n"; //in Firefox, Chrome and Edge use ONINPUT
    webSite += "<TD>Slidervalue" + (String)i + " = <A ID='Sliderval" + (String)i + "'></A><BR>\n";
    webSite += "ESPval" + (String)i + " = <A ID='ESPval" + (String)i + "'></A> milliseconds</TD>\n";
    webSite += "</TR>\n";
  }
  webSite += "</TABLE>\n";
  webSite += "</BODY>\n";
  webSite += "</HTML>\n";
}

String millis2time() {
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

void buildXML() {
  XML = "<?xml version='1.0'?>";
  XML += "<xml>";
  XML += "<millistime>";
  XML += millis2time();
  XML += "</millistime>";
  for (int i = 0; i < sliderMAX; i++) {
    XML += "<sliderval" + (String)i + ">";
    XML += String(sliderVal[i]);
    XML += "</sliderval" + (String)i + ">";
    XML += "<ESPval" + (String)i + ">";
    ESPval[i] = 9 * (100 - sliderVal[i]) + 100;
    XML += String(ESPval[i]);
    XML += "</ESPval" + (String)i + ">";
  }
  XML += "</xml>";
}

void handleWebsite() {
  buildWebsite();
  server.send(200, "text/html", webSite);
}

void handleXML() {
  buildXML();
  server.send(200, "text/xml", XML);
}

void handleESPval() {
  int sliderCNT = server.arg("cnt").toInt();
  sliderVal[sliderCNT] = server.arg("val").toInt();
  buildXML();
  server.send(200, "text/xml", XML);
}


