#include "Actuators_Manager.h"

Manager::Manager(Stream *serController, HardwareSerial *serDebug)
{
    comController = Communication(serController);
    serialDebug = serDebug;
    brasGauche.Init(servoPWM_BrasGauche, MessageID::BrasG_M);
    brasDroit.Init(servoPWM_BrasDroit, MessageID::BrasD_M);

    listActuators[0] = &pavillon;
    listActuators[1] = &brasGauche;
    listActuators[2] = &brasDroit;
    // Check NBR_ACTUATORS when actuators added
}

void Manager::Update()
{
    comController.update();

    if (comController.inWaitingRx() > 0)
    {
        currentMessage = comController.peekOldestMessage();
        currentID = extractID(currentMessage);
        currentBytes = extract4Bytes(currentMessage);
        currentOrder = static_cast<Actuator_Order>(currentBytes.byte0);

        serialDebug->println(int(currentID));

        for (int i=0; i < NBR_ACTUATORS; i++) {
            if (currentID == listActuators[i]->GetID())
            {
                listActuators[i]->NewOrder(currentOrder);
                serialDebug->println(listActuators[i]->GetOrder());
            }
        }
        
        comController.popOldestMessage();
    }

    if (pavillon.Update() == Actuator_State::MouvFinished)
    {
        comController.send(newMessage(MessageID::Pavillon_M));
    }
    
    brasDroit.Update();
    brasGauche.Update();
}