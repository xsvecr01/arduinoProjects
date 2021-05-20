/* Demonstration sketch for PCF8574T I2C LCD Backpack 
Uses library from https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads GNU General Public License, version 3 (GPL-3.0) */
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
 
LiquidCrystal_I2C  lcd(0x27,2,1,0,4,5,6,7); // 0x27 is the I2C bus address for an unmodified backpack

byte ut[8] = {
  0b00010,
  0b00100,
  0b10001,
  0b10001,
  0b10001,
  0b10011,
  0b01101,
  0b00000
};

byte ct[8] = {
  0b01010,
  0b00100,
  0b01110,
  0b10000,
  0b10000,
  0b10001,
  0b01110,
  0b00000
};

byte pa[8] = {
  0b00010,
  0b00100,
  0b01110,
  0b00001,
  0b01111,
  0b10001,
  0b01111,
  0b00000
};
 
void setup()
{
  // activate LCD module
  lcd.begin (16,2); // for 16 x 2 LCD module
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
  // create a new character
  lcd.createChar(0, ut);
  lcd.createChar(1, ct);
  lcd.createChar(2, pa);
}

 
void loop()
{
  lcd.home (); // set cursor to 0,0
  lcd.print("účáuca");
  lcd.write(byte(0));
  lcd.write(byte(1));
  lcd.write(byte(2)); 
  lcd.setCursor (0,1);        // go to start of 2nd line
  lcd.print(millis());
}
