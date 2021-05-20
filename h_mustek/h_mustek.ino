int fwd = 3;
int bwd = 5;
//int motorspeed = 6;

void setup() 
{
  Serial.begin(9600);
  pinMode(fwd, OUTPUT);
  pinMode(bwd, OUTPUT);
  //pinMode(motorspeed, OUTPUT);
  // put your setup code here, to run once:
}

void loop() {

  analogWrite(fwd, 0);
  analogWrite(bwd, 255);
 /* digitalWrite(fwd, LOW);
  digitalWrite(bwd, HIGH);

  delay (500);

  digitalWrite(fwd, LOW);
  digitalWrite(bwd, LOW);

  delay(500);

  digitalWrite(fwd, HIGH);
  digitalWrite(bwd, LOW);

  delay(500);

  digitalWrite(fwd, LOW);
  digitalWrite(bwd, LOW);

  delay(500);
  // put your main code here, to run repeatedly:
*/
}
