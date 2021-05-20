
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define BACKLIGHT_PIN 3


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

void setup(){
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH); 
}
  
void loop(){
  char customKey = customKeypad.getKey();

  Serial.println("A-had");
  Serial.println("B-cas");
  Serial.println("C-pis");
  Serial.println("D-rst");

  if(customKey){
   if (customKey=='A'){
    Serial.println("Ahoj");
   }}
   if(customKey){
    if (customKey=='B'){
      Serial.println("Aktualni cas");
    }}
   if(customKey){ 
     if (customKey=='C'){
      Serial.println("text");
    }}
    if(customKey){
     if (customKey=='D'){
      Serial.println("reset");
    }}
   if (customKey){Serial.println(customKey);}
}
