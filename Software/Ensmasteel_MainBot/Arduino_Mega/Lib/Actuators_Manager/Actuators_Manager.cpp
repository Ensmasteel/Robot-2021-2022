#include "Actuators_Manager.h"

Manager::Manager(Stream *serController, HardwareSerial *serDebug)
{
    comController = Communication(serController);
    serialDebug = serDebug;
    brasGauche.Init(servoPWM_BrasGauche, "G");
    brasDroit.Init(servoPWM_BrasDroit, "D");
}

void Manager::Update()
{
    comController.update();

    if (comController.inWaitingRx() > 0)
    {
        currentMessage = comController.peekOldestMessage();
        currentID = extractID(currentMessage);
        currentBytes = extract4Bytes(currentMessage);
        currentPosition = static_cast<Actuator_Position>(currentBytes.byte0);
        currentOrder = static_cast<Actuator_Order>(currentBytes.byte1);

        serialDebug->println(int(currentID));

        switch (currentID)
        {
        case MessageID::Pavillon_M:
            pavillon.NewOrder(currentOrder);
            serialDebug->println(pavillon.GetOrder());
            break;

        case MessageID::Bras_M:
            switch (currentPosition)
            {
            case Actuator_Position::Gauche:
                brasGauche.NewOrder(currentOrder);
                serialDebug->println(brasGauche.GetName());
                break;

            case Actuator_Position::Droit:
                brasDroit.NewOrder(currentOrder);
                serialDebug->println(brasDroit.GetName());
                break;

            default:
                serialDebug->println("Position error");
                break;
            }
            break;

        default:
            serialDebug->println("ID error");
            break;
        }
        comController.popOldestMessage();
    }

    if (Actuator_State::MouvFinished == pavillon.Update())
    {
        serialDebug->println("Move pav");
    }
    brasDroit.Update();
    brasGauche.Update();
}