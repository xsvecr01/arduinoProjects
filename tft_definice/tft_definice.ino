#include <TouchScreen.h>

#include <pin_magic.h>
#include <registers.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
//#include <stdint.h>
#include "TouchScreen.h"
 
#define LCD_CS A3 //
#define LCD_CD A2 //
#define LCD_WR A1 //
#define LCD_RD A0 //
#define LCD_RESET A4 //
 
#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin
 
#define MINPRESSURE 10
#define MAXPRESSURE 1000
 
#define   BLACK   0x0000
#define WHITE   0xFFFF
 
int TS_MINX_1, TS_MINX_2, TS_MINY_1, TS_MINY_2, TS_MAXX_1, TS_MAXX_2, TS_MAXY_2, TS_MAXY_1 = 0;  
int test =0;
 
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
 
 
void setup(void) {
  Serial.begin(9600);
  Serial.println("TFT LCD TOUCH CALIBRATE  ");
 
  tft.reset();
  tft.begin(0x9341);
 
  tft.fillScreen(BLACK);
  test = 0;
 
}
void loop(void) {
 
if (test == 0) {tft.fillRect(0,0,5,5, WHITE);}
if (test == 1) {tft.fillRect(0,0,5,5, BLACK); tft.fillRect(0,tft.height()-5,5,5, WHITE);}
if (test == 2) {tft.fillRect(0,tft.height()-5,5,5, BLACK); tft.fillRect(tft.width()-5,tft.height()-5,5,5, WHITE);}
if (test == 3) {tft.fillRect(tft.width()-5,tft.height()-5,5,5, BLACK); tft.fillRect(tft.width()-5,0,5,5, WHITE);}
 
digitalWrite(13, HIGH);
TSPoint p = ts.getPoint();
digitalWrite(13, LOW);
pinMode(XM, OUTPUT);
pinMode(YP, OUTPUT);
 
 
if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
 
 
  if (test == 0)
 {
  Serial.println();
  Serial.println("Test TOP LEFT corner");
  TS_MINX_1 = p.x;
  TS_MINY_1 = p.y;
  Serial.println(p.x);
  Serial.println(p.y);
  Serial.println("----------");
  delay(1000);
}  
 
 if (test == 1)
 {
  Serial.println();
  Serial.println("Test DOWN LEFT corner");
  TS_MINX_2 = p.x;
  TS_MAXY_1 = p.y;
  Serial.println(p.x);
  Serial.println(p.y);
  Serial.println("----------");
  delay(1000);
 }
 
  if (test == 2)
  {
  Serial.println();
  Serial.println("Test DOWN RIGHT corner");
  TS_MAXX_1 = p.x;
  TS_MAXY_2 = p.y;
  Serial.println(p.x);
  Serial.println(p.y);
  Serial.println("----------");
  delay(1000);
 
 }
  if (test == 3)
 {
  Serial.println();
  Serial.println("Test TOP RIGHT corner");
  TS_MAXX_2 = p.x;
  TS_MINY_2 = p.y;
  Serial.println(p.x);
  Serial.println(p.y);
  Serial.println("----------");
 
 
  Serial.println("RESULT");
  Serial.print("#define TS_MINX "); Serial.println((TS_MINX_1+ TS_MINX_2)/2);
  Serial.print("#define TS_MINY "); Serial.println((TS_MINY_1+ TS_MINY_2)/2);
  Serial.print("#define TS_MAXX "); Serial.println((TS_MAXX_1+ TS_MAXX_2)/2);
  Serial.print("#define TS_MAXY "); Serial.println((TS_MAXY_1+ TS_MAXY_2)/2);
 
  tft.setTextColor(WHITE);
  tft.setCursor(60,30);
  tft.setTextSize(3);
  tft.print("RESULT");
  tft.setCursor(0,80);
  tft.setTextSize(2);
  tft.print("#define TS_MINX ");tft.println((TS_MINX_1+ TS_MINX_2)/2);
  tft.print("#define TS_MINY ");tft.println((TS_MINY_1+ TS_MINY_2)/2);
  tft.print("#define TS_MAXX ");tft.println((TS_MAXX_1+ TS_MAXX_2)/2);
  tft.print("#define TS_MAXY ");tft.println((TS_MAXY_1+ TS_MAXY_2)/2);
  while(1){}
 
 
 delay(500);
 
 }
test++;
}
 
 
   
   
   
   
 
 

    }
