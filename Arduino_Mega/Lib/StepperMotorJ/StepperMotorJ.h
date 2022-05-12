#ifndef STEPPERMOTORJ_H_
#define STEPPERMOTORJ_H_

#include "Arduino.h"

/**
 * Custom class to control a stepperMotor.
 */
class StepperMotorJ {
    public:

        /**
         * Public constructor of the StepperMotor.
         * @param stepPin : pin number of the step pin.
         * @param dirPin : pin number of the direction pin.
         * @param sleepPin : pin number of the sleep pin.
         * @param pinM0 : pin number of the first motor.
         * @param pinM1 : pin number of the second motor.
         */
        StepperMotorJ(int stepPin, int dirPin, int pinM0, int pinM1);

        /**
         * Method to make the motor move.
         * @param steps : number of steps that the stepper has to do.
         * @param delay : duration while the stepper should wait for the next action.
         * @param up
         * @param holdPosition : Bool, represent whether the stepper should wait in final position or go back to the
         * initial one.
         */
        void move(int steps, int delay, bool up, bool holdPosition);

    private:
        int stepPin;
        int dirPin;
        //int sleepPin;
        int pinM0;
        int pinM1;
};

#endif 