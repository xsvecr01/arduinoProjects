#include "MyServo.h"

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
