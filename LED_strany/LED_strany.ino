//rozsvítí diody z jedné strany na druhou

int led1 = 11;
int led2 = 10;
int led3 = 9;
int led4 = 6;
int led5 = 5;
int led6 = 3;
int svit = 10;
int tma = 0;
int randNumber;

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);

}

void loop() {
  analogWrite(led1, tma);
  analogWrite(led2, tma);
  analogWrite(led3, tma);
  analogWrite(led4, tma);
  analogWrite(led5, tma);
  analogWrite(led6, tma);

  analogWrite(led1, svit);
  delay(50);
  analogWrite(led1, tma);
  analogWrite(led2, svit);
  delay(50);
  analogWrite(led2, tma);
  analogWrite(led3, svit);
  delay(50);
  analogWrite(led3, tma);
  analogWrite(led4, svit);
  delay(50);
  analogWrite(led4, tma);
  analogWrite(led5, svit);
  delay(50);
  analogWrite(led5, tma);
  analogWrite(led6, svit);
  delay(50);
  analogWrite(led6, tma);
  analogWrite(led5, svit);
  delay(50);
  analogWrite(led5, tma);
  analogWrite(led4, svit);
  delay(50);
  analogWrite(led4, tma);
  analogWrite(led3, svit);
  delay(50);
  analogWrite(led3, tma);
  analogWrite(led2, svit);
  delay(50);
  analogWrite(led2, tma);


    


}
