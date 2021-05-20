#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);
const char* ssid="ESPap";
const char* password="thereisnospoon";
String webSite,javaScript,XML;
unsigned long wait000=0UL,wait001=1000UL;
int LED=16;
int sliderVal=60,blinkSpeed=1000;

void buildWebsite(){
  buildJavascript();
  webSite="<!DOCTYPE HTML>\n";
  webSite+="<META name='viewport' content='width=device-width, initial-scale=1'>\n";
  webSite+=javaScript;
  webSite+="<BODY onload='process()'>\n";
  webSite+="<BR>This is the ESP website ...<BR>\n";
  webSite+="Runtime = <A ID='runtime'></A><BR><BR>\n";
  webSite+="<INPUT ID='slider' TYPE='range' ONCHANGE='Slider()'><BR>\n";
  webSite+="Slidervalue = <A ID='Sliderval'></A><BR>\n";
  webSite+="Blinkspeed = <A ID='Blinkspeed'></A> milliseconds<BR>\n";
  webSite+="</BODY>\n";
  webSite+="</HTML>\n";
}

void buildJavascript(){
  javaScript="<SCRIPT>\n";
  javaScript+="xmlHttp=createXmlHttpObject();\n";

  javaScript+="function createXmlHttpObject(){\n";
  javaScript+="  if(window.XMLHttpRequest){\n";
  javaScript+="    xmlHttp=new XMLHttpRequest();\n";
  javaScript+="  }else{\n";
  javaScript+="    xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');\n";
  javaScript+="  }\n";
  javaScript+="  return xmlHttp;\n";
  javaScript+="}\n";

  javaScript+="function process(){\n";
  javaScript+="  if(xmlHttp.readyState==0||xmlHttp.readyState==4){\n";
  javaScript+="    xmlHttp.onreadystatechange=function(){\n";
  javaScript+="      if(xmlHttp.readyState==4&&xmlHttp.status==200){\n";
  javaScript+="        xmlDoc=xmlHttp.responseXML;\n";
  javaScript+="        xmlmsg=xmlDoc.getElementsByTagName('millistime')[0].firstChild.nodeValue;\n";
  javaScript+="        document.getElementById('runtime').innerHTML=xmlmsg;\n";
  javaScript+="        xmlmsg=xmlDoc.getElementsByTagName('sliderval')[0].firstChild.nodeValue;\n";
  javaScript+="        document.getElementById('slider').value=xmlmsg;\n";
  javaScript+="        xmlmsg=xmlDoc.getElementsByTagName('sliderval')[0].firstChild.nodeValue;\n";
  javaScript+="        document.getElementById('Sliderval').innerHTML=xmlmsg;\n";
  javaScript+="        xmlmsg=xmlDoc.getElementsByTagName('blinkspeed')[0].firstChild.nodeValue;\n";
  javaScript+="        document.getElementById('Blinkspeed').innerHTML=xmlmsg;\n";
  javaScript+="      }\n";
  javaScript+="    }\n";
  javaScript+="    xmlHttp.open('PUT','xml',true);\n";
  javaScript+="    xmlHttp.send(null);\n";
  javaScript+="  }\n";
  javaScript+="  setTimeout('process()',1000);\n";
  javaScript+="}\n";

  javaScript+="function Slider(){\n";
  javaScript+="  sliderVal=document.getElementById('slider').value;\n";
  javaScript+="  document.getElementById('Sliderval').innerHTML=sliderVal;\n";
  javaScript+="  document.getElementById('Blinkspeed').innerHTML=9*(100-sliderVal)+100;\n";
  javaScript+="  if(xmlHttp.readyState==0||xmlHttp.readyState==4){\n";
  javaScript+="    xmlHttp.open('PUT','setBlinkspeed?val='+sliderVal,true);\n";
  javaScript+="    xmlHttp.send(null);\n";
  javaScript+="  }\n";
  javaScript+="}\n";

  javaScript+="</SCRIPT>\n";
}

void buildXML(){
  XML="<?xml version='1.0'?>";
  XML+="<xml>";
  XML+="<millistime>";
  XML+=millis2time();
  XML+="</millistime>";
  XML+="<sliderval>";
  XML+=String(sliderVal);
  XML+="</sliderval>";
  XML+="<blinkspeed>";
  blinkSpeed=9*(100-sliderVal)+100;
  XML+=String(blinkSpeed);
  XML+="</blinkspeed>";
  XML+="</xml>";
}
String millis2time(){
  String Time="";
  unsigned long ss;
  byte mm,hh;
  ss=millis()/1000;
  hh=ss/3600;
  mm=(ss-hh*3600)/60;
  ss=(ss-hh*3600)-mm*60;
  if(hh<10)Time+="0";
  Time+=(String)hh+":";
  if(mm<10)Time+="0";
  Time+=(String)mm+":";
  if(ss<10)Time+="0";
  Time+=(String)ss;
  return Time;
}

void handleWebsite(){
  buildWebsite();
  server.send(200,"text/html",webSite);
}

void handleXML(){
  buildXML();
  server.send(200,"text/xml",XML);
}

void handleBlinkspeed(){
  sliderVal=server.arg("val").toInt();
  buildXML();
  server.send(200,"text/xml",XML);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED,OUTPUT);
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED)delay(500);
  WiFi.mode(WIFI_STA);
  Serial.println("\n\nBOOTING ESP8266 ...");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("Station IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/",handleWebsite);
  server.on("/xml",handleXML);
  server.on("/setBlinkspeed",handleBlinkspeed);
  server.begin();
}

void loop() {
  server.handleClient();
  if(millis()>wait000){
    buildXML();
    wait000=millis()+1000UL;
  }
  if(millis()>wait001){
    digitalWrite(LED,!digitalRead(LED));
    wait001=millis()+blinkSpeed;
  }
}
