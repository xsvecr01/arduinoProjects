long randNumber;
int a;
int b;
int c;

void setup(){
  Serial.begin(9600);

  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));
}

void loop() {

  // print a random number from 10 to 19
  randNumber = random(0, 10000000000);
  Serial.println(randNumber);

  delay(1);
}
