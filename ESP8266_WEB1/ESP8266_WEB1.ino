/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
#include <ESP8266WebServer.h>

/* Set these to your desired credentials. */
const char *ssid = "ESPap";
const char *password = "thereisnospoon";

String website, XML, javascript, datax, datay;
int Speed, Steer = 0;
int inc = 0;

ESP8266WebServer server(80);

void buildJAVASCRIPT()
{
  
  javascript = "<SCRIPT>\n";

  javascript+="function process(){\n";

  javascript+="var xmlString,datax,datay;\n";
  javascript+="datax = document.getElementById('sliderX').value;\n";
  javascript+="datay = document.getElementById('sliderY').value;\n";
  
  javascript+="var xmlHttp;\n";
  javascript+="if(window.XMLHttpRequest){\n";
  javascript+="xmlHttp = new XMLHttpRequest();\n";
  javascript+="}\n";
  javascript+="else if(window.ActiveXObject){\n";
  javascript+="xmlHttp = new ActiveXObject('Microsoft.XMLHTTP');\n";
  javascript+="}\n";
  
  javascript+="xmlString = '<xml><datax>'+datax+'</datax><datay>'+datay+'</datay></xml>';\n";
  javascript+="xmlHttp.open('PUT','/xml',true);\n";
  javascript+="xmlHttp.setRequestHeader('Content-Type','text/xml');\n";
  javascript+="xmlHttp.send(xmlString);\n";
  //javascript+="setTimeout('process()',500);\n";
  javascript+="}\n";

  javascript += "</SCRIPT>\n";

}

void buildHTML()
{

  website = "<!DOCTYPE HTML>\n";
  website += "<html>\n";
  website += "<body>\n";
  website += "<div id='sliderSteer'>\n";
  website += "<input type='range' min='65' max='145' value='105' id='sliderX' oninput='process()' style='width:50%; position:absolute; left:5%; top:50%;'>\n";
  website += "</div>\n";
  website += "<div id='sliderSpeed'>\n";
  website += "<input type='range' min='-255' max='255' value='0' id='sliderY' style='width:40%; -webkit-transform:rotate(270deg); position:absolute; left:60%; top:50%;'>\n";
  website += "</div>\n";
  website += "</body>\n";
  website += "</html>\n";

  buildJAVASCRIPT();
  website += javascript;
  server.send(200, "text/html", website);
}

void buildXML()
{
  
  XML = server.arg('xml');
 /* XML = "<?xml version='1.0'?>";
  XML+="<xml>";
  XML+="  <datax>";
  XML+=    datax;
  XML+="  </datax>";
  XML+="  <datay>";
  XML+=    datay;
  XML+="  </datay>";
  XML+="</xml>";*/

  server.send(200, "text/xml",XML);
  
}

void setup() {
  WiFi.disconnect();
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...\n");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", buildHTML);
  server.on("/xml", buildXML);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

}
