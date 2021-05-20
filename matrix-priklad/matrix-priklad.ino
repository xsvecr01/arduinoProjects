byte sloupce[] = {13,3,4,10,6,11,15,16};
byte radky[] = {9,14,8,12,1,7,2,5};

//dvojrozměrné pole pro obrázek
byte obrazek[8][8] = {{1,1,1,1,1,1,1,1},
                      {1,0,0,0,0,0,0,1},
                      {1,0,1,1,1,1,0,1},
                      {1,0,1,0,0,1,0,1},
                      {1,0,1,0,0,1,0,1},
                      {1,0,1,1,1,1,0,1},
                      {1,0,0,0,0,0,0,1},
                      {1,1,1,1,1,1,1,1}};                 
                      

void setup(){
    for(int i = 0; i < 8; i++){
        //nastavíme piny
        pinMode(sloupce[i], OUTPUT);
        pinMode(radky[i], OUTPUT);
        
        //zajistíme vypnutí displeje
        digitalWrite(sloupce[i], HIGH); 
        digitalWrite(radky[i], LOW);
    }
}

void loop(){
    for(int i = 0; i < 8; i++){
        //zapneme řádek i
        digitalWrite(radky[i], HIGH);
        
        //dále pracujeme s jednotlivými sloupci
        for(int j = 0; j < 8; j++){
            //pokud je ve vybraném políčku 1, rozsvítí se LED
            if(obrazek[i][j] == 1){
                digitalWrite(sloupce[j], LOW);
            }
        }
        delay(1); //chvíli počkáme, aby byl obraz dostatečně jasný
        
        //vypneme všechny sloupce
        for(int j = 0; j < 8; j++){
            digitalWrite(sloupce[j], HIGH);
        }
        
        //vypneme řádek i
        digitalWrite(radky[i], LOW);
    }
}



