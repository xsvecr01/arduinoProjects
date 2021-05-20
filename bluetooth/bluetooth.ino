#include <SoftwareSerial.h>
 
// inicializace ser. portu na 2 a 3 pinu
//SoftwareSerial(RX, TX);
SoftwareSerial Bluetooth =  SoftwareSerial(2, 3);
 
 
void setup() {
  Bluetooth.begin(9600); // inicializace seriové linky
  delay(500);// čekej 0,5 sec  
}
 
void loop() {
  int i=0;
  while(1){
    Bluetooth.println(i); // odešli na ser. linku ahoj
    delay(1000);  //čekej 2 sec
    i++;
}
}
