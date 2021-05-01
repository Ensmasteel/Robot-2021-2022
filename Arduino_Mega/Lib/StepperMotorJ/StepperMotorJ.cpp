#include "StepperMotorJ.h"

StepperMotorJ(int stepPin, int dirPin) {
    this->stepPin = stepPin;
    this->dirPin = dirPin;
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
}

void StepperMotorJ::move(int steps, int delay, Boolean up) {
    digitalWrite(dirPin, up ? HIGH : LOW);

    for(int x = 0; x < steps; x++) {
        digitalWrite(stepPin,HIGH);
        delayMicroseconds(delay);
        digitalWrite(stepPin,LOW);
        delayMicroseconds(delay);
  }
}