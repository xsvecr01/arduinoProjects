#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
    <head>
        <title>xD</title>
        <meta content='width=device-width; initial-scale=1.0; maximum-scale=1.0; minimum-scale=1.0; user-scalable=no;' name='viewport'/>
        <style>
            #joystick-container {
            border: solid 1px #00FF00;
            display: inline-block;
            }
            #joystick {
                width: 100px;
                height: 100px;
                margin: 1px;
                cursor: crosshair;
            }
        </style>

    </head>
    
    <body class="noselect" onload="init();">
      <div id="joystick-container"><div id="joystick"></div></div>

        <p><strong>Tip:</strong> Try to click different places in the heading.</p>

        <p id="demo"></p>
        <p id="xD"></p>
    </body>
    
    <script>
        var joystick = document.getElementById('joystick');
        joystick.onmousemove = function (event) {
            //document.getElementById("xD").innerHTML = "touch";
            if(event.buttons === 1) {
                var x = event.clientX - joystick.getBoundingClientRect().left - 50;
                var y = - (event.clientY - joystick.getBoundingClientRect().top - 50);
                var moved = 0;
                if(x != 0 && y != 0)
                    moved = 1;
                    
                var angle = Math.atan(y/x) * 180/Math.PI;
                if(x < 0)
                    angle += 180;
                else if(y < 0)
                    angle += 360;
                if(angle < 90)
                    angle += 360;
                    angle -= 90;
                
                angle = parseInt(angle);
                
                document.getElementById("xD").innerHTML = angle;
                document.getElementById("demo").innerHTML = moved;
                
                var xmlhttp=new XMLHttpRequest();
                xmlhttp.open("PUT", moved + ";" + angle + ";", true);
                xmlhttp.send();
            }
        };
        
        joystick.onmouseup = function (event) {
            document.getElementById("xD").innerHTML = 0;
            document.getElementById("demo").innerHTML = 0;
            
            var xmlhttp=new XMLHttpRequest();
            xmlhttp.open("PUT", 0 + ";" + 0 + ";", true);
            xmlhttp.send();
        }

    </script>
</html>
)=====";

// Set these to your desired credentials.
const char *ssid = "yourAP";
const char *password = "yourPassword";

WiFiServer server(80);


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  WiFi.softAP(ssid, password);
  WiFi.setSleep(false);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
}

char input[100];
char inChar = -1;
int indx = 0;

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        //char c = client.read();             // read a byte, then
        if(client.read() == 'P')
        {
            if(client.read() == 'U')
            {
                if(client.read() == 'T')
                {
                    //client.read();
                    //client.read();
                    char c = client.read();
                    while(c != ' ')
                    {
                        input[indx] = c;
                        indx++;
                        input[indx] = '\0';
                        c = client.read();
                    }
                    indx = 0;
                    Serial.println(input);
                }
            }
        }
        //Serial.write(c);                    // print it out the serial monitor
        char c = client.read();
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            //client.print("Test website xD");
            client.print(MAIN_page);
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(LED_BUILTIN, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(LED_BUILTIN, LOW);                // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
  delay(1);
}
