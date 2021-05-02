#include "StepperMotorJ.h"

StepperMotorJ::StepperMotorJ(int stepPin, int dirPin, int sleepPin) {
    this->stepPin = stepPin;
    this->dirPin = dirPin;
    this->sleepPin = sleepPin;
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(sleepPin, OUTPUT);
}

void StepperMotorJ::move(int steps, int delay, bool up) {
    digitalWrite(sleepPin, HIGH);
    digitalWrite(dirPin, up ? HIGH : LOW);
    Serial.println(steps);
    for(int x = 0; x < steps; x++) {
        digitalWrite(stepPin,HIGH);
        delayMicroseconds(delay);
        digitalWrite(stepPin,LOW);
        delayMicroseconds(delay);
    }
    digitalWrite(sleepPin, LOW);
}
