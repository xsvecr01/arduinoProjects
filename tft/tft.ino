#include "TFTLCD.h"
#include "TouchScreen.h"


#define YP A3  
#define XM A2
#define YM 9
#define XP 8  


#define TS_MINX 882
#define TS_MINY 895
#define TS_MAXX 150
#define TS_MAXY 103

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 400);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

// Color definitions
#define  BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF



TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

#define BOXSIZE 40
#define PENRADIUS 3
int oldcolor, currentcolor;

void setup(void) {
 Serial.begin(9600);
 Serial.println("Paint!");
 
 tft.reset();

 tft.initDisplay();
 tft.fillScreen(BLACK);
 
 tft.fillRect(0, 0, BOXSIZE, BOXSIZE, RED);
 tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, YELLOW);
 tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, GREEN);
 tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, CYAN);
 tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, BLUE);
 tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, MAGENTA);
// tft.fillRect(BOXSIZE*6, 0, BOXSIZE, BOXSIZE, WHITE);

tft.drawRect(0, 0, BOXSIZE, BOXSIZE, WHITE);
currentcolor = RED;

 pinMode(13, OUTPUT);

}


#define MINPRESSURE 10
#define MAXPRESSURE 1000




void loop()
{
 digitalWrite(13, HIGH);
 Point p = ts.getPoint();
 digitalWrite(13, LOW);

 pinMode(XM, OUTPUT);
 pinMode(YP, OUTPUT);

 if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
/*
    Serial.print(p.y);
    Serial.print(" , ");
 Serial.println(p.x);
   delay(1);
 */  
   if (p.x < (TS_MINY-5)) {
     Serial.println("erase");
    // press the bottom of the screen to erase
     tft.fillRect(0, BOXSIZE, tft.width(), tft.height()-BOXSIZE, BLACK);
     
   }
   // turn from 0->1023 to tft.width
   p.y = map(p.y, TS_MINX, TS_MAXX, tft.width(), 0);
   p.x = map(p.x, TS_MINY, TS_MAXY, tft.height(), 0);

   if (p.x < BOXSIZE) {
      oldcolor = currentcolor;

 
      if (p.y < BOXSIZE) {
        currentcolor = RED;
        tft.drawRect(0, 0, BOXSIZE, BOXSIZE, WHITE);
      }
      else if (p.y < BOXSIZE*2) {
        currentcolor = YELLOW;
        tft.drawRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, WHITE);
      }
      else if (p.y < BOXSIZE*3) {
        currentcolor = GREEN;
        tft.drawRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, WHITE);
      }
      else if (p.y < BOXSIZE*4) {
        currentcolor = CYAN;
        tft.drawRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, WHITE);
      }
      else if (p.y < BOXSIZE*5) {
        currentcolor = BLUE;
        tft.drawRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, WHITE);
      }
      else if (p.y < BOXSIZE*6) {
        currentcolor = MAGENTA;
        tft.drawRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, WHITE);
      }
     
      if (oldcolor != currentcolor) {
         if (oldcolor == RED) tft.fillRect(0, 0, BOXSIZE, BOXSIZE, RED);
         if (oldcolor == YELLOW) tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, YELLOW);
         if (oldcolor == GREEN) tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, GREEN);
         if (oldcolor == CYAN) tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, CYAN);
         if (oldcolor == BLUE) tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, BLUE);
         if (oldcolor == MAGENTA) tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, MAGENTA);
      }
   }
   if (((p.x-PENRADIUS) > BOXSIZE) && ((p.x+PENRADIUS) < tft.height())) {
     tft.fillCircle(p.y, p.x, PENRADIUS, currentcolor);
   }
 }
}
