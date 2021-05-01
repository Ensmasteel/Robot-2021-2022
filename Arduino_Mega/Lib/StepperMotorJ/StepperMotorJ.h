#ifndef STEPPERMOTORJ_H_
#define STEPPERMOTORJ_H_

#include "Arduino.h"

class StepperMotorJ {
    public:
        StepperMotorJ(int stepPin, int dirPin, Boolean up);
        void move(int steps, int delay);

    private:
        int stepPin;
        int dirPin;
}