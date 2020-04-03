#include "Actuators_Manager.h"

Manager::Manager(Stream *serController, HardwareSerial *serDebug)
{
    comController = Communication(serController);
    serialDebug = serDebug;
}

void Manager::Update()
{
    comController.update();
    
    if (comController.inWaitingRx() > 0)
    {
        currentMessage = comController.peekOldestMessage();
        currentID = extractID(currentMessage);
        currentBytes = extract4Bytes(currentMessage);
        
        switch (currentID)
        {
        case MessageID::Pavillon_M :
            pavillon.Update(Actuator_State::High);
            serialDebug->print("Pav order");
            break;
        
        default:
            break;
        }

        comController.popOldestMessage();
    }
}