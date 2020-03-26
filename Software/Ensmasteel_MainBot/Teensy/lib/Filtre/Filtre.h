#ifndef FILTRE_INCLUDED
#define FILTRE_INCLUDED
#include "Arduino.h"

class Filtre
{
public:
    float lastValue;
    float wc;
    void in(float newRaw, float dt);
    float out();
    void reset(float value);
    Filtre(float initValue, float frequency);
    Filtre();
};
#endif
