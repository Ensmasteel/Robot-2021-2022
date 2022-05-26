#ifndef ACTUATORS_MANAGER_H_
#define ACTUATORS_MANAGER_H_

#include "Arduino.h"
#include "Communication.h"
#include "MessageID.h"
#include "Actuators.h"

#define NBR_ACTUATORS 8

/**
 * Basic class to declare an actuator manager wich will organize all the actions during the match.
 */
class Manager
{
public:
    /**
     * Public constructor of the manager. Get 2 parameters :
     * @param serControllerStream : stream on which the information will transit in the robot itself.
     * @param serDebug : Serial port to show information during debugging.
     */
    Manager(Stream* serControllerStream, HardwareSerial* serDebug = &Serial);
    void Update();
private:
    HardwareSerial* serialDebug;
    Communication comController;
    
    // ACTUATORS //
    Actuator* listActuators[NBR_ACTUATORS];
    Pompe pompeBrasGauche;
    Pompe pompeBrasDroit;
    Pompe pompeStockageGauche;
    Pompe pompeStockageDroit;
    Bras brasGauche;
    Bras brasDroit;
    Tourelle tourelleGauche;
    Tourelle tourelleDroite;

    // PINS //
    //////////
    
    // Bras
    uint8_t brasGauche_servoPWM1 = 2;
    uint8_t brasGauche_servoPWM2 = 3;
    uint8_t brasGauche_servoPWM3 = 4;
    uint8_t brasDroit_servoPWM1 = 5;
    uint8_t brasDroit_servoPWM2 = 6;
    uint8_t brasDroit_servoPWM3 = 7;

    // Tourelle
    uint8_t tourelleGauche_Dir = 22;
    uint8_t tourelleGauche_Step = 23;
    //uint8_t pav_Sleep = 22;
    uint8_t tourelleGauche_M0 = 26;
    uint8_t tourelleGauche_M1 = 27;

    uint8_t tourelleDroite_Dir = 24;
    uint8_t tourelleDroite_Step = 25;
    //uint8_t pav_Sleep = 22;
    uint8_t tourelleDroite_M0 = 28;
    uint8_t tourelleDroite_M1 = 29;

    // Pompes
    uint8_t pompeBrasDroit_PIN = 32; //Ne pas utiliser la pin 30
    uint8_t pompeBrasGauche_PIN = 34;
    uint8_t pompeStockageDroit_PIN = 32;
    uint8_t pompeStockageGauche_PIN = 33;

    // DATA MESSAGE //
    Message currentMessage;
    MessageID currentID = MessageID::Stop_M;
    FourBytes currentBytes;
    Actuator_Order currentOrder;
};

#endif