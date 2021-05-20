
int b1 = 2;
int b2 = 3;
int b3 = 4;

int l1 = 9;
int l2 = 10;
int l3 = 11;

//zvolene cislo
int selected = 0;

//pocet opakovani
int count;

//vyherni cislo
int win = 0;

int dimmed = 50;

void setup() {
  Serial.begin(9600);

  //buttons
  pinMode(b1, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);
  pinMode(b3, INPUT_PULLUP);

  //leds
  pinMode(l1, OUTPUT);
  pinMode(l2, OUTPUT);
  pinMode(l3, OUTPUT);
    
}

void loop() {

  analogWrite(l1, 0);
  analogWrite(l2, 0);
  analogWrite(l3, 0);
  
  if (digitalRead(b1) == LOW) {
    selected = 1;
    analogWrite(l1, dimmed);
    analogWrite(l2, 0);
    analogWrite(l3, 0);
    delay(100);
      //Button one is pressed!
 }

  if (digitalRead(b2) == LOW) {
    selected = 2;
    analogWrite(l2, dimmed);
    analogWrite(l1, 0);
    analogWrite(l3, 0);
    delay(100);
      //Button two is pressed!
 }

  if (digitalRead(b3) == LOW) {
    selected = 3;
    analogWrite(l3, dimmed);
    analogWrite(l1, 0);
    analogWrite(l2, 0);
    delay(100);
      //Button two is pressed!
 }

  if(selected != 0)
  {
    count = random(10, 50);
    for(int i = 0; i < count; i++)
    {
      win = random(1, 4);

      switch (win)
      {
        case 1:
          analogWrite(l1, 200);
          analogWrite(l2, 0);
          analogWrite(l3, 0);
          break;
        case 2:
          analogWrite(l2, 200);
          analogWrite(l1, 0);
          analogWrite(l3, 0);
          break;
        case 3:
          analogWrite(l3, 200);
          analogWrite(l1, 0);
          analogWrite(l2, 0);
          break;
      }
      delay(50);
      if(i == count-1)
      {
        delay(500);
      }
    }
       
  
   if (win == selected)
   {
    analogWrite(l1, 255);
    analogWrite(l2, 255);
    analogWrite(l3, 255);
   }

   selected = 0;
   delay(1500);
  }

}
