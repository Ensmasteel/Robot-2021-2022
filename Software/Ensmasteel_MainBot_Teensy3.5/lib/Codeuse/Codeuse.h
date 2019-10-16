#ifndef CODEUSE_H_
#define CODEUSE_H8

#include "Arduino.h"
#include <Encoder.h>

class Codeuse
{
    int32_t ticks,oldTicks;     //Nombre de ticks compté, nombre de ticks compté au dernier appel
    float debug;
    float diametreRoue;
    uint16_t ticksPerRound;

public:
    
    float v,deltaAvance;        //Vitesse et avance du robot AU NIVEAU DE LA ROUE CODEUSE
    
    void actuate(float dt);     //Actualise (transforme les ticks en vitesse puis en avance)
    Codeuse();
    Codeuse(uint8_t pinA,uint8_t pinB, uint16_t ticksPerRound, float diametreRoue);
    Encoder *enc;
};
#endif
