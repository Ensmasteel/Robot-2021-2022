#include "StepperMotorJ.h"

StepperMotorJ::StepperMotorJ(int stepPin, int dirPin, int pinM0, int pinM1) {
    this->stepPin = stepPin;
    this->dirPin = dirPin;
    //this->sleepPin = sleepPin;
    this->pinM0 = pinM0;
    this->pinM1 = pinM1;
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    //pinMode(sleepPin, OUTPUT);
    pinMode(pinM0, OUTPUT);
    pinMode(pinM1, OUTPUT);

    digitalWrite(pinM0, LOW);
    digitalWrite(pinM1, LOW);
}

void StepperMotorJ::move(int steps, int delay, bool up, bool holdPosition) {
    //digitalWrite(sleepPin, HIGH);
    digitalWrite(dirPin, up ? HIGH : LOW);
    for(int x = 0; x < steps; x++) {
        digitalWrite(stepPin,HIGH);
        delayMicroseconds(delay);
        digitalWrite(stepPin,LOW);
        delayMicroseconds(delay);
    }
    //digitalWrite(sleepPin, holdPosition ? HIGH : LOW);
}
