#include "Codeuse.h"

Interrupteur::Interrupteur(uint8_t pin)
{
    this->pin = pin;
    pinMode(pin, INPUT_PULLUP);
    contact = false;
}

void Interrupteur::updateContact()
{
    contact = (digitalRead(pin) == LOW);
}

bool Interrupteur::isContact()
{
    return contact;
}

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

Codeuse::Codeuse() {}

void Odometrie::updateCinetique(float dt)
{
    codeuseGauche.actuate(dt);
    codeuseDroite.actuate(dt);
    cinetique->_v = (codeuseDroite.v + codeuseGauche.v) / 2.0;
    cinetique->_w = (codeuseDroite.v - codeuseGauche.v) / eloignementCodeuses;
    cinetique->_theta += (codeuseDroite.deltaAvance - codeuseGauche.deltaAvance) / eloignementCodeuses;
    cinetique->normalizeTheta();
    (*cinetique) += directeur(cinetique->_theta) * ((codeuseDroite.deltaAvance + codeuseGauche.deltaAvance) / 2);

    interGauche->updateContact();
    interDroite->updateContact();
}

Odometrie::Odometrie(){}

Odometrie::Odometrie(uint16_t ticksPerRound, Cinetique *cinetique, float eloignementCodeuses,
                    uint8_t pinACodeuseGauche, uint8_t pinBCodeuseGauche, float diametreRoueGauche,
                    uint8_t pinACodeuseDroite, uint8_t pinBCodeuseDroite, float diametreRoueDroite,
                    uint8_t pinInterDroite, uint8_t pinInterGauche)
{
    codeuseGauche = Codeuse(pinACodeuseGauche, pinBCodeuseGauche, 2*ticksPerRound, diametreRoueGauche);
    codeuseDroite = Codeuse(pinACodeuseDroite, pinBCodeuseDroite, ticksPerRound, diametreRoueDroite);
    this->cinetique = cinetique;
    this->eloignementCodeuses = eloignementCodeuses;
    interGauche = new Interrupteur(pinInterGauche);
    interDroite = new Interrupteur(pinInterDroite);
}

bool Odometrie::getInterDroiteContact()
{
    return interDroite->isContact();
}

bool Odometrie::getInterGaucheContact()
{
    return interGauche->isContact();
}

