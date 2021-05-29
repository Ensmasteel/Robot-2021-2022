#ifndef STEPPERMOTORJ_H_
#define STEPPERMOTORJ_H_

#include "Arduino.h"

class StepperMotorJ {
    public:
        StepperMotorJ(int stepPin, int dirPin, int sleepPin, int pinM0, int pinM1);
        void move(int steps, int delay, bool up, bool holdPosition);

    private:
        int stepPin;
        int dirPin;
        int sleepPin;
        int pinM0;
        int pinM1;
};
#endif 