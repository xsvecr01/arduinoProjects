//#include <Adafruit_GFX.h>
//#include <gfxfont.h>

//#include <Adafruit_TFTLCD.h>

#include <pin_magic.h>
#include <registers.h>
#include <TouchScreen.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

#define LCD_CS A3 //
#define LCD_CD A2 //
#define LCD_WR A1 //
#define LCD_RD A0 //
#define LCD_RESET A4 //

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define MINPRESSURE 2
#define MAXPRESSURE 1000

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define GROUND 180

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);


bool jump = false;
bool go_down = false;
int len = 0;
int gameSpeed = 1;

struct CollisionObject{
  int x = -100;
  int y = 0;
  int size_x = 0;
  int size_y = 0;
  uint16_t color;
};

CollisionObject player;

CollisionObject obstSmall;
CollisionObject obstBig;
CollisionObject obstFlying;

CollisionObject obstSmall1;
CollisionObject obstBig1;
CollisionObject obstFlying1;

void setup(void) {
  Serial.begin(9600);
  tft.reset();

  uint16_t identifier = tft.readID();
  tft.begin(identifier);

  tft.setRotation(3);

  tft.fillScreen(WHITE); //

  for(int i = 0; i < 320; i++){
    tft.drawPixel(i, random(GROUND, GROUND+2), BLACK);
  }

  player.x = 40;
  player.size_x = 20;
  player.size_y = 30;
  player.y = GROUND - player.size_y;
  player.color = BLUE;

  obstSmall.size_x = 15;
  obstSmall.size_y = 20;
  obstSmall.color = BLACK;
  obstSmall.y = GROUND - obstSmall.size_y;

  obstBig.size_x = 10;
  obstBig.size_y = 30;
  obstBig.color = BLACK;
  obstBig.y = GROUND - obstBig.size_y;

  obstFlying.size_x = 30;
  obstFlying.size_y = 10;
  obstFlying.color = BLACK;

  obstSmall1 = obstSmall;
  obstBig1 = obstBig;
  obstFlying1 = obstFlying;
}

/**
 * Detection of collisions between two rectangular objects
 */
bool detectCollision(CollisionObject obj1, CollisionObject obj2){
  if(
    (((obj1.y >= obj2.y) && (obj1.y <= obj2.y + obj2.size_y)) ||
    ((obj1.y + obj1.size_y >= obj2.y) && (obj1.y + obj1.size_y <= obj2.y + obj2.size_y))) &&
    (((obj1.x <= obj2.x) && (obj1.x >= obj2.x - obj2.size_x)) ||
    ((obj1.x - obj1.size_x <= obj2.x) && (obj1.x - obj1.size_x >= obj2.x - obj2.size_x))))
    {
      return true;
    }
  else return false;
}

void redraw(CollisionObject obj, int gSpeed){
  tft.fillRect(obj.x, obj.y, obj.size_x, obj.size_y, obj.color);
  tft.fillRect(obj.x + obj.size_x, obj.y, gSpeed, obj.size_y, WHITE);
}

unsigned long cnt = 0;
unsigned long prgcnt = 0;
int r = 0;
unsigned long dist = 170;
int jump_dist = 0;
/**
 * Main
 */
void loop(void) {
  if(cnt > 160 && cnt%dist < 10)
  {
    dist += random(160, 240);
    if(r == 0){
      if(obstBig.x <= -obstBig.size_x){
        r = random(0, 3);
        obstBig.x = 320 + obstBig.size_x;
      }
      else if(obstBig1.x <= -obstBig.size_x){
        r = random(0, 3);
        obstBig1.x = 320 + obstBig1.size_x;
      }
    }
    else if(r == 1){
      if(obstSmall.x <= -obstSmall.size_x){
        r = random(0, 3);
        obstSmall.x = 320 + obstSmall.size_x;
      }
      else if(obstSmall1.x <= -obstSmall1.size_x){
        r = random(0, 3);
        obstSmall1.x = 320 + obstSmall1.size_x;
      }
    }
    else if(r == 2){
      if(obstFlying.x <= -obstFlying.size_x){
        r = random(0, 3);
        if(r == 1) obstFlying.y = GROUND - obstFlying.size_y - 20;
        else if(r == 0) obstFlying.y = GROUND - obstFlying.size_y - 50;
        else if(r == 2) obstFlying.y = GROUND - obstFlying.size_y - 10;
        obstFlying.x = 320 + obstFlying.size_x;
      }
      else if(obstFlying1.x <= -obstFlying1.size_x){
        r = random(0, 3);
        if(r == 1) obstFlying1.y = GROUND - obstFlying.size_y - 32;
        else if(r == 0) obstFlying1.y = GROUND - obstFlying.size_y - 50;
        else if(r == 2) obstFlying1.y = GROUND - obstFlying.size_y - 10;
        obstFlying1.x = 320 + obstFlying1.size_x;
      }
    }
  }
  
  
  
  obstBig.x -= gameSpeed;
  redraw(obstBig, gameSpeed);
  
  obstSmall.x -= gameSpeed;
  redraw(obstSmall, gameSpeed);

  obstFlying.x -= gameSpeed;
  redraw(obstFlying, gameSpeed);

  obstBig1.x -= gameSpeed;
  redraw(obstBig1, gameSpeed);
  
  obstSmall1.x -= gameSpeed;
  redraw(obstSmall1, gameSpeed);

  obstFlying1.x -= gameSpeed;
  redraw(obstFlying1, gameSpeed);

  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if(prgcnt%5000 != 0){
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
      if(!jump) jump = true;
      jump_dist = 0;
    }
  }
  if(jump){
    if(len >= 60) {
      go_down = true;
    }
    if(len <= 60 && !go_down){
      len+= gameSpeed;
      player.y = GROUND - len - player.size_y;
      tft.fillRect(player.x, player.y + player.size_y, player.size_x, gameSpeed, WHITE);
      jump_dist = cnt;
    }
    else if(cnt - jump_dist > 15){
      player.y = GROUND - len - player.size_y;
      tft.fillRect(player.x, player.y - gameSpeed, player.size_x, gameSpeed, WHITE);
      len-= gameSpeed;
      if(len == 0){
        jump = false;
        go_down = false;
        player.y = GROUND - len - player.size_y;
        tft.fillRect(player.x, player.y - gameSpeed, player.size_x, gameSpeed, WHITE);
      }
    }
  }

  tft.fillRect(player.x, player.y, player.size_x, player.size_y, player.color);

  if(detectCollision(obstSmall, player) || detectCollision(obstBig, player) || detectCollision(obstFlying, player) ||
    detectCollision(obstSmall1, player) || detectCollision(obstBig1, player) || detectCollision(obstFlying1, player)){
    player.color = RED;
  }
  else player.color = BLUE;

  if(prgcnt%10 == 0){
    tft.fillRect(10, 10, 100, 24, WHITE);
    tft.setCursor(10, 10);
    tft.setTextColor(BLACK);
    tft.setTextSize(3);
    tft.print(cnt);
  }
  
  cnt += gameSpeed;
  prgcnt++;
  if(prgcnt%5000 == 0 && gameSpeed < 4){
    gameSpeed++;
  }
}
