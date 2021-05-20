#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define BACKLIGHT_PIN 3


LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7); 

int sek = 0;
int minut = 0;
int hod = 0;

void setup()
{
  lcd.begin(16, 2);
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);

  lcd.setCursor(0,0);
  lcd.print("--uplynuly cas--");
     

}
void loop()
{
  lcd.setCursor(15,1);
  lcd.print("-");
  lcd.setCursor(14,1);
  lcd.print("-");
  lcd.setCursor(13,1);
  lcd.print("-");
  lcd.setCursor(12,1);
  lcd.print("-");
  lcd.setCursor(11,1);
  lcd.print(" ");
  lcd.setCursor(0,1);
  lcd.print("-");
  lcd.setCursor(1,1);
  lcd.print("-");
  lcd.setCursor(2,1);
  lcd.print("-");
  lcd.setCursor(3,1);
  lcd.print("-");
  sek=0;
   while(sek<60)
    {
    lcd.setCursor(4,1);
    lcd.print(hod);
    lcd.setCursor(6,1);
    lcd.print(":");
    lcd.setCursor(7,1);
    lcd.print(minut);
    lcd.setCursor(9,1);
    lcd.print(":");
    lcd.setCursor(10,1);
    lcd.print(sek);
    delay(990);
    sek=sek+1;   
   }
   minut=minut+1;
   if(minut>=60) 
    {
      lcd.setCursor(8,1);
      lcd.print(" ");
      hod=hod+1;
      minut=0;
      
    }

  
}
