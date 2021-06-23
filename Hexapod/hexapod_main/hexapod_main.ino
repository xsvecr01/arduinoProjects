// hexapod_main file
/********* INCLUDE *********/
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <cQueue.h>
#include <pthread.h>

/********* CONSTANTS *********/
#define S0  5   //coxa
#define S1  18  //femur
#define S2  0   //tibia

#define S3  1   //coxa
#define S4  2   //femur
#define S5  3   //tibia

#define S6  4   //coxa
#define S7  5   //femur
#define S8  6   //tibia

#define S9  7   //coxa
#define S10 8   //femur
#define S11 9   //tibia

#define S12 10  //coxa
#define S13 11  //femur
#define S14 12  //tibia

#define S15 13  //coxa
#define S16 14  //femur
#define S17 15  //tibia

#define FREQ 50  //pwm frequency
#define RESOLUTION 12  //pwm resolution

#define TEN_EIGHTY 0.05555555555555555555555556  // 10/180


#define COXA_LEN 12.5
#define FEMUR_LEN 45
#define TIBIA_LEN 57

#define SERVO_STEP 2

Adafruit_PWMServoDriver pcaBoard = Adafruit_PWMServoDriver(0x40);


enum LegPosition {Front, Middle, Back};
enum State {Sitting, Standing, Walking};

float RotateX(float x, float angle);
float RotateY(float x, float y, float angle);
unsigned long millis1();


/********* CLASSES *********/

class MyServo
{
    public:
        Queue_t commandQ;
        unsigned long _diffMillis;

        MyServo(uint8_t pin, bool pca = 1, uint8_t channel = 0)
        {
            if (!pca)
            {
                ledcSetup(channel, FREQ, RESOLUTION);
                ledcAttachPin(pin, channel);
            }
            _pin = pin;
            _pca = pca;
            _channel = channel;
            _startMillis = millis();

            _prevAngle = _angle = _default = _mid = 90;

            q_init(&commandQ, sizeof(Command), 30, FIFO, false);
        }

        uint8_t GetMid()
        {
            return _mid;
        }

        uint8_t GetDefault()
        {
            return _default;
        }

        void SetMid(uint8_t value)
        {
            _mid =  value;
        }

        void SetDefault(uint8_t value)
        {
            _angle = value;
            _default = value;
            _mid = value;
            _prevAngle = value;
        }

        void Sleep(uint16_t duration)
        {
            uint8_t tmp_angle = _desired;
            Command *tmp;
            if(q_peekPrevious(&commandQ, &tmp))
            {
                tmp_angle = tmp->angle;
            }
            SetPos(tmp_angle, duration);
        }


        void SetPos(uint8_t angle, float duration)
        {
            uint8_t tmp_angle = _desired;

            Command *cmd = new (Command){angle, duration};
            q_push(&commandQ, &cmd);
        }

        void SetPosFast(uint8_t angle)
        {
            _SetPos(angle);
            _angle = angle;
            _prevAngle = angle;
            _desired = angle;
        }

        void Refresh()
        {
            _currentMillis = millis();
            if(_currentMillis - _startMillis >= _duration)
            //if(currM - _startMillis >= _duration)
            {
                _startMillis = millis();
                //_startMillis = currM;
                _angle = _desired;
                _prevAngle = _angle;
                if(!q_isEmpty(&commandQ))
                {
                    _PopQueue();
                }
            }
            else if(_desired != _angle);
            {
                int16_t y = (int16_t) (_desired - _prevAngle) * ((_currentMillis - _startMillis) / _duration);
                _angle = _prevAngle + y;
            }
            _SetPos(_angle);
        }

        uint8_t GetPos()
        {
            return _angle;
        }

    private:
        uint8_t _pin, _channel, _mid, _default, _angle, _prevAngle, _desired;
        float _duration = 400;
        unsigned long _currentMillis, _startMillis;
        bool _pca;
        

        struct Command
        {
            uint8_t angle;
            float duration;
        };

        
        float _GetDuty(uint8_t angle)
        {
            float duty = (TEN_EIGHTY * angle + 2);
            return ((4095 * duty) / 100);
        }

        void _SetPos(uint8_t angle)
        {
            if (!_pca)
            {
                ledcWrite(_channel, _GetDuty(angle));
            }
            else
            {
                pcaBoard.setPWM(_pin, 0, _GetDuty(angle));
            }
            _angle = angle;
        }     

        void _PopQueue()
        {
            Command* cmd;
            q_pop(&commandQ, &cmd);
            _desired = cmd->angle;
            _duration = cmd->duration;

            delete cmd;
        }
};



class Leg
{
    public:
        bool _right;
        LegPosition _legPos;
                
