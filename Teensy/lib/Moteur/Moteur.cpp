#include "Moteur.h"
#include "Arduino.h"

//Il faut accorder le nombre de bit de la résolution du PWM avec sa fréquence
//cf la table sur le site: https://www.pjrc.com/teensy/td_pulse.html
double idealFrequency(uint8_t bitResolution)
{
    switch (bitResolution)
    {
    case 8:
        return 234375.0;
        break;
    case 9:
        return 117187.5;
        break;
    case 10:
        return 58593.75;
        break;
    case 11:
        return 29296.875;
        break;
    case 12:
        return 14648.437;
        break;
    case 13:
        return 7324.219;
        break;
    case 14:
        return 3662.109;
        break;
    case 15:
        return 1831.055;
        break;
    case 16:
        return 915.527;
        break;
    default:
        return 234375.0;
        break;
    }
}

void Motor::actuate()
{
    if (bypass)
        order = orderByPass;

    uint16_t pwmValue = round(abs(order) * maxPwm);

    analogWrite(pinPWR, pwmValue);
    if (order >= 0)
        digitalWrite(pinSens, HIGH);
    else
        digitalWrite(pinSens, LOW);
}

Motor::Motor(uint8_t pinPWR, uint8_t pinSens, uint8_t bitResolution)
{

    this->bitResolution = bitResolution;
    this->maxPwm = (uint16_t)round(pow(2, bitResolution)) - 1;
#ifdef TEENSY35
    analogWriteResolution(bitResolution);
    analogWriteFrequency(pinPWR, idealFrequency(bitResolution));
#endif
    bypass = false;
    this->pinPWR = pinPWR;
    pinMode(pinPWR, OUTPUT);
    this->pinSens = pinSens;
    pinMode(pinSens, OUTPUT);
    order = 0;
    orderByPass = 0;
}

void Motor::setOrder(float order)
{
    this->order = order;
}

void Motor::stop()
{
    orderByPass = 0;
    bypass = true;
}

void Motor::bypassOrder(float order)
{
    this->orderByPass = order;
    bypass = true;
}

void Motor::resume()
{
    bypass = false;
}
