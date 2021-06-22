#include <WiFi.h>
#include <WifiAP.h>
 
const char* ssid = "myAP";
const char* password =  "supakicka";
 
WiFiServer server(666);

void setup() {
 
    Serial.begin(115200);
    
    WiFi.softAP(ssid, password);
    WiFi.setSleep(false);
    IPAddress myIP = WiFi.softAPIP();    
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    server.begin();
    
    Serial.println("Server started");
}
// Strength + ";" + Angle + ";" + Fold + ";" + Strafe + ";" + Height + ";\n"

char buff[20];
char curr = -1;
int id = 0;

int idP = 0;

int strength = 0;
int angle = 0;
int fold = 0;
int strafe = 0;
int height = 0;

unsigned long currM;
unsigned long startM;


void loop() {
 
    WiFiClient client = server.available();
    
    if (client)
    {
        startM = millis();
        while (client.connected())
        {
            while (client.available()>0)
            {
                char c = client.read();
                if(c == 'i')
                {
                    startM = millis();
                }
                if(c == ';')
                {
                    buff[id] = '\0';
                    id = 0;
                    switch(idP)
                    {
                        case 0:
                            strength = atoi(buff);
                            break;
                        case 1:
                            angle = atoi(buff);
                            break;
                        case 2:
                            fold = atoi(buff);
                            break;
                        case 3:
                            strafe = atoi(buff);
                            break;
                        case 4:
                            height = atoi(buff);
                            Serial.print("Strength: "); Serial.print(strength); Serial.print(", Angle: "); Serial.print(angle); Serial.print(", Fold: "); Serial.print(fold);
                            Serial.print(", Strafe: "); Serial.print(strafe); Serial.print(", Height: "); Serial.println(height);
                            break;
                        default:
                            break;
                    }
                    idP++;
                    if(idP > 4)
                    {
                        idP = 0;
                    }
                }
                else
                {
                    buff[id] = c;
                    id++;
                }
            }
            delay(10);
            currM = millis();

            if(currM - startM > 2000)
            {
                break;
            }
        }
        
        client.stop();
        Serial.println("Client disconnected");
    }
}