        Leg(MyServo *coxa, MyServo *femur, MyServo *tibia, LegPosition legPos, bool right = true)
        {
            _Coxa = coxa;
            _Femur = femur;
            _Tibia = tibia;
            _right = right;
            _legPos = legPos;
        }

        void SetXYZ(float x, float y, float z, float z_off, uint16_t duration)
        {
            z_off += 18;
            float z_dif = z_off - z;            
            float gamma = atan(x/y);
            float L1 = sqrt(x * x + y * y);
            float L = sqrt((z_dif * z_dif) + pow(L1 - COXA_LEN, 2));
            float alpha = acos(z_dif / L) + acos(((TIBIA_LEN * TIBIA_LEN) - (FEMUR_LEN * FEMUR_LEN) - (L * L)) / (-2 * FEMUR_LEN * L));
            float beta = acos(((L * L) - (TIBIA_LEN * TIBIA_LEN) - (FEMUR_LEN * FEMUR_LEN)) / (-2 * TIBIA_LEN * FEMUR_LEN));


            gamma = (int)(gamma * 180 / PI);
            alpha = (int)(alpha * 180 / PI);
            beta = (int)(beta * 180 / PI);
            
            if(_right)
            {
                _Coxa->SetPos(_Coxa->GetMid() - gamma, duration);
                _Femur->SetPos(_Femur->GetMid() - (alpha - 90), duration);
                _Tibia->SetPos(_Tibia->GetMid() + (beta - 90), duration);
            }
            else
            {
                _Coxa->SetPos(_Coxa->GetMid() + gamma, duration);
                _Femur->SetPos(_Femur->GetMid() + (alpha - 90), duration);
                _Tibia->SetPos(_Tibia->GetMid() - (beta - 90), duration);
            }
            
        }

        void Sleep(uint16_t duration)
        {
            _Coxa->Sleep(duration);
            _Femur->Sleep(duration);
            _Tibia->Sleep(duration);
        }

        void Middle(uint16_t duration)
        {
            _Coxa->SetPos(_Coxa->GetMid(), duration);
            _Femur->SetPos(_Femur->GetMid(), duration);
            _Tibia->SetPos(_Tibia->GetMid(), duration);
        }

        void FoldFast()
        {
            _Coxa->SetPosFast(_Coxa->GetDefault());
            if (_right)
            {
                _Femur->SetPosFast(_Femur->GetDefault() - 80);
                _Tibia->SetPosFast(_Tibia->GetDefault() - 80);
            }
            else
            {
                _Femur->SetPosFast(_Femur->GetDefault() + 80);
                _Tibia->SetPosFast(_Tibia->GetDefault() + 80);
            }
        }

        void Fold(uint16_t duration)
        {
            _Coxa->SetPos(_Coxa->GetDefault(), duration);
            if (_right)
            {
                _Femur->SetPos(_Femur->GetDefault() - 80, duration);
                _Tibia->SetPos(_Tibia->GetDefault() - 80, duration);
            }
            else
            {
                _Femur->SetPos(_Femur->GetDefault() + 80, duration);
                _Tibia->SetPos(_Tibia->GetDefault() + 80, duration);
            }
        }

        bool Finished()
        {
            if(q_getCount(&_Coxa->commandQ) <= 8 && q_getCount(&_Femur->commandQ) <= 8 && q_getCount(&_Tibia->commandQ) <= 8)
            {
                return true;
            }
            else
            {
                return false;
            }
                
        }

    private:
        MyServo *_Coxa;
        MyServo *_Femur;
        MyServo *_Tibia;
};

class Hexapod
{
    public:
        Leg *RF, *RM, *RB, *LF, *LM, *LB;
        Leg* Legs[6];
        int Speed = 0;
        int Angle = 0;
        int Height = 1;
        bool Folded = true;
        bool Strafe = true;
        State state;

        Hexapod(Leg *RightFront, Leg *RightMiddle, Leg *RightBack, Leg *LeftFront, Leg *LeftMiddle, Leg *LeftBack)
        {
            RF = RightFront;
            RM = RightMiddle;
            RB = RightBack;
            LF = LeftFront;
            LM = LeftMiddle;
            LB = LeftBack;
            Legs[0] = RF;
            Legs[1] = RM;
            Legs[2] = RB;
            Legs[3] = LF;
            Legs[4] = LM;
            Legs[5] = LB;
            state = Sitting;
        }

        bool Finished()
        {
            return RF->Finished() && RM->Finished() && RB->Finished() && LF->Finished() && LM->Finished() && LB->Finished();
        }

        void SetHeight()
        {
            for(Leg *l : Legs)
            {
                l->SetXYZ(0, 60, 1, _height, _duration);
            }
        }

