#ifndef ACTUATORS_MANAGER_H_
#define ACTUATORS_MANAGER_H_

#include "Arduino.h"
#include "Communication.h"
#include "MessageID.h"
#include "Actuators.h"

#define NBR_ACTUATORS 6

class Manager
{
public:
    Manager(Stream* serControllerStream, HardwareSerial* serDebug = &Serial);
    void Update();
private:
    HardwareSerial* serialDebug;
    Communication comController;
    
    // ACTUATORS //
    Actuator* listActuators[NBR_ACTUATORS];
    Pavillon pavillon;
    Bras brasGauche;
    Bras brasDroit;
    PinceAvant pinceAvantGauche;
    PinceAvant pinceAvantDroite;
    PinceArriere pinceArr;

    // PINS //
    //////////
    
    // Bras
    uint8_t brasGauche_servoPWM = 3;
    uint8_t brasDroit_servoPWM = 4;

    // Pavillon
    uint8_t pav_Dir = 35;
    uint8_t pav_Step = 34;
    uint8_t pav_Sleep = 22;
    uint8_t pav_M0 = 37;
    uint8_t pav_M1 = 36;

    // Pinces
    uint8_t pinceArr_servoPWM = 5;
    uint8_t pinceAvG_servoPWM = 6;
    uint8_t pinceAvD_servoPWM = 7;
    
    uint8_t pinceAvG_Dir = 24;
    uint8_t pinceAvG_Step = 23;
    uint8_t pinceAvD_Dir = 26;
    uint8_t pinceAvD_Step = 25;
    uint8_t pinceArr_Dir = 31;
    uint8_t pinceArr_Step = 30;

    uint8_t pincesAv_M0 = 29;
    uint8_t pincesAv_M1 = 28;
    uint8_t pincesArr_M0 = 33;
    uint8_t pincesArr_M1 = 32;

    uint8_t pinces_Sleep = 27;

    // DATA MESSAGE //
    Message currentMessage;
    MessageID currentID = MessageID::Stop_M;
    FourBytes currentBytes;
    Actuator_Order currentOrder;
};

#endif