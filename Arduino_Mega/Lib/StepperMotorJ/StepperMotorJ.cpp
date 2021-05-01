#include "StepperMotorJ.h"

StepperMotorJ::StepperMotorJ(int stepPin, int dirPin) {
    this->stepPin = stepPin;
    this->dirPin = dirPin;
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
}

void StepperMotorJ::move(int steps, int delay, bool up) {
    digitalWrite(dirPin, up ? HIGH : LOW);
    Serial.println(steps);
    for(int x = 0; x < steps; x++) {
        digitalWrite(stepPin,HIGH);
        delayMicroseconds(delay);
        digitalWrite(stepPin,LOW);
        delayMicroseconds(delay);
  }
}
