int led = 13;
int pocet;
int i = 100;

void setup()
{
  pinMode(led, OUTPUT);
}

void loop() 
{
  while (i<=1000)
  {
    digitalWrite(led, HIGH);
    delay(i);
    digitalWrite(led, LOW);
    delay(i);
    i = i + 100;
  }
  i = 100;

}

