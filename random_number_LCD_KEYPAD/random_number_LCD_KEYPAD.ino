#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define BACKLIGHT_PIN 3
#include <Keypad.h>

int randNumber;
int a;
int b;



LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7); 

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 



void setup() {

  lcd.begin(16, 2);
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH); 

}

void loop() {
  char customKey = customKeypad.getKey();
  lcd.setCursor(0,0);
  lcd.print("minimum-potvrd A");
  if(customKey){
  while(customKey!='A'){
    lcd.setCursor(0,1);
    lcd.print(customKey);
  }}
    lcd.clear();
    lcd.setCursor(0,0);
  lcd.setCursor(0,0);
  lcd.print("maximum-potvrd B");
  if(customKey){
    lcd.setCursor(0,1);
    lcd.print(customKey);
  }
  if(customKey=='B'){
    lcd.clear();
    lcd.setCursor(0,0);
  }
  

  
}
