#include "Filtre.h"
#include "Vector.h"
#include "Arduino.h"

void Filtre::in(float newRaw, float dt)
{
    if (dt > 0.5)
        lastValue = newRaw; //Le filtre n'a pas ete appelle pendant trop de temps
    else
        lastValue = (lastValue + newRaw * dt * wc) / (wc * dt + 1);
}

float Filtre::out()
{
    return lastValue;
}

Filtre::Filtre(float initValue, float frequency)
{
    this->lastValue = initValue;
    this->wc = 2 * PI * frequency;
}

Filtre::Filtre() {}

void Filtre::reset(float value)
{
    lastValue = value;
}
