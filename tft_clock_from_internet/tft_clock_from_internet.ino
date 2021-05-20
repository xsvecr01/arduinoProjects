#include <Adafruit_TFTLCD.h> //Library

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

// Agar warna mudah dimengerti (Human Readable color):
#define TFT_BLACK   0x0000
#define TFT_BLUE    0x001F
#define TFT_RED     0xF800
#define TFT_GREEN   0x07E0
#define TFT_CYAN    0x07FF
#define TFT_MAGENTA 0xF81F
#define TFT_YELLOW  0xFFE0
#define TFT_WHITE   0xFFFF
#define TFT_GREY    0x5AEB

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

float sx = 0, sy = 1, mx = 1, my = 0, hx = -1, hy = 0;    // Saved H, M, S x & y multipliers
float sdeg = 0, mdeg = 0, hdeg = 0;
uint16_t osx = 120, osy = 120, omx = 120, omy = 120, ohx = 120, ohy = 120; // Saved H, M, S x & y coords
int16_t x0 = 0, x1 = 0, yy0 = 0, yy1 = 0;
uint32_t targetTime = 0;                    // for next 1 second timeout

uint16_t xpos; // x posisi jam
uint8_t conv2d(const char* p) {
uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
   v = *p - '0';
  return 10 * v + *++p - '0';
}

//Mengambil waktu jam dari waktu Compile/Upload di system Windows
uint8_t hh = conv2d(__TIME__), mm = conv2d(__TIME__ + 3), ss = conv2d(__TIME__ + 6); // mengambil data waktu dari jam Compile-Upload
boolean initial = 1;

void setup(void) {
  tft.reset();        //perlu saat menggunakan lib.SPFD5408_Adafruit_TFTLCD.h
  tft.begin(0x9341);  //perlu address ini saat menggunakan lib.SPFD5408_Adafruit_TFTLCD.h
  tft.setRotation(1); //Lanscape Mode

  tft.setTextColor(TFT_WHITE);  // warna font
  tft.fillScreen(TFT_BLACK);    //warna latar
  
  // Draw clock face
  xpos = tft.width() / 2; // mencari titik koordinat tengah LCD
  tft.fillCircle(xpos, 120, 118, TFT_GREEN); //warna lingkaran luar
  tft.fillCircle(xpos, 120, 110, TFT_BLACK); //warna jam bagian dalam

  // Draw 12 lines
  for (int i = 0; i < 360; i += 30) {
    sx = cos((i - 90) * 0.0174532925);
    sy = sin((i - 90) * 0.0174532925);
    x0 = sx * 114 + xpos;
    yy0 = sy * 114 + 120;
    x1 = sx * 100 + xpos;
    yy1 = sy * 100 + 120;

    tft.drawLine(x0, yy0, x1, yy1, TFT_YELLOW);//garis penanda angka jam
  }

  // Draw 60 dots
  for (int i = 0; i < 360; i += 6) {
    sx = cos((i - 90) * 0.0174532925);
    sy = sin((i - 90) * 0.0174532925);
    x0 = sx * 102 + xpos;
    yy0 = sy * 102 + 120;
    // Draw minute markers
    tft.drawPixel(x0, yy0, TFT_WHITE); //titik penanda menit

    // Draw main quadrant dots
    if (i == 0 || i == 180) tft.fillCircle(x0, yy0, 2, TFT_WHITE); //penanda 12 dan 6
    if (i == 90 || i == 270) tft.fillCircle(x0, yy0, 2, TFT_WHITE); //penanda 3 dan 9
  }

  tft.fillCircle(xpos, 121, 3, TFT_WHITE);
  targetTime = millis() + 1000;
}

void loop() {
  if (targetTime < millis()) {
    targetTime = millis() + 1000;
    ss++;              // Advance second
    if (ss == 60) {
      ss = 0;
      mm++;            // Advance minute
      if (mm > 59) {
        mm = 0;
        hh++;          // Advance hour
        if (hh > 23) {
          hh = 0;
        }
      }
    }

    // Pre-compute hand degrees, x & y coords for a fast screen update
    sdeg = ss * 6;                     // 0-59 -> 0-354
    mdeg = mm * 6 + sdeg * 0.01666667; // 0-59 -> 0-360 - includes seconds, but these increments are not used
    hdeg = hh * 30 + mdeg * 0.0833333; // 0-11 -> 0-360 - includes minutes and seconds, but these increments are not used
    hx = cos((hdeg - 90) * 0.0174532925);
    hy = sin((hdeg - 90) * 0.0174532925);
    mx = cos((mdeg - 90) * 0.0174532925);
    my = sin((mdeg - 90) * 0.0174532925);
    sx = cos((sdeg - 90) * 0.0174532925);
    sy = sin((sdeg - 90) * 0.0174532925);

    if (ss == 0 || initial) {
      initial = 0;
      // Erase hour and minute hand positions every minute
      tft.drawLine(ohx, ohy, xpos, 121, TFT_BLACK);
      ohx = hx * 62 + xpos + 1;
      ohy = hy * 62 + 121;
      tft.drawLine(omx, omy, xpos, 121, TFT_BLACK);
      omx = mx * 84 + xpos;
      omy = my * 84 + 121;
    }

    // Redraw new hand positions, hour and minute hands not erased here to avoid flicker
    tft.drawLine(osx, osy, xpos, 121, TFT_BLACK);
    osx = sx * 90 + xpos + 1;
    osy = sy * 90 + 121;
    tft.drawLine(osx, osy, xpos, 121, TFT_RED);
    tft.drawLine(ohx, ohy, xpos, 121, TFT_WHITE);
    tft.drawLine(omx, omy, xpos, 121, TFT_WHITE);
    tft.drawLine(osx, osy, xpos, 121, TFT_RED);

    tft.fillCircle(xpos, 121, 3, TFT_RED);

    // Draw text at position xpos,260 using fonts 4
  // Only font numbers 2,4,6,7 are valid. Font 6 only contains characters [space] 0 1 2 3 4 5 6 7 8 9 : . a p m
  // Font 7 is a 7 segment font and only contains characters [space] 0 1 2 3 4 5 6 7 8 9 : .
  tft.fillRect(xpos-5, 200,12,10,TFT_BLACK);
  tft.setCursor(xpos-5, 200);
  tft.setTextSize(1);
  tft.print(ss);
  }
}
