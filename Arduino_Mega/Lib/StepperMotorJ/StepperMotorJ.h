#ifndef STEPPERMOTORJ_H_
#define STEPPERMOTORJ_H_

#include "Arduino.h"

class StepperMotorJ {
    public:
        StepperMotorJ(int stepPin, int dirPin, int sleepPin);
        void move(int steps, int delay, bool up);

    private:
        int stepPin;
        int dirPin;
        int sleepPin;
};
#endif 