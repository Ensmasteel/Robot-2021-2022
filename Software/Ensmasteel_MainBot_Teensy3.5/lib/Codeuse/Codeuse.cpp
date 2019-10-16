#include "Codeuse.h"

void Codeuse::actuate(float dt)
{
    ticks = enc->read();                                                    //On recupère les ticks de l'objet Encoder (automatiquement mis a jour par interruptions cf cours ISE)
    deltaAvance = (ticks - oldTicks) * (PI * diametreRoue) / ticksPerRound; //Simple géométrie
    v = deltaAvance / dt;
    debug += deltaAvance;
    oldTicks = ticks;
}

Codeuse::Codeuse(uint8_t pinA, uint8_t pinB, uint16_t ticksPerRound, float diametreRoue)
{
    enc = new Encoder(pinA, pinB); //Objet Encoder de la librairie Encoder.
    this->diametreRoue = diametreRoue;
    this->ticksPerRound = ticksPerRound; //Nombe de ticks par tour
    ticks = 0;
    oldTicks = 0;
    v = 0.0;
    deltaAvance = 0;
    debug = 0;
}

Codeuse::Codeuse()
{
}
