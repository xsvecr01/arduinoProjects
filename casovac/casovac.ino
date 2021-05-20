int sek = 0;
int minut = 0;
int hod = 0;

void setup() {
  Serial.begin(9600);

}

void loop() {
   
   sek=0;
   while(sek<60)
    {
    Serial.print(hod);
    Serial.print(":");
    Serial.print(minut);
    Serial.print(":");
    Serial.print(sek);
    Serial.println();
    delay(1000);
    sek=sek+1;   
   }
   minut=minut+1;
   if(minut>=60) 
    {
      hod=hod+1;
      minut=0;
    }
   

    
  

    
}
