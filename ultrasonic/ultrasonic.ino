#define ECHOPIN 2        // Echo pin z HC-SC04 na pin 2
#define TRIGPIN 3        // Trig pin z HC-SC04 na pin 3
#define led1 4 //ledka

//lcd_i2c
//#include <LiquidCrystal_I2C.h>
//#include <Wire.h>
//#define BACKLIGHT_PIN 3
// lcd(0x27, 2, 1, 0, 4, 5, 6, 7);

void setup()
{
//Nastaví sériovou komunikaci
Serial.begin(9600);
//Nastaví pin 2 jako vstupní
pinMode(ECHOPIN, INPUT);
//Nastaví pin 3 jako výstupní
pinMode(TRIGPIN, OUTPUT);

pinMode(led1, OUTPUT);

//lcd.begin(16, 2);
//lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
//lcd.setBacklight(HIGH);
 
}


void loop()
{
 // Vyšle impuls do modulu HC-SR04
  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);
  
  // Spočítá vzdálenost
  float distance = pulseIn(ECHOPIN, HIGH);
  distance= distance*0.017315f;
  
  // odešle informace na sérivý port
  Serial.print(distance);
  Serial.print("cm\n");

  //lcd.setCursor(0,0);
  //lcd.print(distance);
  //lcd.print("cm      ");

  if(distance<50)
  {
    digitalWrite(led1, HIGH);
  }
  else digitalWrite(led1, LOW);
  
  //počká 1 sekundu
  delay(100);  
}


//Více zde: http://arduino8.webnode.cz/news/lekce-9-merime-vzdalenost-s-hc-sr04/
