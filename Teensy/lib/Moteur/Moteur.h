#ifndef MOTEUR_H_
#define MOTEUR_H_
#include "Arduino.h"

class Motor
{
    uint8_t pinPWR, pinSens;
    uint16_t maxPwm;
    uint8_t bitResolution;
    float orderByPass;
    bool bypass = false; //Lorsque le bypass est activé, le moteur prend l'odre orderByPass comme ordre et ignore l'order
    //Cela permet notamment de faire des recallage: On impose une force sur les deux moteurs en boucle ouverte pendant un certain temps

public:
    float order;    //Public pour le simulateur
    void actuate(); //Actualisation de la puissance moteur
    Motor(uint8_t pinPWR, uint8_t pinSens, uint8_t bitResolution = 8);
    Motor() {}
    void setOrder(float order);    //Change la valeur de l'ordre courant
    void stop();                   //Arrete les moteurs et ignore l'ordre order jusqu'a l'appel de resume
    void bypassOrder(float order); //Impose cet ordre jusqu'a l'appel de resume
    void resume();                 //Le moteur suit maintenant l'ordre standard "order"
};

#endif
