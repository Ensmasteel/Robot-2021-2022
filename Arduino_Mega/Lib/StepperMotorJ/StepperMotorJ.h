/*#ifndef STEPPERMOTORJ_H_
#define STEPPERMOTORJ_H_

#include "Arduino.h"

class StepperMotorJ {
    public:
        StepperMotorJ(int stepPin, int dirPin, bool up);
        void move(int steps, int delay);

    private:
        int stepPin;
        int dirPin;
}
#endif */