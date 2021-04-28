#ifndef CODEUSE_H_
#define CODEUSE_H_

#include "Vector.h"
#include "Arduino.h"

#ifndef STM32BOTH
#include <Encoder.h>
#else // !STM32BOTH

class Encoder
{
public:
    Encoder(uint8_t pinA, uint8_t pinB) {}
    int32_t read() { return 0; }
};
#endif

class Codeuse
{
    int32_t ticks, oldTicks; //Nombre de ticks compté, nombre de ticks compté au dernier appel
    float debug;             //Distance parcourue par la codeuse depuis l'alllumage
    float diametreRoue;
    uint16_t ticksPerRound; //Nombre de ticks par tours de roue
    Encoder *enc;           //Objet Encoder de la librairie Encoder.

public:
    float v, deltaAvance; //Vitesse et avance du robot AU NIVEAU DE LA ROUE CODEUSE

    void actuate(float dt); //Actualise (transforme les ticks en vitesse puis en avance)
    Codeuse();
    Codeuse(uint8_t pinA, uint8_t pinB, uint16_t ticksPerRound, float diametreRoue);
};

class Odometrie
{
private:
    Codeuse codeuseGauche, codeuseDroite;
    Cinetique *cinetique;
    float eloignementCodeuses;

public:
    void updateCinetique(float dt);
    Odometrie(uint16_t ticksPerRound, Cinetique *cinetique, float eloignementCodeuses,
              uint8_t pinACodeuseGauche, uint8_t pinBCodeuseGauche, float diametreRoueGauche,
              uint8_t pinACodeuseDroite, uint8_t pinBCodeuseDroite, float diametreRoueDroite);
    Odometrie() {}
};

#endif