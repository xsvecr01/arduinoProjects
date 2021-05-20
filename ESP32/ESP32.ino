
#define CHANNEL_0 0

#define FREQ 50

#define SERVO_PIN 18


float getDuty(int angle)
{
  return (0.055556 * angle + 2);
}

void SetAngle(uint8_t angle)
{
  float duty = (0.05555555555555555555555556 * angle + 2);
  //Serial.println(duty);
  ledcWrite(CHANNEL_0, int((4095*duty)/100));
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  ledcSetup(CHANNEL_0, FREQ, 12);
  ledcAttachPin(SERVO_PIN, CHANNEL_0);
  SetAngle(90);
}

void loop() {
  
  Serial.println("ahoj");
  //SetAngle(90);
  SetAngle(70);
  delay(500);
  SetAngle(110);
  delay(500);
  // put your main code here, to run repeatedly:

}
