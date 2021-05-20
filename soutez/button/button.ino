
int btn1 = 10;
int btn2 = 12;

int state1 = 1;
int state2 = 1;
int last1;
int last2;
int i = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  state1 = digitalRead(btn1);
  state2 = digitalRead(btn2);
  

  if (state1 != last1 && state1 == LOW)
  {
    i++;
    Serial.println(i);
    delay(100);
  }
  
  if (state2 != last2 && state2 == LOW)
  {
    i--;
    Serial.println(i);
    delay(100);
  }

  last1 = state1;
  last2 = state2;  

}
