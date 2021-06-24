// hexapod_main file
/********* INCLUDE *********/
#include "Hexapod.h"



/********* VARIABLES *********/
Adafruit_PWMServoDriver pcaBoard = Adafruit_PWMServoDriver(0x40);

MyServo coxa0(S0, NULL, 0);
MyServo femur0(S1, NULL, 1);
MyServo tibia0(S2, &pcaBoard);

MyServo coxa1(S3, &pcaBoard);
MyServo femur1(S4, &pcaBoard);
MyServo tibia1(S5, &pcaBoard);

MyServo coxa2(S6, &pcaBoard);
MyServo femur2(S7, &pcaBoard);
MyServo tibia2(S8, &pcaBoard);

MyServo coxa3(S9, &pcaBoard);
MyServo femur3(S10, &pcaBoard);
MyServo tibia3(S11, &pcaBoard);

MyServo coxa4(S12, &pcaBoard);
MyServo femur4(S13, &pcaBoard);
MyServo tibia4(S14, &pcaBoard);

MyServo coxa5(S15, &pcaBoard);
MyServo femur5(S16, &pcaBoard);
MyServo tibia5(S17, &pcaBoard);

LegPosition posB = Back;
Leg RightB(&coxa0, &femur0, &tibia0, posB);
Leg LeftB(&coxa1, &femur1, &tibia1, posB, false);

LegPosition posM = Middle;
Leg RightM(&coxa2, &femur2, &tibia2, posM);
Leg LeftM(&coxa3, &femur3, &tibia3, posM, false);

LegPosition posF = Front;
Leg RightF(&coxa4, &femur4, &tibia4, posF);
Leg LeftF(&coxa5, &femur5, &tibia5, posF, false);

Hexapod Tronik(&RightF, &RightM, &RightB, &LeftF, &LeftM, &LeftB);


MyServo* allServos[18];
Leg* allLegs[6];

pthread_t thr_update;

char ptrTaskList[250];

/********* FUNCTIONS *********/

void updateServos(void* d)
{
    delay(1000);
    uint16_t count = 0;
    unsigned long currM, postM = 0;
    Serial.println("updateServos running on core: ");
    Serial.println(xPortGetCoreID());
    while(1)
    {
        currM = millis();
        //currM = millis();
        /*for(int i = 0; i < 18; i++)
        {
            allServos[i]->Refresh();
        }*/
        for (MyServo *servo : allServos)
        {
            servo->Refresh(currM);
        }
        //postM = millis();

        /*Serial.print(count);
        Serial.print(" start:");
        Serial.print(currM);
        Serial.print(", end:");
        Serial.print(postM);
        Serial.print(", diff:");
        Serial.println(postM - currM);*/
        count++;
        if(count > 500)
        {
            count = 1;
            delay(1);
        }
    }
}

void initServos()
{
    allServos[0] = &coxa0;
    allServos[6] = &femur0;
    allServos[12] = &tibia0;
    tibia0.SetDefault(tibia0.GetMid() - 3);

    allServos[1] = &coxa1;
    coxa1.SetDefault(coxa1.GetMid() + 3);
    allServos[7] = &femur1;
    allServos[13] = &tibia1;
    tibia1.SetDefault(tibia1.GetMid() + 3);

    allServos[2] = &coxa2;
    allServos[8] = &femur2;
    femur2.SetDefault(femur2.GetMid() + 15);
    allServos[14] = &tibia2;
    tibia2.SetDefault(tibia1.GetMid() + 3);

    allServos[3] = &coxa3;
    coxa3.SetDefault(coxa3.GetMid() + 10);
    allServos[9] = &femur3;
    femur3.SetDefault(femur3.GetMid() + 8);
    allServos[15] = &tibia3;
    tibia3.SetDefault(tibia3.GetMid() - 3);


    allServos[4] = &coxa4;
    coxa4.SetDefault(coxa4.GetMid() - 10);
    allServos[10] = &femur4;
    femur4.SetDefault(femur4.GetMid() - 5);
    allServos[16] = &tibia4;
    tibia4.SetDefault(tibia4.GetMid() - 3);

    allServos[5] = &coxa5;
    allServos[11] = &femur5;
    allServos[17] = &tibia5;

    allLegs[0] = &RightB;
    allLegs[1] = &LeftB;
    allLegs[2] = &RightM;
    allLegs[3] = &LeftM;
    allLegs[4] = &RightF;
    allLegs[5] = &LeftF;

    for (int i = 0; i < 6; i++)
    {
        allLegs[i]->FoldFast();
        delay(500);
    }
}


int angle = 0;

TaskHandle_t TaskRefresh;

//TaskHandle_t TaskLoop;

/********* SETUP *********/
void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);

    pcaBoard.begin();
    pcaBoard.setPWMFreq(FREQ);

    initServos();
    delay(500);

    Serial.println("--------");
    while(Serial.available() == 0){
    }
    xTaskCreatePinnedToCore(updateServos, "TaskRefresh", 4096, NULL, 2, &TaskRefresh, 0);
    //xTaskCreatePinnedToCore(loop1, "TaskLoop", 8192, NULL, 0, &TaskLoop, 0);

    //int res = pthread_create(&thr_update, NULL, updateServos, NULL);

    delay(2000);
    Tronik.Fold(false);

    //Tronik.Prep33();

    Tronik.Prep42(0);
}



/********* MAIN *********/
int xd = 0;
void loop() {
    if(!xd)
    {
        Serial.println("loop running on core: ");
        Serial.println(xPortGetCoreID());
        xd = 1;
    }
    if(Tronik.Finished())
    {
        Tronik.Step42(0);
    }
    delay(1);

}
