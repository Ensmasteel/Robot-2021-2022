#ifndef MOTEUR_H_
#define MOTEUR_H_
#include "Arduino.h"

class Motor
{
    uint8_t pinPWR,pinSens;
    uint16_t maxPwm;
    uint8_t bitResolution;
    float order,orderByPass;
    bool bypass=false;
public:
    void actuate();                     //Actualisation de la puissance moteur
    Motor(uint8_t pinPWR, uint8_t pinSens,uint8_t bitResolution);
    void setOrder(float order);
    void stop();
    void bypassOrder(float order);
    void resume();
    
};

#endif
