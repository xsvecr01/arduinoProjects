#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>


// Include Wire Library for I2C
#include <Wire.h>

// Include Adafruit PCA9685 Servo Library
#include <Adafruit_PWMServoDriver.h>

// Creat object to represent PCA9685 at default I2C address
Adafruit_PWMServoDriver pca9685 = Adafruit_PWMServoDriver(0x40);

Adafruit_MPU6050 mpu;


// Define maximum and minimum number of "ticks" for the servo motors
// Range from 0 to 4095
// This determines the pulse width

#define SERVOMIN  80  // Minimum value
#define SERVOMAX  600  // Maximum value

// Define servo motor connections (expand as required)
#define SER00 5
#define SER01 18
#define SER0 0   //Servo Motor 0 on connector 0
#define SER1 1  //Servo Motor 1 on connector 12
#define SER2 2
#define SER3 3
#define SER4 4
#define SER5 5
#define SER6 6
#define SER7 7
#define SER8 8
#define SER9 9
#define SER10 10
#define SER11 11
#define SER12 12
#define SER13 13
#define SER14 14
#define SER15 15


// Variables for Servo Motor positions (expand as required)
int pwm0;
int pwm1;

int GetPwm(uint8_t angle)
{
  float duty = (0.05555555555555555555555556 * angle + 2);
  //Serial.println(duty);
  return int((4095*duty)/100);
}

void setup() {

  // Serial monitor setup
  Serial.begin(115200);

  ledcSetup(0, 50, 12);
  ledcSetup(1, 50, 12);
  ledcAttachPin(SER00, 0);
  ledcAttachPin(SER01, 1);
  
  // Print to monitor
  Serial.println("PCA9685 Servo Test");

  // Initialize PCA9685
  pca9685.begin();

  // Set PWM Frequency to 50Hz
  pca9685.setPWMFreq(50);
  
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
  ledcWrite(0, GetPwm(90));
  delay(100);
  ledcWrite(1, GetPwm(90));
  delay(100);
  pca9685.setPWM(SER0, 0, GetPwm(90));
  delay(100);
  pca9685.setPWM(SER1, 1, GetPwm(90));
  delay(100);
  pca9685.setPWM(SER2, 2, GetPwm(90));
  delay(100);
  pca9685.setPWM(SER3, 3, GetPwm(90));
  delay(100);
  pca9685.setPWM(SER4, 4, GetPwm(90));
  delay(100);
  pca9685.setPWM(SER5, 5, GetPwm(90));
  delay(100);
  pca9685.setPWM(SER6, 6, GetPwm(90));
  delay(100);
  pca9685.setPWM(SER7, 7, GetPwm(90));
  delay(100);
  pca9685.setPWM(SER8, 8, GetPwm(90));
  delay(100);
  pca9685.setPWM(SER9, 9, GetPwm(90));
  delay(100);
  pca9685.setPWM(SER10, 10, GetPwm(90));
  delay(100);
  pca9685.setPWM(SER11, 11, GetPwm(90));
  delay(100);
  pca9685.setPWM(SER12, 12, GetPwm(90));
  delay(100);
  pca9685.setPWM(SER13, 13, GetPwm(90));
  delay(100);
  pca9685.setPWM(SER14, 14, GetPwm(90));
  delay(100);
  pca9685.setPWM(SER15, 15, GetPwm(90));

}



void loop() {
/*
  pwm0 = map(0, 0, 180, SERVOMIN, SERVOMAX);
  pca9685.setPWM(SER0, 0, pwm0);
  pca9685.setPWM(SER1, 0, pwm0);
  delay(500);
  pwm0 = map(180, 0, 180, SERVOMIN, SERVOMAX);
  pca9685.setPWM(SER0, 0, pwm0);
  pca9685.setPWM(SER1, 0, pwm0);
  delay(500);*/
  pca9685.setPWM(SER0, 0, GetPwm(90));
  delay(500);
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  Serial.println("");

  ledcWrite(0, GetPwm(90));
  ledcWrite(1, GetPwm(90));
  pca9685.setPWM(SER0, 0, GetPwm(90));
  pca9685.setPWM(SER1, 1, GetPwm(90));
  pca9685.setPWM(SER2, 2, GetPwm(90));
  pca9685.setPWM(SER3, 3, GetPwm(90));
  pca9685.setPWM(SER4, 4, GetPwm(90));
  pca9685.setPWM(SER5, 5, GetPwm(90));
  pca9685.setPWM(SER6, 6, GetPwm(90));
  pca9685.setPWM(SER7, 7, GetPwm(90));
  pca9685.setPWM(SER8, 8, GetPwm(90));
  pca9685.setPWM(SER9, 9, GetPwm(90));
  pca9685.setPWM(SER10, 10, GetPwm(90));
  pca9685.setPWM(SER11, 11, GetPwm(90));
  pca9685.setPWM(SER12, 12, GetPwm(90));
  pca9685.setPWM(SER13, 13, GetPwm(90));
  pca9685.setPWM(SER14, 14, GetPwm(90));
  pca9685.setPWM(SER15, 15, GetPwm(90));
  
  //delay(500);
  /*pca9685.setPWM(SER0, 0, GetPwm(180));
  delay(500);
  pca9685.setPWM(SER0, 0, GetPwm(90));
  delay(500);
  pca9685.setPWM(SER0, 0, GetPwm(0));
  delay(500);*/

  
  // Move Motor 0 from 0 to 180 degrees
  /*for (int posDegrees = 0; posDegrees <= 180; posDegrees++) {

    // Determine PWM pulse width
    pwm0 = map(posDegrees, 0, 180, SERVOMIN, SERVOMAX);
    // Write to PCA9685
    pca9685.setPWM(SER0, 0, pwm0);
    // Print to serial monitor
    Serial.print("Motor 0 = ");
    Serial.println(posDegrees);
    //delay(30);
  }

  // Move Motor 1 from 180 to 0 degrees
  for (int posDegrees = 180; posDegrees >= 0; posDegrees--) {

    // Determine PWM pulse width
    pwm1 = map(posDegrees, 0, 180, SERVOMIN, SERVOMAX);
    // Write to PCA9685
    pca9685.setPWM(SER1, 0, pwm1);
    // Print to serial monitor
    Serial.print("Motor 1 = ");
    Serial.println(posDegrees);
    //delay(30);
  }

  // Move Motor 0 from 180 to 0 degrees
  for (int posDegrees = 180; posDegrees >= 0; posDegrees--) {

    // Determine PWM pulse width
    pwm0 = map(posDegrees, 0, 180, SERVOMIN, SERVOMAX);
    // Write to PCA9685
    pca9685.setPWM(SER0, 0, pwm0);
    // Print to serial monitor
    Serial.print("Motor 0 = ");
    Serial.println(posDegrees);
    //delay(30);
  }


  // Move Motor 1 from 0 to 180 degrees
  for (int posDegrees = 0; posDegrees <= 180; posDegrees++) {

    // Determine PWM pulse width
    pwm1 = map(posDegrees, 0, 180, SERVOMIN, SERVOMAX);
    // Write to PCA9685
    pca9685.setPWM(SER1, 0, pwm1);
    // Print to serial monitor
    Serial.print("Motor 1 = ");
    Serial.println(posDegrees);
    //delay(30);
  }*/


}
