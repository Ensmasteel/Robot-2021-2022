#ifndef ACTUATORS_MANAGER_H_
#define ACTUATORS_MANAGER_H_

#include "Arduino.h"
#include "Communication.h"
#include "MessageID.h"
#include "Actuators.h"

class Manager
{
public:
    Manager(Stream* serControllerStream, HardwareSerial* serDebug = &Serial);
    void Update();

    // ACTUATORS // a mettre en private quand debug fini
    Pavillon pavillon;
    Bras brasGauche;
    Bras brasDroit;


private:
    HardwareSerial* serialDebug;
    Communication comController;

    // PINS //
    uint8_t servoPWM_BrasGauche = 1;
    uint8_t servoPWM_BrasDroit = 1;

    // DATA MESSAGE //
    Message currentMessage;
    MessageID currentID = MessageID::Stop_M;
    FourBytes currentBytes;
    Actuator_Order currentOrder;
    Actuator_Position currentPosition;
};

#endif