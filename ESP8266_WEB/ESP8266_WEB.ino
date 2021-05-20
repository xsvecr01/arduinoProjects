/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

/* Set these to your desired credentials. */
const char *ssid = "ESPap";
const char *password = "thereisnospoon";

String website, data, XML, javascript;
int inc=0;

ESP8266WebServer server(80);

void buildJAVASCRIPT()
{
  javascript = "<SCRIPT>\n";
  //javascript += "alert('pes');\n";
  //javascript +="var speed=getElementById('sliderSpeed').value;\n";
  //javascript +="var steer=getElementById('sliderSteer').value;\n";
  
  javascript += "var xmlHttp=createXmlHttpObject();\n";


  javascript += "function createXmlHttpObject(){\n";  
  javascript += "if(window.XMLHttpRequest){\n";
  javascript += "xmlHttp=new XMLHttpRequest();\n";  
  javascript += "}else{\n";
  javascript += "xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');\n";
  javascript += "}\n";  
  javascript += "return xmlHttp;\n";
  javascript += "}\n";
  javascript += "\n";
  
  
  javascript += "function response(){\n";
  javascript += "xmlResponse=xmlHttp.responseXML;\n";
  javascript += "xmldoc = xmlResponse.getElementsByTagName('data');\n";
  javascript += "message = xmldoc[0].firstChild.nodeValue;\n";
  javascript += "document.getElementById('div1').innerHTML=message;\n";
  javascript += "}\n";

  javascript += "function process(){\n";
  javascript += "xmlHttp.open('PUT','xml',true);\n";
  javascript += "xmlHttp.onreadystatechange=response;\n";
  javascript += "xmlHttp.send(null);\n";
  javascript += "setTimeout('process()',200);\n";
  javascript += "}\n";
  
  javascript += "</SCRIPT>\n";

  
}

void buildHTML()
{
  /*
  website = "<!DOCTYPE HTML>\n";
  website += "<html>\n";
  website += "<body>\n";
  website += "<div id='sliderSteer'>\n";
  website += "<input type='range' min='65' max='145' value='105' class='slider' id='myRange' style='width:50%; position:absolute; left:5%; top:50%;'>\n";
  website += "</div>\n";    
  website += "<div id='sliderSpeed'>\n";
  website += "<input type='range' min='-255' max='255' value='0' class='slider' id='myRange' style='width:40%; -webkit-transform:rotate(270deg); position:absolute; left:60%; top:50%;'>\n";
  website += "</div>\n";
  website += "</body>\n";
  website += "</html>\n";
*/

  website ="<html>\n";
  website +="<body onload='process()'><div id='div1'>"+data+"</div></body>";
  website +="</html>\n";

  buildJAVASCRIPT();
  website+=javascript;
  server.send(200, "text/html", website);
}



void buildXML()
{
  XML = "<?xml version='1.0'?>";
  XML += "<data>";
  XML += data;
  XML += "</data>";

  server.send(200, "text/xml", XML);
}



/* 
void handleHTML() {
  buildWebsite();
  buildXML();
	server.send(200, "text/html", website);
  server.send(200, "text/xml", XML);
}*/

void setup() {
  WiFi.disconnect();
	delay(1000);
	Serial.begin(115200);
	Serial.println();
	Serial.print("Configuring access point...");
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

  inc++;
  data = (String)inc;
	server.handleClient();
}
