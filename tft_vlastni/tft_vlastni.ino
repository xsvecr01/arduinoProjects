//#include <Adafruit_GFX.h>
//#include <gfxfont.h>

//#include <Adafruit_TFTLCD.h>
//#include <pin_magic.h>
//#include <registers.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

#define LCD_CS A3 //
#define LCD_CD A2 //
#define LCD_WR A1 //
#define LCD_RD A0 //
#define LCD_RESET A4 //

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

void setup(void) {
  Serial.begin(9600);
  tft.reset();

  uint16_t identifier = tft.readID();

  if (identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if (identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if (identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if (identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    return;
  }

  tft.begin(identifier);



  tft.setRotation(2);

  tft.fillScreen(BLACK); //
  //tft.setTextColor(WHITE);
  //tft.setCursor(5, 5);
  //tft.setTextSize(5);
  //tft.print("Hello World");

}

void loop(void) {

  //telo
  tft.drawLine(120, 140, 120, 240, YELLOW);
  //ruka prava
  tft.drawLine(120, 160, 160, 240, YELLOW);
  //noha leva
  tft.drawLine(120, 240, 60, 310, YELLOW);
  //noha prava
  tft.drawLine(120, 240, 180, 310, YELLOW);
  //hlava
  tft.drawCircle(120, 111, 29, YELLOW);
  
  tft.drawLine(120, 160, 60, 80, YELLOW);
  delay(50);
  tft.drawLine(120, 160, 60, 80, BLACK);

  tft.drawLine(120, 160, 50, 90, YELLOW);
  delay(50);
  tft.drawLine(120, 160, 50, 90, BLACK);
  
  tft.drawLine(120, 160, 40, 100, YELLOW);
  delay(50);
  tft.drawLine(120, 160, 40, 100, BLACK);

  tft.drawLine(120, 160, 30, 110, YELLOW);
  delay(50);
  tft.drawLine(120, 160, 30, 110, BLACK);
  
  //===========
  tft.drawLine(120, 160, 20, 140, YELLOW);
  delay(50);
  tft.drawLine(120, 160, 20, 140, BLACK);
  //===========
  
  tft.drawLine(120, 160, 30, 110, YELLOW);
  delay(50);
  tft.drawLine(120, 160, 30, 110, BLACK);

  tft.drawLine(120, 160, 40, 100, YELLOW);
  delay(50);
  tft.drawLine(120, 160, 40, 100, BLACK);

  tft.drawLine(120, 160, 50, 90, YELLOW);
  delay(50);
  tft.drawLine(120, 160, 50, 90, BLACK);


}
