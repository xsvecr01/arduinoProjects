/********* INCLUDE *********/
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <cQueue.h>
#include <pthread.h>
#include <mutex>


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


enum LegPosition { Front, Middle, Back};

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
            _durationMillis = millis();

            _angle = _default = _desired = _mid = 90;

            q_init(&commandQ, sizeof(Command), 20, FIFO, false);
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
            _default =  value;
            _mid = value;
            _desired = value;
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


        void SetPos(uint8_t angle, uint16_t duration)
        {
            uint8_t diff;
            uint8_t tmp_angle = _desired;
            Command *tmp;
            if(q_peekPrevious(&commandQ, &tmp))
            {
                tmp_angle = tmp->angle;
            }
            
            diff = abs(tmp_angle - angle);

            float delayMs;
            if(diff > 0)
            {
                delayMs = ((SERVO_STEP * duration) / diff);
            }
            if(delayMs < 0)
            {
                delayMs = 0;
            }
            Command *cmd = new (Command){angle, delayMs, duration};
            _mtx.lock();
            q_push(&commandQ, &cmd);
            _mtx.unlock();
        }

        void SetPosFast(uint8_t angle)
        {
            _SetPos(angle);
            _desired = angle;
        }

        void Refresh()
        {
            if(_angle == _desired && q_isEmpty(&commandQ))
            {
                _durationMillis = millis();
                return;
            }
            _currentMillis = millis();
            _diffMillis = _currentMillis - _durationMillis;
            
            if (_desired > _angle)
            {
                while(_diffMillis >= (unsigned long) _delayMs * _steps)
                {
                    _angle += SERVO_STEP;
                    _steps++;
                }
                while(_desired < _angle)
                {
                    _angle --;
                }
            }
            else if (_desired < _angle)
            {
                while(_diffMillis >= (unsigned long) _delayMs * _steps)
                {
                    _angle -= SERVO_STEP;
                    _steps++;
               }
                while(_desired > _angle)
                {
                    _angle++;
                }
            }

            _SetPos(_angle);
            
            if(_angle == _desired && !q_isEmpty(&commandQ))
            {
                _Pop_queue();
                _durationMillis = millis();
                _steps = 0;
            }
        }

        

        uint8_t GetPos()
        {
            return _angle;
        }

    private:
        uint8_t _pin, _channel, _desired, _mid, _default, _angle;
        float _delayMs = 10;
        float _steps = 0;
        uint16_t _duration = 400;
        unsigned long _currentMillis;
        unsigned long _durationMillis;
        bool _pca;
        std::mutex _mtx;
        

        struct Command
        {
            uint8_t angle;
            float delayMs;
            uint16_t duration;
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

        void _Pop_queue()
        {
            Command* cmd;
            _mtx.lock();
            q_pop(&commandQ, &cmd);
            _mtx.unlock();
            _desired = cmd->angle;
            _delayMs = cmd->delayMs;
            _duration = cmd->duration;
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

        bool Finished()
        {
            if(q_getCount(&_Coxa->commandQ) <= 6 && q_getCount(&_Femur->commandQ) <= 6 && q_getCount(&_Tibia->commandQ) <= 6)
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
        int8_t height = 30;

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
        }

        bool Finished()
        {
            return RF->Finished() && RM->Finished() && RB->Finished() && LF->Finished() && LM->Finished() && LB->Finished();
        }

        void PrepareStep(uint16_t duration)
        {
            RF->Sleep(duration/2);
            RB->Sleep(duration/2);
            LM->Sleep(duration/2);
        }

        void Step(float angle, uint16_t duration)
        {
            float x0 = 0;
            float x1 = 20;
            float x1_ = x1;
            float y = 60;
            float z2 = height - 10;
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

                if(!l->_right)
                {
                    x1 = -x1;
                    angle = -angle;
                }
                
                if(l->_right)
                {
                    if(l->_legPos == Front)
                        angle -= 45;
                    else if(l->_legPos == Back)
                        angle += 45;
                }
                else
                {
                    angle += 180;
                    if(l->_legPos == Front)
                        angle -= 45;
                    else if(l->_legPos == Back)
                        angle += 45;
                }
                
                l->SetXYZ(RotateX(-x1, angle), RotateY(-x1, y, angle), z0, height, duration/4);
                l->SetXYZ(RotateX(-x1, angle), RotateY(-x1, y, angle), z1, height, duration/8);
                l->SetXYZ(RotateX(x0, angle),  RotateY(x0, y, angle),  z2, height, duration/8);
                l->SetXYZ(RotateX(x1, angle),  RotateY(x1, y, angle),  z1, height, duration/8);
                l->SetXYZ(RotateX(x1, angle),  RotateY(x1, y, angle),  z0, height, duration/8);
                l->SetXYZ(RotateX(x0, angle),  RotateY(x0, y, angle),  z0, height, duration/4);
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



/********* FUNCTIONS *********/

void* updateServos(void* d)
{
    int count = 0;
    while(1)
    {
        for (int i = 0; i < 18; i++)
        {
            allServos[i]->Refresh();
        }
        count++;
        if(count > 100)
        {
            delay(2);
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

int dur = 1200;
int angle = 90;
/********* SETUP *********/
void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);

    pcaBoard.begin();
    pcaBoard.setPWMFreq(FREQ);

    initServos();
    delay(1000);

    int d = 0;
    int res = pthread_create(&thr_update, NULL, updateServos, NULL);



    /*for(int j = 0; j < 6; j++)
    {
        //allLegs[i]->Middle(1000);
        allLegs[j]->SetXYZ(0, 60, 1, 30, dur);
    }*/
    Tronik.PrepareStep(dur);

}


/********* MAIN *********/
void loop() {
    //updateServos();
    if(Tronik.Finished())
    {
        Tronik.Step(angle, dur);
        Serial.println(millis());
    }
    /*for(int i = 0; i < 18; i++)
    {
        Serial.print(i);
        Serial.print(": ");
        Serial.println(allServos[i]->_diffMillis);
    }*/
    
    //Serial.println(millis1());
    //delay(1);
}
