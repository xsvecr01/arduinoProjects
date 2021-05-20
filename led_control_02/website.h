void buildWebsite() {
  buildJavascript();
  webSite = "<!DOCTYPE HTML>\n";
  webSite += "<META name='viewport' content='width=device-width, initial-scale=1'>\n";
  webSite += javaScript;
  webSite += "<BODY onload='process()'>\n";
   webSite += "<font face=\"Arial\">\r\n";
   webSite += "<table border=\"1\" align=\"center\" cellpadding=\"6\" name=\"Table\">\r\n";
   webSite += "<tr> <th align=\"center\" colspan=\"6\" bgcolor=\"#66FFCC\" >Led Control 02</th></tr>\r\n";

   webSite += "<tr> <td align=\"center\" bgcolor=\"#FFFF99\">\r\n";
   webSite += "Elapsed Time\r\n";
   webSite += "<br>\r\n";
   webSite += "<b id=\"tiempo\"></b>\r\n";
   webSite += "</td>\r\n";
   webSite += "</tr>\r\n";

   webSite += "<tr> <td align=\"center\" bgcolor=\"#FFFF99\">\r\n";
   webSite += "Delay Led \r\n";
   webSite += "<b id=\"duraT\"></b>\r\n";
   webSite += " Sec.\r\n";
   webSite += "<br>\r\n";
   webSite += "<input type=\"text\" name=\"duraID\" size=\"1\" id=\"duraID\" />\r\n";
   webSite += "<button onclick=\"DuraClick()\">Set</button>\r\n";
   webSite += "</td>\r\n";
   webSite += "</tr>\r\n";
   webSite += "<tr> <td align=\"center\" bgcolor=\"#FFFF99\">\r\n";
   webSite += "<button onclick=\"SirenaClick()\">Led ON</button>\r\n";
   webSite += "<br>\r\n";
   webSite += "</td>\r\n";
   webSite += "</tr>\r\n";
   
   webSite += "</table>\r\n";
   webSite += "</font>\r\n";
  webSite += "</BODY>\n";
  webSite += "</HTML>\n";
}


