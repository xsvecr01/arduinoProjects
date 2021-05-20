#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
//#include <stdint.h>
#include "TouchScreen.h"
 
#define LCD_CS A3 //
#define LCD_CD A2 //
#define LCD_WR A1 //
#define LCD_RD A0 //
#define LCD_RESET A4 //
 
#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin
 
 
// definice max a min tlaku na TouchScreen
#define MINPRESSURE 10
#define MAXPRESSURE 1000
 
// tyto údaje jsme získali z kalibrace
#define TS_MINX 200
#define TS_MINY 75
#define TS_MAXX 872
#define TS_MAXY 908
 
// tady jsme si definovali cernou a bilou
#define   BLACK   0x0000
#define WHITE   0xFFFF
 
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300); // inicializace TouchScreen
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET); // inicializace LCD
 
int x, y = 0;
TSPoint p;
 
void setup(void) {
  tft.reset();
  tft.begin(0x9341); // inicializace naseho LCD shieldu
  tft.fillScreen(BLACK);
 
 
}
void loop(void) {
 
digitalWrite(13, HIGH); //aktivovaní cteni z TouchScreen
p = ts.getPoint(); // získání analogových hodnot
digitalWrite(13, LOW); //deaktivovani cteni z TouchScreen
// aby jsme mohli zase zadavat prikazy displeji
// jinak dotykovy panel toto pouziva jako INPUT
// protoze A1 i A2 pouziva LCD i TOUCHScreen
pinMode(XM, OUTPUT);
pinMode(YP, OUTPUT);
 
// když je tlak v mezich, mozno nastavit
if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
x = map(p.x, TS_MINX, TS_MAXX, 0, 240); // premapovani na pixely
y = map(p.y, TS_MINY, TS_MAXY, 0, 320); // premapovani na pixely
 
  if (x < 30 && y > 322) tft.fillScreen(BLACK);
  else tft.fillCircle(x,y,3, WHITE);
 
 
 }
 
}
