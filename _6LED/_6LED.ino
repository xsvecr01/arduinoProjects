//rozsvítí 1 náhodnou LED diodu z 6ti

int led1 = 11;
int led2 = 10;
int led3 = 9;
int led4 = 6;
int led5 = 5;
int led6 = 3;
int svit = 255;
int tma = 0;
int randNumber;

void setup() {
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  randomSeed(analogRead(0));

}

void loop() {
  analogWrite(led1, tma);
  analogWrite(led2, tma);
  analogWrite(led3, tma);
  analogWrite(led4, tma);
  analogWrite(led5, tma);
  analogWrite(led6, tma);
  
  randNumber = random(1,7);
  Serial.println(randNumber); 
   
  if(randNumber == 1){
    analogWrite(led1, svit);
  }
  else if(randNumber == 2){
    analogWrite(led2, svit);
  }
  else if(randNumber == 3){
    analogWrite(led3, svit);
  }
  else if(randNumber == 4){
    analogWrite(led4, svit);
  }
  else if(randNumber == 5){
    analogWrite(led5, svit);
  }
  else if (randNumber == 6){
    analogWrite(led6, svit);
  }
    
  delay(1000);

}
