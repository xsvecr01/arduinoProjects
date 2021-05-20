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

void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);  //A
  pinMode(3, OUTPUT);  //B
  pinMode(4, OUTPUT);  //C
  pinMode(5, OUTPUT);  //D
  pinMode(6, OUTPUT);  //E
  pinMode(7, OUTPUT);  //F
  pinMode(8, OUTPUT);  //G  
  pinMode(9, OUTPUT);
  writeDot(1);  // start with the "dot" off
}

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

void loop() {
  sevenSegWrite(1);

}
