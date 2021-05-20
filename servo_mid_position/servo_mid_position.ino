#include <Servo.h> 

Servo myservo;

void setup() 
{ 
  Serial.begin(9600);
  myservo.attach(9);
  myservo.write(90);
  //myservo.write(93);  // set servo to mid-point
} 

void loop() 
{
  while (Serial.available() == 0);
  int val = Serial.parseInt(); //read int or parseFloat for ..float...
  if(val != 0){
    Serial.println(val);
    myservo.write(val);
  }
}
