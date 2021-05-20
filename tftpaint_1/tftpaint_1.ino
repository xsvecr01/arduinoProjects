// Paint example specifically for the TFTLCD breakout board.
// If using the Arduino shield, use the tftpaint_shield.pde sketch instead!
// DOES NOT CURRENTLY WORK ON ARDUINO LEONARDO

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7

// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).
//   D0 connects to digital pin 22
//   D1 connects to digital pin 23
//   D2 connects to digital pin 24
//   D3 connects to digital pin 25
//   D4 connects to digital pin 26
//   D5 connects to digital pin 27
//   D6 connects to digital pin 28
//   D7 connects to digital pin 29

// For the Arduino Due, use digital pins 33 through 40
// (on the 2-row header at the end of the board).
//   D0 connects to digital pin 33
//   D1 connects to digital pin 34
//   D2 connects to digital pin 35
//   D3 connects to digital pin 36
//   D4 connects to digital pin 37
//   D5 connects to digital pin 38
//   D6 connects to digital pin 39
//   D7 connects to digital pin 40

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define TS_MINX 98
#define TS_MINY 111
#define TS_MAXX 802
#define TS_MAXY 894

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

// Assign human-readable names to some common 16-bit color values:
#define BLACK           0x0000      /*   0,   0,   0 */
#define NAVY            0x000F      /*   0,   0, 128 */
#define DARKGREEN       0x03E0      /*   0, 128,   0 */
#define DARKCYAN        0x03EF      /*   0, 128, 128 */
#define MAROON          0x7800      /* 128,   0,   0 */
#define PURPLE          0x780F      /* 128,   0, 128 */
#define OLIVE           0x7BE0      /* 128, 128,   0 */
#define LIGHTGREY       0xC618      /* 192, 192, 192 */
#define DARKGREY        0x7BEF      /* 128, 128, 128 */
#define BLUE            0x001F      /*   0,   0, 255 */
#define GREEN           0x07E0      /*   0, 255,   0 */
#define CYAN            0x07FF      /*   0, 255, 255 */
#define RED             0xF800      /* 255,   0,   0 */
#define MAGENTA         0xF81F      /* 255,   0, 255 */
#define YELLOW          0xFFE0      /* 255, 255,   0 */
#define WHITE           0xFFFF      /* 255, 255, 255 */
#define ORANGE          0xFD20      /* 255, 165,   0 */
#define GREENYELLOW     0xAFE5      /* 173, 255,  47 */
#define PINK            0xF81F


Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

#define BOXSIZE 24
int PENRADIUS = 1;
int oldcolor, currentcolor;

void setup(void) {
  Serial.begin(9600);
  Serial.println(F("Paint!"));
  
  tft.reset();
  
  uint16_t identifier = tft.readID();

  if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    return;
  }

  tft.begin(identifier);

  tft.fillScreen(WHITE);

  tft.fillRect(0, 0, BOXSIZE, BOXSIZE, BLACK);
  tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, NAVY);
  tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, DARKGREEN);
  tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, DARKCYAN);
  tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, MAROON);
  tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, PURPLE);
  tft.fillRect(BOXSIZE*6, 0, BOXSIZE, BOXSIZE, OLIVE);
  tft.fillRect(BOXSIZE*7, 0, BOXSIZE, BOXSIZE, LIGHTGREY);
  tft.fillRect(BOXSIZE*8, 0, BOXSIZE, BOXSIZE, DARKGREY);
  tft.fillRect(BOXSIZE*9, 0, BOXSIZE, BOXSIZE, BLUE);
  
  tft.fillRect(0, BOXSIZE, BOXSIZE, BOXSIZE, GREEN);
  tft.fillRect(BOXSIZE, BOXSIZE, BOXSIZE, BOXSIZE, CYAN);
  tft.fillRect(BOXSIZE*2, BOXSIZE, BOXSIZE, BOXSIZE, RED);
  tft.fillRect(BOXSIZE*3, BOXSIZE, BOXSIZE, BOXSIZE, MAGENTA);
  tft.fillRect(BOXSIZE*4, BOXSIZE, BOXSIZE, BOXSIZE, YELLOW);
  tft.fillRect(BOXSIZE*5, BOXSIZE, BOXSIZE, BOXSIZE, WHITE);
  tft.fillRect(BOXSIZE*6, BOXSIZE, BOXSIZE, BOXSIZE, ORANGE);
  tft.fillRect(BOXSIZE*7, BOXSIZE, BOXSIZE, BOXSIZE, GREENYELLOW);
  tft.fillRect(BOXSIZE*8, BOXSIZE, BOXSIZE, BOXSIZE, RED);
  tft.fillCircle(BOXSIZE*8.5, BOXSIZE*1.5, BOXSIZE/7, BLACK);
  tft.fillRect(BOXSIZE*9, BOXSIZE, BOXSIZE, BOXSIZE, RED);
  tft.drawLine(BOXSIZE*9, BOXSIZE, BOXSIZE*10-1, BOXSIZE*2-1, BLACK);
  tft.drawLine(BOXSIZE*9, BOXSIZE*2-1, BOXSIZE*10-1, BOXSIZE, BLACK);
   
  tft.drawRect(0, 0, BOXSIZE, BOXSIZE, WHITE);
  currentcolor = BLACK;
 
  pinMode(13, OUTPUT);
}