        void Fold(boolean fold)
        {
            if(fold)
            {
                for(Leg *l : Legs)
                {
                    l->SetXYZ(0, 60, _height + 10, _height, _duration/2);
                    l->Fold(_duration/2);
                }
            }
            else
            {
                for(Leg *l : Legs)
                {
                    l->SetXYZ(0, 60, _height + 10, _height, _duration/2);
                    l->SetXYZ(0, 60, 1, _height, _duration/2);
                }
            }
        }

        void Prep33()
        {
            RF->Sleep(_duration/2);
            RB->Sleep(_duration/2);
            LM->Sleep(_duration/2);
        }

        void Wait33()
        {
            LF->Sleep(_duration/4);
            LB->Sleep(_duration/4);
            RM->Sleep(_duration/4);
        }

        void Step33(float angle)
        {
            float x0 = 0;
            float x1 = 20;
            float x1_ = x1;
            float y = 60;
            float z2 = _height - 10;
            float angle_ = angle;

            
            if(z2 < 20)
            {
                z2 = 20;
            }
            float z1 = z2 / 2;
            float z0 = 1;
            
            for(Leg *l : Legs)
            {
                angle = angle_;
                x1 = x1_;
                AngleX1(l, &angle, &x1);
                                
                l->SetXYZ(RotateX(-x1, angle), RotateY(-x1, y, angle), z0, _height, _duration/4);
                l->SetXYZ(RotateX(-x1, angle), RotateY(-x1, y, angle), z1, _height, _duration/8);
                l->SetXYZ(RotateX(x0, angle),  RotateY(x0, y, angle),  z2, _height, _duration/8);
                l->SetXYZ(RotateX(x1, angle),  RotateY(x1, y, angle),  z1, _height, _duration/8);
                l->SetXYZ(RotateX(x1, angle),  RotateY(x1, y, angle),  z0, _height, _duration/8);
                l->SetXYZ(RotateX(x0, angle),  RotateY(x0, y, angle),  z0, _height, _duration/4);
            }
        }

        void Prep42(float angle)
        {
            float x0 = 0;
            float x1 = 20;
            float x1_ = x1;
            float y = 60;
            float z2 = _height - 10;
            float angle_ = angle;
            
            if(z2 < 20)
            {
                z2 = 20;
            }
            float z1 = z2 / 2;
            float z0 = 1;
            
            for(Leg *l : Legs)
            {
                angle = angle_;
                x1 = x1_;
                AngleX1(l, &angle, &x1);
                
                if(l->_legPos == Middle)
                    l->SetXYZ(RotateX(-x1, angle), RotateY(-x1, y, angle), z0, _height, _duration/2);

                if((l->_legPos == Front && !l->_right) || (l->_legPos == Back && l->_right))
                    l->SetXYZ(RotateX(-x1, angle), RotateY(-x1, y, angle), z0, _height, _duration);                
            }
        }

        void Step42(float angle)
        {
            float x0 = 0;
            float x1 = 20;
            float x1_ = x1;
            float y = 60;
            float z2 = _height - 10;
            float angle_ = angle;
            
            if(z2 < 20)
            {
                z2 = 20;
            }
            float z1 = z2 / 2;
            float z0 = 1;
            
            for(Leg *l : Legs)
            {
                angle = angle_;
                x1 = x1_;
                AngleX1(l, &angle, &x1);

                l->SetXYZ(RotateX(-x1, angle), RotateY(-x1, y, angle), z1, _height, _duration/8);
                l->SetXYZ(RotateX(x0, angle),  RotateY(x0, y, angle),  z2, _height, _duration/8);
                l->SetXYZ(RotateX(x1, angle),  RotateY(x1, y, angle),  z1, _height, _duration/8);
                l->SetXYZ(RotateX(x1, angle),  RotateY(x1, y, angle),  z0, _height, _duration/8);
                l->SetXYZ(RotateX(-x1, angle), RotateY(-x1, y, angle), z0, _height, _duration);
            }
        }

        void Prep51()
        {
            RF->Sleep(_duration/2);
            RB->Sleep(_duration/2);
            LM->Sleep(_duration/2);
        }

        void Wait51()
        {
            LF->Sleep(_duration/2);
            LB->Sleep(_duration/2);
            RM->Sleep(_duration/2);
        }

