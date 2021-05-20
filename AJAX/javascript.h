String webSite, javaScript, XML;
const int sliderMAX = 1;        // This sets the number of sliders you want
int sliderVal[sliderMAX] = {60}, ESPval[sliderMAX];

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
  javaScript += "        document.getElementById('runtime').innerHTML=xmlmsg;\n";
  javaScript += "        for(i=0;i<" + (String)sliderMAX + ";i++){\n";
  javaScript += "          xmlmsg=xmlDoc.getElementsByTagName('sliderval'+i)[0].firstChild.nodeValue;\n";
  javaScript += "          document.getElementById('slider'+i).value=xmlmsg;\n";
  javaScript += "          document.getElementById('Sliderval'+i).innerHTML=xmlmsg;\n";
  javaScript += "          xmlmsg=xmlDoc.getElementsByTagName('ESPval'+i)[0].firstChild.nodeValue;\n";
  javaScript += "          document.getElementById('ESPval'+i).innerHTML=xmlmsg;\n";
  javaScript += "        }\n";
  javaScript += "      }\n";
  javaScript += "    }\n";
  javaScript += "    xmlHttp.open('PUT','xml',true);\n";
  javaScript += "    xmlHttp.send(null);\n";
  javaScript += "  }\n";
  javaScript += "  setTimeout('process()',1000);\n";
  javaScript += "}\n";

  javaScript += "function Slider(cnt){\n";
  javaScript += "  sliderVal=document.getElementById('slider'+cnt).value;\n";
  javaScript += "  document.getElementById('Sliderval'+cnt).innerHTML=sliderVal;\n";
  javaScript += "  document.getElementById('ESPval'+cnt).innerHTML=9*(100-sliderVal)+100;\n";
  javaScript += "  if(xmlHttp.readyState==0||xmlHttp.readyState==4){\n";
  javaScript += "    xmlHttp.open('PUT','setESPval?cnt='+cnt+'&val='+sliderVal,true);\n";
  javaScript += "    xmlHttp.send(null);\n";
  javaScript += "  }\n";
  javaScript += "}\n";

  javaScript += "</SCRIPT>\n";
}

