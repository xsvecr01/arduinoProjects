#include <dht11.h>
dht11 cidlo;
int piezo = 53;
int horko = 50;
int dht11pin = 12;
int teplota;
int vlhkost;
String vstup;
boolean upozorneni = false;

void setup()
{
  Serial.begin(9600);
}

void loop()
{

  cidlo.read(dht11pin);
  teplota = cidlo.temperature;
  vlhkost = cidlo.humidity;

  if (Serial.available())
  {
   int input =Serial.read();
    vstup = String(input, DEC);
    odpovez(vstup);
  }


  if ( teplota > horko){
    tone(piezo, 1500);
    if(upozorneni == false){
      Serial.println("Je moc horko!");
      upozorneni = true;
    }
  }
  if (teplota < horko) {
    noTone(piezo);
    if(upozorneni == true){
      Serial.println("Uz neni horko.");
      upozorneni = false;
    }
  }
 delay(100);


}
void odpovez(String vstup)
{

  if (vstup == "116")
    {
      Serial.print("Teplota je ");
      Serial.print(teplota);
      Serial.println(" stupnu.");
    }
  else if(vstup == "118")
  {
    Serial.print("Vlhkost: ");
    Serial.print(vlhkost);
    Serial.println(" %.");
  }
  else
    {
      Serial.println("Ajaj, asi sem ti nerozumel. Napis \"t\" pro teplotu a \"v\" pro vlhkost.");
    }

}