        void Step51(float angle)
        {
            float x0 = 0;
            float x1 = 20;
            float x1_ = x1;
            float y = 60;
            float z2 = _height - 10;
            float angle_ = angle;
            
            if(z2 < 20)
            {
                z2 = 20;
            }
            float z1 = z2 / 2;
            float z0 = 1;
            
            for(Leg *l : Legs)
            {
                angle = angle_;
                x1 = x1_;
                AngleX1(l, &angle, &x1);
                
                l->SetXYZ(RotateX(-x1, angle), RotateY(-x1, y, angle), z0, _height, _duration/4);
                l->SetXYZ(RotateX(-x1, angle), RotateY(-x1, y, angle), z1, _height, _duration/8);
                l->SetXYZ(RotateX(x0, angle),  RotateY(x0, y, angle),  z2, _height, _duration/8);
                l->SetXYZ(RotateX(x1, angle),  RotateY(x1, y, angle),  z1, _height, _duration/8);
                l->SetXYZ(RotateX(x1, angle),  RotateY(x1, y, angle),  z0, _height, _duration/8);
                l->SetXYZ(RotateX(x0, angle),  RotateY(x0, y, angle),  z0, _height, _duration/4);
            }
        }
        
        
        /*void Refresh()
        {
            if(Height == 0)
            {
                _height = 15;
                _duration = 600;
            }
            else if(Height == 1)
            {
                _height = 30;
                _duration = 1000;
            }
            else if(Height == 2)
            {
                _height = 50;
                _duration = 1400;
            }
            if(_oldHeight != _height)
            {
                MoveChangeHeight();
                _oldHeight = _height;
            }
        }*/

    private:
        int _height = 30;
        int _oldHeight = _height;
        uint16_t _duration = 1400;

        float RotateX(float x, float angle)
        {
            angle = (angle * PI) / 180;
            float res = x * cos(angle);
            return res;
        }
        
        float RotateY(float x, float y, float angle)
        {
            angle = (angle * PI) / 180;
            float res = x * -sin(angle) + y;
            return res;
        }

        void AngleX1(Leg *l, float *angle, float *x1)
        {
            if(!l->_right)
                {
                    *x1 = -*x1;
                    *angle = -*angle;
                }
                
                if(l->_right)
                {
                    if(l->_legPos == Front)
                        *angle -= 45;
                    else if(l->_legPos == Back)
                        *angle += 45;
                }
                else
                {
                    *angle += 180;
                    if(l->_legPos == Front)
                        *angle -= 45;
                    else if(l->_legPos == Back)
                        *angle += 45;
                }
        }
};

/********* VARIABLES *********/

MyServo coxa0(S0, false, 0);
MyServo femur0(S1, false, 1);
MyServo tibia0(S2);

MyServo coxa1(S3);
MyServo femur1(S4);
MyServo tibia1(S5);

MyServo coxa2(S6);
MyServo femur2(S7);
MyServo tibia2(S8);

MyServo coxa3(S9);
MyServo femur3(S10);
MyServo tibia3(S11);

MyServo coxa4(S12);
MyServo femur4(S13);
MyServo tibia4(S14);

MyServo coxa5(S15);
MyServo femur5(S16);
MyServo tibia5(S17);

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
    //unsigned long currM, postM = 0;
    Serial.println("updateServos running on core: ");
    Serial.println(xPortGetCoreID());
    while(1)
    {
        //currM = millis();
        //currM = millis();
        /*for(int i = 0; i < 18; i++)
        {
            allServos[i]->Refresh();
        }*/
        for (MyServo *servo : allServos)
        {
            servo->Refresh();
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
    //tibia5.SetDefault(tibia5.GetMid() + 7);

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

/*
char input[100];
char inChar = -1;
int indx = 0;

xd = 0;

void loop1(void *asdf) {
    while(1)
    {
        if(!xd)
        {
            Serial.println("loop running on core: ");
            Serial.println(xPortGetCoreID());
            xd = 1;
        }
        
        if(Tronik.Finished())
        {
            switch(Tronik.state)
            {
                case Folded:
                    if(!Tronik.Fold)
                    {   
                        Tronik.MoveUnfold();
                        Tronik.state = Standing;
                    }
                    break;
                case Standing:
                    Tronik.RefreshStats();
                    if(Tronik.Fold)
                    {
                        Tronik.MoveFold();
                        Tronik.state = Folded;
                    }
                    if(!Tronik.Fold && Tronik.Speed > 0)
                    {
                        Tronik.MovePrepareStep();
                        Tronik.state = Walking;
                    }
                    break;
                case Walking:
                    if(Tronik.Speed > 0)
                    {
                        Tronik.MoveStep(Tronik.Angle);
                    }
                    else
                    {
                        Tronik.MoveWaitForStep();
                        Tronik.state = Standing;
                    }
                    break;
                default:
                    break;
            }
        }
        
        
        
        if (SerialBT.available() > 0) 
        {
            while(inChar != '\n')
            {
                if(SerialBT.available() > 0)
                {
                    inChar = SerialBT.read();
                    input[indx] = inChar;
                    indx++;
                    input[indx] = '\0';
                }
            }
            ParseInput(input, &Tronik.Speed, &Tronik.Angle, &Tronik.Fold, &Tronik.Strafe, &Tronik.Height);
            indx = 0;
            inChar = -1;
        }
        delay(1);
    }
}
*/

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

    Tronik.Prep33();
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
        Tronik.Step33(0);
    }
    delay(1);

}
