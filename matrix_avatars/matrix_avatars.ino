#include <LedControl.h>
// Pin 12 DATA IN - Pin 11 CS - Pin 10 CLK  
LedControl lc=LedControl(12,10,11,0);

byte smile[8]={B00111100,B01000010,B10100101,B10000001,B10100101,B10011001,B01000010,B00111100};
byte creeper[8]={B00000000,B01100110,B01100110,B00011000,B00111100,B00111100,B00100100,B00000000};



int button = 13;


void setup() 
{
  lc.shutdown(0,false);// turn off power saving, enables display
  lc.setIntensity(0,5);// sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen

  pinMode(button, INPUT_PULLUP);
}

int m_draw(byte pic[])
{
  int j = 7;
  for(int i = 0; i < 8; i++)
  {
    lc.setColumn(0,i,pic[j]);
    j--;
  }
  return 0;
}

int m_fill()
{
  for(int i = 0; i < random(1, 9); i++)
  {
    for(int j = 0; j < random(1, 9); j++)
    {
      lc.setLed(0, i, j, random(true, false));
      delay(100);      
    }
  }
}

void loop() {

  m_fill();
  delay(100);
  //m_draw(smile);
  //delay(5000);
  //m_draw(creeper);
  //delay(5000);

}
