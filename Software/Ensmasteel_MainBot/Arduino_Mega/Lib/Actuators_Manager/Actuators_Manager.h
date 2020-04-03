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

    Pavillon pavillon; // a mettre en private quand debug fini

private:
    HardwareSerial* serialDebug;
    Communication comController;
    Message currentMessage;
    MessageID currentID = MessageID::Stop_M;
    FourBytes currentBytes;

};

#endif