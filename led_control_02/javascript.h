String webSite, javaScript, XML;

void buildJavascript() {
  javaScript = "<SCRIPT>\n";
  javaScript += "xmlHttp=createXmlHttpObject();\n";

  javaScript += "function createXmlHttpObject(){\n";
  javaScript += "  if(window.XMLHttpRequest){\n";
  javaScript += "    xmlHttp=new XMLHttpRequest();\n";
  javaScript += "  }else{\n";
  javaScript += "    xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');\n";
  javaScript += "  }\n";
  javaScript += "  return xmlHttp;\n";
  javaScript += "}\n";

  javaScript += "function process(){\n";
  javaScript += "  if(xmlHttp.readyState==0||xmlHttp.readyState==4){\n";
  javaScript += "    xmlHttp.onreadystatechange=function(){\n";
  javaScript += "      if(xmlHttp.readyState==4&&xmlHttp.status==200){\n";
  javaScript += "        xmlDoc=xmlHttp.responseXML;\n";
  javaScript += "        xmlmsg=xmlDoc.getElementsByTagName('millistime')[0].firstChild.nodeValue;\n";
  javaScript += "        document.getElementById('tiempo').innerHTML=xmlmsg;\n";
  javaScript += "        xmlmsg=xmlDoc.getElementsByTagName('duracion')[0].firstChild.nodeValue;\n";
  javaScript += "        document.getElementById('duraT').innerHTML=xmlmsg;\n";
  javaScript += "      }\n";
  javaScript += "    }\n";
  javaScript += "    xmlHttp.open('PUT','xml',true);\n";
  javaScript += "    xmlHttp.send(null);\n";
  javaScript += "  }\n";
  javaScript += "  setTimeout('process()',1000);\n";
  javaScript += "}\n";

  javaScript += "function SirenaClick(){\n";
  javaScript += " xmlHttp.open('SET','set1ESPval?Start=on',true);\n";
  javaScript += " xmlHttp.send(null);\n";
  javaScript += "}\n";

  javaScript += "function DuraClick(){\n";
  javaScript += "  var t = document.getElementById('duraID').value;\n";
  javaScript += " xmlHttp.open('SET','setDuraVal?Valor='+t,true);\n";
  javaScript += " xmlHttp.send(null);\n";
  javaScript += "}\n";
  
  javaScript += "</SCRIPT>\n";
}

