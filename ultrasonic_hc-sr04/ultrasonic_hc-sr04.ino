#define ECHOPIN 2
#define TRIGPIN 3

void setup()
{
Serial.begin(9600);
pinMode(ECHOPIN, INPUT);
pinMode(TRIGPIN, OUTPUT);
 
}


void loop()
{
  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);

  float distance = pulseIn(ECHOPIN, HIGH);
  distance= distance*0.017315f;
  
  Serial.print(distance);
  Serial.print("cm\n");
  delay(100);  
}
