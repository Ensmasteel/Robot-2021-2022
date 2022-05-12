#include "Actuators_Manager.h"

//test commit

Manager::Manager(Stream *serController, HardwareSerial *serDebug)
{
    comController = Communication(serController);
    serialDebug = serDebug;

    tourelleDroite.Init(tourelleDroite_Dir, tourelleDroite_Step, tourelleDroite_M0, tourelleDroite_M1,MessageID::TourelleD_M);
    tourelleGauche.Init(tourelleGauche_Dir, tourelleGauche_Step, tourelleGauche_M0, tourelleGauche_M1,MessageID::TourelleG_M);
    brasGauche.Init(brasGauche_servoPWM1,brasGauche_servoPWM2,brasGauche_servoPWM3, MessageID::BrasG_M);
    brasDroit.Init(brasDroit_servoPWM1,brasDroit_servoPWM2,brasDroit_servoPWM3, MessageID::BrasD_M);
    pompeBrasDroit.Init(pompeBrasDroit_PIN, MessageID::Pompe_BrasD_M);
    pompeBrasGauche.Init(pompeBrasGauche_PIN, MessageID::Pompe_BrasG_M);
    pompeStockageDroit.Init(pompeStockageDroit_PIN, MessageID::Pompe_StockageD_M);
    pompeStockageGauche.Init(pompeStockageGauche_PÏN, MessageID::Pompe_StockageG_M);

    listActuators[0] = &tourelleDroite;
    listActuators[1] = &tourelleGauche;
    listActuators[2] = &brasDroit;
    listActuators[3] = &brasGauche;
    listActuators[4] = &pompeBrasDroit;
    listActuators[5] = &pompeBrasGauche;
    listActuators[6] = &pompeStockageDroit;
    listActuators[7] = &pompeStockageGauche;
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