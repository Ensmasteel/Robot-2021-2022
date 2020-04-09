#ifndef ACTUATORS_MANAGER_H_
#define ACTUATORS_MANAGER_H_

#include "Arduino.h"
#include "Communication.h"
#include "MessageID.h"
#include "Actuators.h"

#define NBR_ACTUATORS 3

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

    // PINS //
    uint8_t servoPWM_BrasGauche = 3;
    uint8_t servoPWM_BrasDroit = 4;

    // DATA MESSAGE //
    Message currentMessage;
    MessageID currentID = MessageID::Stop_M;
    FourBytes currentBytes;
    Actuator_Order currentOrder;
};

#endif