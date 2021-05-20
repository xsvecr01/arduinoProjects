//hodiny s teploměrem na lcd
#include <DHT11.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define DS3231_I2C_ADDRESS 0x68

byte ut[8] = {
  0b00010,
  0b00100,
  0b10001,
  0b10001,
  0b10001,
  0b10011,
  0b01101,
  0b00000
};

byte ct[8] = {
  0b01010,
  0b00100,
  0b01110,
  0b10000,
  0b10000,
  0b10001,
  0b01110,
  0b00000
};

byte pa[8] = {
  0b00010,
  0b00100,
  0b01110,
  0b00001,
  0b01111,
  0b10001,
  0b01111,
  0b00000
};

byte decToBcd(byte val){
    return( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val){
    return( (val/16*10) + (val%16) );
}

LiquidCrystal_I2C  lcd(0x27,2,1,0,4,5,6,7); // 0x27 is the I2C bus address for an unmodified backpack

int pin=4;
DHT11 dht11(pin); 

void setup()
{
  Wire.begin();
  lcd.begin(16,2);
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.createChar(0, ut);
  lcd.createChar(1, ct);
  lcd.createChar(2, pa);
  lcd.clear();  
  // create a new character
  
}

void readDS3231time(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year){
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0); // set DS3231 register pointer to 00h
    Wire.endTransmission();
    Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
    // request seven bytes of data from DS3231 starting from register 00h
    *second = bcdToDec(Wire.read() & 0x7f);
    *minute = bcdToDec(Wire.read());
    *hour = bcdToDec(Wire.read() & 0x3f);
    *dayOfWeek = bcdToDec(Wire.read());
    *dayOfMonth = bcdToDec(Wire.read());
    *month = bcdToDec(Wire.read());
    *year = bcdToDec(Wire.read());
}

void displayTime(){
    byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
    // retrieve data from DS3231
    readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
    // send it to the serial monitor
    lcd.home();
    lcd.print(hour, DEC);
    // convert the byte variable to a decimal number when displayed
    lcd.print(":");
    if (minute<10){
        lcd.print("0");
    }
    lcd.print(minute, DEC);
    lcd.print(":");
    if (second<10){
        lcd.print("0");
    }
    lcd.print(second, DEC);
    lcd.setCursor(0,1);
    lcd.print(dayOfMonth, DEC);
    lcd.print("/");
    lcd.print(month, DEC);
    lcd.print("/");
    lcd.print(year, DEC);
    lcd.print(" ");
    switch(dayOfWeek){
        case 1:
            lcd.print("po");
            break;
        case 2:
            lcd.write(byte(0));
            lcd.print("t");
            break;
        case 3:
            lcd.print("st");
            break;
        case 4:
            lcd.write(byte(1));
            lcd.print("t");
            break;
        case 5:
            lcd.print("p");
            lcd.write(byte(2));
            break;
        case 6:
            lcd.print("so");
            break;
        case 7:
            lcd.print("ne");
            break;
    }
}

void dhtToLCD()
{
  int err;
  float temp, humi;
  if((err=dht11.read(humi, temp))==0)
  {
    int hum = (int) humi;
    int tem = (int) temp-1.5;
    String vlh = String(hum);
    String tep = String(tem);
    lcd.setCursor (12,0);        // go to start of 2nd line
    lcd.print(tep);
    lcd.print((char)223);
    lcd.print("C ");
    lcd.setCursor(12,1);
    lcd.print(vlh);
    lcd.print("%");        
  }
  
}

void loop()
{
  displayTime();
  dhtToLCD();
  delay(100);
}
