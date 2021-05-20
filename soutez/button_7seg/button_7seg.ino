byte seven_seg_digits[10][7] = { { 0,0,0,0,0,0,1 },  // = 0
                                 { 1,0,0,1,1,1,1 },  // = 1
                                 { 0,0,1,0,0,1,0 },  // = 2
                                 { 0,0,0,0,1,1,0 },  // = 3
                                 { 1,0,0,1,1,0,0 },  // = 4
                                 { 0,1,0,0,1,0,0 },  // = 5
                                 { 0,1,0,0,0,0,0 },  // = 6
                                 { 0,0,0,1,1,1,1 },  // = 7
                                 { 0,0,0,0,0,0,0 },  // = 8
                                 { 0,0,0,0,1,0,0 }   // = 9
                                };
                                
int btn1 = 10;
int btn2 = 12;

int state1 = 1;
int state2 = 1;
int last1;
int last2;
int i = 0;

void writeDot(byte dot) {
  digitalWrite(9, dot);
}

   
void sevenSegWrite(byte digit) {
  byte pin = 2;
  for (byte segCount = 0; segCount < 7; ++segCount) {
    digitalWrite(pin, seven_seg_digits[digit][segCount]);
    ++pin;
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  Serial.begin(9600);
  pinMode(2, OUTPUT);  //A
  pinMode(3, OUTPUT);  //B
  pinMode(4, OUTPUT);  //C
  pinMode(5, OUTPUT);  //D
  pinMode(6, OUTPUT);  //E
  pinMode(7, OUTPUT);  //F
  pinMode(8, OUTPUT);  //G  
  pinMode(9, OUTPUT);
  writeDot(1);  // start with the "dot" off
  sevenSegWrite(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  state1 = digitalRead(btn1);
  state2 = digitalRead(btn2);
  

  if (state1 != last1 && state1 == LOW)
  {
    if (i<9)
    {
      i++;
      sevenSegWrite(i);
      Serial.println(i);
    }
    delay(100);    
  }
  
  if (state2 != last2 && state2 == LOW)
  {
    if (i>0)
    {
      i--;
      sevenSegWrite(i);
      Serial.println(i);
    }
    delay(100);
  }

  last1 = state1;
  last2 = state2;  

}
