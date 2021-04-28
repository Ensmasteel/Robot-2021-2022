#include "Actuators_Manager.h"

Manager::Manager(Stream *serController, HardwareSerial *serDebug)
{
    comController = Communication(serController);
    serialDebug = serDebug;

    pavillon.Init(pav_Dir, pav_Step, pav_Sleep, pav_M0, pav_M1);
    brasGauche.Init(brasGauche_servoPWM, MessageID::BrasG_M, 90, 180);
    brasDroit.Init(brasDroit_servoPWM, MessageID::BrasD_M, 90, 0);
    pinceAvantGauche.Init(pinceAvG_servoPWM, pinceAvG_Dir, pinceAvG_Step, pinces_Sleep, pincesAv_M0, pincesAv_M1, MessageID::PinceAvG_M);
    pinceAvantDroite.Init(pinceAvD_servoPWM, pinceAvD_Dir, pinceAvD_Step, pinces_Sleep, pincesAv_M0, pincesAv_M1, MessageID::PinceAvD_M);
    pinceArr.Init(pinceArr_servoPWM, pinceArr_Dir, pinceArr_Step, pinces_Sleep, pincesArr_M0, pincesArr_M1, MessageID::PinceArr_M, 175, 120);

    listActuators[0] = &pavillon;
    listActuators[1] = &brasGauche;
    listActuators[2] = &brasDroit;
    listActuators[3] = &pinceAvantGauche;
    listActuators[4] = &pinceAvantDroite;
    listActuators[5] = &pinceArr;
    // Check NBR_ACTUATORS when actuators added
}

void Manager::Update()
{
    comController.update();

    // Reception des ordres
    if (comController.inWaitingRx() > 0)
    {
        currentMessage = comController.peekOldestMessage();
        currentID = extractID(currentMessage);
        // currentBytes = extract4Bytes(currentMessage);
        currentOrder = extractOrder(currentMessage);

        serialDebug->println(int(currentID));

        for (int i = 0; i < NBR_ACTUATORS; i++)
        {
            if (currentID == listActuators[i]->GetID())
            {
                listActuators[i]->NewOrder(currentOrder);
                serialDebug->println(listActuators[i]->GetOrder());
            }
        }
        comController.popOldestMessage();
    }

    // Update des actionneurs
    for (int i = 0; i < NBR_ACTUATORS; i++)
    {
        if (listActuators[i]->Update() == Actuator_State::MouvFinished)
        {
            comController.send(listActuators[i]->OrderCompleted());
        }
    }
}