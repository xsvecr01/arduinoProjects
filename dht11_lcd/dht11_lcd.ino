#include <DHT11.h>
int pin=4;
DHT11 dht11(pin); 

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define BACKLIGHT_PIN 3

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7);

void setup()
{
   Serial.begin(9600);
  while (!Serial) {
      ; // wait for serial port to connect. Needed for Leonardo only
    }
    lcd.begin(16, 2);
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);
}

void loop()
{
  int err;
  float temp, humi;
  if((err=dht11.read(humi, temp))==0)
  {
    Serial.print("temperature:");
    Serial.print(temp);
    Serial.print(" humidity:");
    Serial.print(humi);
    Serial.println();

    lcd.setCursor(0,0);
    lcd.print("Teplota:");
    lcd.print(temp);
    lcd.print("C");
    lcd.setCursor(0,1);
    lcd.print("Vlhkost:");
    lcd.print(humi);
    lcd.print("%");
    
  }
  else
  {
    Serial.println();
    Serial.print("Error No :");
    Serial.print(err);
    Serial.println();    
  }
  delay(DHT11_RETRY_DELAY); //delay for reread
}



