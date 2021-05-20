#include <LedControl.h>
LedControl lc=LedControl(12,10,11,0);

int inLeft = 7;
int valLeft = 0;
int inRight = 6;
int valRight = 0;
int row = 0;
int col = 0;

void setup() {
  Serial.begin(9600);
  pinMode(inLeft, INPUT);    // declare pushbutton as input
  pinMode(inRight, INPUT);    // declare pushbutton as input
  lc.shutdown(0,false);// turn off power saving, enables display
  lc.setIntensity(0,5);// sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  lc.setLed(0,row,col,true);
}

void loop(){
  valLeft = digitalRead(inLeft);
  valRight = digitalRead(inRight);
  if (valLeft == HIGH) 
  {
    row--;
    Serial.println(row);
    lc.setLed(0,row,col,true);   
    lc.setLed(0,row+2,col,false);
    Serial.println(row);
    if (row < 0)
    {
      row = 7;
      lc.setLed(0,row-6,col,false);
    }
    Serial.println(row);
    lc.setLed(0,row,col,true);
  }
  delay(50);
  if (valRight == HIGH)
  {
    row++;
    lc.setLed(0,row,col,true);   
    lc.setLed(0,row-2,col,false);
    if (row > 7)
    {
      row = 0;
    }
  }
  delay(50);
}
