#include "headers.h"



class MyServo
{
    public:
        MyServo(uint8_t pin, Adafruit_PWMServoDriver *pca, uint8_t channel = 0)
        {
            if (!pca)
            {
                ledcSetup(channel, FREQ, RESOLUTION);
                ledcAttachPin(pin, channel);
            }
            _pin = pin;
            pcaBoard = pca;
            _channel = channel;
            _startMillis = millis();

            _prevAngle = _angle = _default = _mid = 90;

            commandQ = xQueueCreate(16, sizeof(struct Command));
            if(!commandQ)
            {
                Serial.println("Error queue");
            }
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
            Command tmp;

            if(xQueuePeek(commandQ, &tmp, 0))
            {
                tmp_angle = tmp.angle;
            }

            SetPos(tmp_angle, duration);
        }


        void SetPos(uint8_t angle, float duration)
        {
            uint8_t tmp_angle = _desired;

            Command cmd;
            cmd.angle = angle;
            cmd.duration = duration;
            xQueueSend(commandQ, &cmd, 10);
        }

        void SetPosFast(uint8_t angle)
        {
            _SetPos(angle);
            _angle = angle;
            _prevAngle = angle;
            _desired = angle;
        }

        void Refresh(unsigned long currM)
        {
            //_currentMillis = millis();
            //if(_currentMillis - _startMillis >= _duration)
            //Serial.println("1");
            if(currM - _startMillis >= _duration)
            {
                //Serial.println("2");
                //_startMillis = millis();
                _startMillis = currM;
                _angle = _desired;
                _prevAngle = _angle;
                //Serial.println("3");

                //Command *tmp;
                //if(xQueuePeek(commandQ, &tmp, 0))
                //{
                Command cmd;
                //Serial.println("4");
                if(xQueueReceive(commandQ, &cmd, 0))
                {
                    //Serial.println("5");
                    _desired = cmd.angle;
                    _duration = cmd.duration;
                }
                //}

               //Serial.println("6");
            }
            else if(_desired != _angle);
            {
                //int16_t y = (int16_t) (_desired - _prevAngle) * ((_currentMillis - _startMillis) / _duration);
                //int16_t y = (int16_t) (_desired - _prevAngle) * ((currM - _startMillis) / _duration);
                //Serial.print(y);
                //Serial.print("_______________");
                //Serial.println((_desired - _prevAngle) * ((currM - _startMillis) / _duration));
                //Serial.println("7");
                _angle = _prevAngle + (_desired - _prevAngle) * ((currM - _startMillis) / _duration);
                /*Serial.println("8");
                Serial.print(_desired);Serial.print(" - ");Serial.print(_prevAngle);Serial.print(" * ");
                Serial.print(currM);Serial.print(" - ");Serial.print(_startMillis);Serial.print(" / ");Serial.println(_duration);
                Serial.println(_angle);*/
            }
            //Serial.println("9");
            _SetPos(_angle);
            //Serial.println("10");
        }

        bool QueueFinished()
        {
            if(uxQueueMessagesWaiting(commandQ) <= 8)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        uint8_t GetPos()
        {
            return _angle;
        }

    private:
        Adafruit_PWMServoDriver *pcaBoard;
        QueueHandle_t commandQ;
        
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
            if (!pcaBoard)
            {
                ledcWrite(_channel, _GetDuty(angle));
            }
            else
            {
                pcaBoard->setPWM(_pin, 0, _GetDuty(angle));
            }
            _angle = angle;
        }
};
