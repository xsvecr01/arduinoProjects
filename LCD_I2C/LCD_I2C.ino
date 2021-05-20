#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define BACKLIGHT_PIN 3

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7); 

void setup() {

  lcd.begin(16, 2);
  //lcd.backight();
  //lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  //lcd.setBacklight(HIGH);  

}

void loop() {

  lcd.print("pes");
  // put your main code here, to run repeatedly:

}
