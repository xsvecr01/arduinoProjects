#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define BACKLIGHT_PIN 3

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7); 


#include <Keypad.h>

int dalsi = 0;
int i=0;
int z=0;

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

void setup(){
  lcd.begin(16, 2);
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH); 
  Serial.begin(9600);
  
}
  
void loop(){
 
  char customKey = customKeypad.getKey();
  
  if (customKey){
       if(dalsi>31){
        
        lcd.clear();
        dalsi=0;
        i=0;
        z=0;
        }
    dalsi++;
    if(dalsi<=16){

      lcd.setCursor(i,0);
      i++;
       lcd.print(customKey);
    Serial.println(customKey);
    }

    else{
      lcd.setCursor(z,1);
      z++;
        lcd.print(customKey);
    Serial.println(customKey);
   
    }
    }
    
 }

