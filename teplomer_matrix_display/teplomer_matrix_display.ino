#include <LedControl.h>
// Pin 12 DATA IN - Pin 10 CLK - Pin 11 CS 
LedControl lc=LedControl(12,10,11,0);

#include <DHT11.h>
DHT11 dht11(13);

void disp(int i, int j){
  switch(j){
    default:lc.setColumn(0, i, B00011110);
            lc.setColumn(0, i+1, B00100001);
            lc.setColumn(0, i+2, B00011110);
            break;
    case 1: lc.setColumn(0, i, B00001000);
            lc.setColumn(0, i+1, B00010000);
            lc.setColumn(0, i+2, B00111111);
            break;
    case 2: lc.setColumn(0, i, B00010011);
            lc.setColumn(0, i+1, B00100101);
            lc.setColumn(0, i+2, B00011001);
            break;
    case 3: lc.setColumn(0, i, B00100010);
            lc.setColumn(0, i+1, B00101001);
            lc.setColumn(0, i+2, B00010110);
            break;
    case 4: lc.setColumn(0, i, B00111100);
            lc.setColumn(0, i+1, B00000111);
            lc.setColumn(0, i+2, B00000100);
            break;
    case 5: lc.setColumn(0, i, B00111010);
            lc.setColumn(0, i+1, B00101001);
            lc.setColumn(0, i+2, B00100110);
            break;
    case 6: lc.setColumn(0, i, B00111110);
            lc.setColumn(0, i+1, B00101001);
            lc.setColumn(0, i+2, B00100110);
            break;
    case 7: lc.setColumn(0, i, B00111000);
            lc.setColumn(0, i+1, B00100000);
            lc.setColumn(0, i+2, B00111111);
            break;
    case 8: lc.setColumn(0, i, B00010110);
            lc.setColumn(0, i+1, B00101001);
            lc.setColumn(0, i+2, B00010110);
            break;
    case 9: lc.setColumn(0, i, B00011001);
            lc.setColumn(0, i+1, B00100101);
            lc.setColumn(0, i+2, B00011110);
            break;
  }
}

void setup() {
  lc.shutdown(0,false);// turn off power saving, enables display
  lc.setIntensity(0,15);// sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  lc.setRow(0, 0, B00000011);
  lc.setRow(0, 1, B00000011);
  // put your setup code here, to run once:

}

void loop() {

  for(int i=0; i<10; i++)
  {
    disp(0,i);
    delay(1000);
  }
  // put your main code here, to run repeatedly:

}
