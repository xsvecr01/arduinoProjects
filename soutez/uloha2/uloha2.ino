byte seven_seg_digits[11][7] = { { 0,0,0,0,0,0,1 },  // = 0
                                 { 1,0,0,1,1,1,1 },  // = 1
                                 { 0,0,1,0,0,1,0 },  // = 2
                                 { 0,0,0,0,1,1,0 },  // = 3
                                 { 1,0,0,1,1,0,0 },  // = 4
                                 { 0,1,0,0,1,0,0 },  // = 5
                                 { 0,1,0,0,0,0,0 },  // = 6
                                 { 0,0,0,1,1,1,1 },  // = 7
                                 { 0,0,0,0,0,0,0 },  // = 8
                                 { 0,0,0,0,1,0,0 },  // = 9
                                 { 1,1,1,1,1,1,0 },  
                                };
                                
int btn1 = 10;

int state1 = 1;
int last1;
int i = 0;
int j = 0;
int k = 0;

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
  pinMode(btn1, INPUT);
  Serial.begin(9600);
  pinMode(2, OUTPUT);  //A
  pinMode(3, OUTPUT);  //B
  pinMode(4, OUTPUT);  //C
  pinMode(5, OUTPUT);  //D
  pinMode(6, OUTPUT);  //E
  pinMode(7, OUTPUT);  //F
  pinMode(8, OUTPUT);  //G  
  pinMode(9, OUTPUT);
  writeDot(1);
  sevenSegWrite(0);
}

void loop() {
  state1 = digitalRead(btn1);
while (state1 == HIGH)
{
 last1 = state1;
 delay(100);
 j++;
 state1 = digitalRead(btn1);
  if (j < 10)
  {       
    if (state1 != last1 && state1 == LOW)
    {   
      i++;
      j = 0;      
      if (i > 9)
      {
        i = 0;
      }
      delay(100);
      j++;
      sevenSegWrite(i);    
    }
  }
  if (j==10)
  {
    while(k<3)
    {
      i = 10;
      delay(100);
      sevenSegWrite(i);
      k++;
    }
    k = 0;
    i = 0;
    sevenSegWrite(i);
    j = 0;
  } 
}



         

}