#define MINPRESSURE 10
#define MAXPRESSURE 1000

void loop()
{
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    /*
    Serial.print("X = "); Serial.print(p.x);
    Serial.print("\tY = "); Serial.print(p.y);
    Serial.print("\tPressure = "); Serial.println(p.z);
    */
    /*
    if (p.y < (TS_MINY-5)) {
      Serial.println("erase");
      // press the bottom of the screen to erase 
      tft.fillRect(0, BOXSIZE, tft.width(), tft.height()-BOXSIZE, WHITE);
    }*/
    // scale from 0->1023 to tft.width
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
    /*
    Serial.print("("); Serial.print(p.x);
    Serial.print(", "); Serial.print(p.y);
    Serial.println(")");
    */
    if (p.y < BOXSIZE) 
    {
       oldcolor = currentcolor;

       if (p.x < BOXSIZE) {
         currentcolor = BLACK; 
         tft.drawRect(0, 0, BOXSIZE, BOXSIZE, WHITE);
       } else if (p.x < BOXSIZE*2) {
         currentcolor = NAVY;
         tft.drawRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, BLACK);
       } else if (p.x < BOXSIZE*3) {
         currentcolor = DARKGREEN;
         tft.drawRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, BLACK);
       } else if (p.x < BOXSIZE*4) {
         currentcolor = DARKCYAN;
         tft.drawRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, BLACK);
       } else if (p.x < BOXSIZE*5) {
         currentcolor = MAROON;
         tft.drawRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, BLACK);
       } else if (p.x < BOXSIZE*6) {
         currentcolor = PURPLE;
         tft.drawRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, BLACK);
       } else if (p.x < BOXSIZE*7) {
         currentcolor = OLIVE;
         tft.drawRect(BOXSIZE*6, 0, BOXSIZE, BOXSIZE, BLACK);
       } else if (p.x < BOXSIZE*8) {
         currentcolor = LIGHTGREY;
         tft.drawRect(BOXSIZE*7, 0, BOXSIZE, BOXSIZE, BLACK);
       } else if (p.x < BOXSIZE*9) {
         currentcolor = DARKGREY;
         tft.drawRect(BOXSIZE*8, 0, BOXSIZE, BOXSIZE, BLACK);
       } else if (p.x < BOXSIZE*10) {
         currentcolor = BLUE;
         tft.drawRect(BOXSIZE*9, 0, BOXSIZE, BOXSIZE, BLACK);
       }

       if (oldcolor != currentcolor) {
          if (oldcolor == BLACK) tft.fillRect(0, 0, BOXSIZE, BOXSIZE, BLACK);
          if (oldcolor == NAVY) tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, NAVY);
          if (oldcolor == DARKGREEN) tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, DARKGREEN);
          if (oldcolor == DARKCYAN) tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, DARKCYAN);
          if (oldcolor == MAROON) tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, MAROON);
          if (oldcolor == PURPLE) tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, PURPLE);
          if (oldcolor == OLIVE) tft.fillRect(BOXSIZE*6, 0, BOXSIZE, BOXSIZE, OLIVE);
          if (oldcolor == LIGHTGREY) tft.fillRect(BOXSIZE*7, 0, BOXSIZE, BOXSIZE, LIGHTGREY);
          if (oldcolor == DARKGREY) tft.fillRect(BOXSIZE*8, 0, BOXSIZE, BOXSIZE, DARKGREY);
          if (oldcolor == BLUE) tft.fillRect(BOXSIZE*9, 0, BOXSIZE, BOXSIZE, BLUE);
          if (oldcolor == GREEN) tft.fillRect(0, BOXSIZE, BOXSIZE, BOXSIZE, GREEN);
          if (oldcolor == CYAN) tft.fillRect(BOXSIZE, BOXSIZE, BOXSIZE, BOXSIZE, CYAN);
          if (oldcolor == RED) tft.fillRect(BOXSIZE*2, BOXSIZE, BOXSIZE, BOXSIZE, RED);
          if (oldcolor == MAGENTA) tft.fillRect(BOXSIZE*3, BOXSIZE, BOXSIZE, BOXSIZE, MAGENTA);
          if (oldcolor == YELLOW) tft.fillRect(BOXSIZE*4, BOXSIZE, BOXSIZE, BOXSIZE, YELLOW);
          if (oldcolor == WHITE) tft.fillRect(BOXSIZE*5, BOXSIZE, BOXSIZE, BOXSIZE, WHITE);
          if (oldcolor == ORANGE) tft.fillRect(BOXSIZE*6, BOXSIZE, BOXSIZE, BOXSIZE, ORANGE);
          if (oldcolor == GREENYELLOW) tft.fillRect(BOXSIZE*7, BOXSIZE, BOXSIZE, BOXSIZE, GREENYELLOW);
        }
    }
    if(p.y > BOXSIZE && p.y < BOXSIZE*2)
    {
       oldcolor = currentcolor;        

       if (p.x < BOXSIZE && p.y > BOXSIZE) {
         currentcolor = GREEN;
         tft.drawRect(0, BOXSIZE, BOXSIZE, BOXSIZE, BLACK);
       } else if (p.x < BOXSIZE*2) {
         currentcolor = CYAN;
         tft.drawRect(BOXSIZE*1, BOXSIZE, BOXSIZE, BOXSIZE, BLACK);
       } else if (p.x < BOXSIZE*3) {
         currentcolor = RED;
         tft.drawRect(BOXSIZE*2, BOXSIZE, BOXSIZE, BOXSIZE, BLACK);
       } else if (p.x < BOXSIZE*4) {
         currentcolor = MAGENTA;
         tft.drawRect(BOXSIZE*3, BOXSIZE, BOXSIZE, BOXSIZE, BLACK);
       } else if (p.x < BOXSIZE*5) {
         currentcolor = YELLOW;
         tft.drawRect(BOXSIZE*4, BOXSIZE, BOXSIZE, BOXSIZE, BLACK);
       } else if (p.x < BOXSIZE*6) {
         currentcolor = WHITE;
         tft.drawRect(BOXSIZE*5, BOXSIZE, BOXSIZE, BOXSIZE, BLACK);
       } else if (p.x < BOXSIZE*7) {
         currentcolor = ORANGE;
         tft.drawRect(BOXSIZE*6, BOXSIZE, BOXSIZE, BOXSIZE, BLACK);
       } else if (p.x < BOXSIZE*8) {
         currentcolor = GREENYELLOW;
         tft.drawRect(BOXSIZE*7, BOXSIZE, BOXSIZE, BOXSIZE, BLACK);
       } else if (p.x < BOXSIZE*9) {
         PENRADIUS ++;
         switch (PENRADIUS){
          case 2: tft.fillRect(BOXSIZE*8, BOXSIZE, BOXSIZE, BOXSIZE, RED);
                  tft.fillCircle(BOXSIZE*8.5, BOXSIZE*1.5, 4, BLACK);
                  break;
          case 3: tft.fillRect(BOXSIZE*8, BOXSIZE, BOXSIZE, BOXSIZE, RED);
                  tft.fillCircle(BOXSIZE*8.5, BOXSIZE*1.5, 5, BLACK);
                  break;
          case 4: tft.fillRect(BOXSIZE*8, BOXSIZE, BOXSIZE, BOXSIZE, RED);
                  tft.fillCircle(BOXSIZE*8.5, BOXSIZE*1.5, 6, BLACK);
                  break;
          case 5: tft.fillRect(BOXSIZE*8, BOXSIZE, BOXSIZE, BOXSIZE, RED);
                  tft.fillCircle(BOXSIZE*8.5, BOXSIZE*1.5, 7, BLACK);
                  break;
          default: tft.fillRect(BOXSIZE*8, BOXSIZE, BOXSIZE, BOXSIZE, RED);
                   tft.fillCircle(BOXSIZE*8.5, BOXSIZE*1.5, 3, BLACK);
                   PENRADIUS = 1;
                   break;       
         }
         delay(150);
       } else if (p.x < BOXSIZE*10 && p.x > BOXSIZE*9) {
         tft.fillRect(0, BOXSIZE*2, tft.width(), tft.height()-BOXSIZE*2, currentcolor);
         tft.fillRect(BOXSIZE*9, BOXSIZE, BOXSIZE, BOXSIZE, RED);
         tft.drawLine(BOXSIZE*9, BOXSIZE, BOXSIZE*10-1, BOXSIZE*2-1, BLACK);
         tft.drawLine(BOXSIZE*9, BOXSIZE*2-1, BOXSIZE*10-1, BOXSIZE, BLACK);
         
       }

       if (oldcolor != currentcolor) {
          if (oldcolor == BLACK) tft.fillRect(0, 0, BOXSIZE, BOXSIZE, BLACK);
          if (oldcolor == NAVY) tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, NAVY);
          if (oldcolor == DARKGREEN) tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, DARKGREEN);
          if (oldcolor == DARKCYAN) tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, DARKCYAN);
          if (oldcolor == MAROON) tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, MAROON);
          if (oldcolor == PURPLE) tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, PURPLE);
          if (oldcolor == OLIVE) tft.fillRect(BOXSIZE*6, 0, BOXSIZE, BOXSIZE, OLIVE);
          if (oldcolor == LIGHTGREY) tft.fillRect(BOXSIZE*7, 0, BOXSIZE, BOXSIZE, LIGHTGREY);
          if (oldcolor == DARKGREY) tft.fillRect(BOXSIZE*8, 0, BOXSIZE, BOXSIZE, DARKGREY);
          if (oldcolor == BLUE) tft.fillRect(BOXSIZE*9, 0, BOXSIZE, BOXSIZE, BLUE);          
          if (oldcolor == GREEN) tft.fillRect(0, BOXSIZE, BOXSIZE, BOXSIZE, GREEN);
          if (oldcolor == CYAN) tft.fillRect(BOXSIZE, BOXSIZE, BOXSIZE, BOXSIZE, CYAN);
          if (oldcolor == RED) tft.fillRect(BOXSIZE*2, BOXSIZE, BOXSIZE, BOXSIZE, RED);
          if (oldcolor == MAGENTA) tft.fillRect(BOXSIZE*3, BOXSIZE, BOXSIZE, BOXSIZE, MAGENTA);
          if (oldcolor == YELLOW) tft.fillRect(BOXSIZE*4, BOXSIZE, BOXSIZE, BOXSIZE, YELLOW);
          if (oldcolor == WHITE) tft.fillRect(BOXSIZE*5, BOXSIZE, BOXSIZE, BOXSIZE, WHITE);
          if (oldcolor == ORANGE) tft.fillRect(BOXSIZE*6, BOXSIZE, BOXSIZE, BOXSIZE, ORANGE);
          if (oldcolor == GREENYELLOW) tft.fillRect(BOXSIZE*7, BOXSIZE, BOXSIZE, BOXSIZE, GREENYELLOW);        
       }
    }
    if (((p.y-PENRADIUS) > BOXSIZE*2) && ((p.y+PENRADIUS) < tft.height())) {
      tft.fillCircle(p.x, p.y, PENRADIUS, currentcolor);
    }
  }
}
