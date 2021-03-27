#include "Actuators.h"

Actuator::Actuator(String name, MessageID messID)
{
    this->messID = messID;
    this->name = name;
    etat = Actuator_State::Attente;
}

Actuator_State Actuator::Update()
{
    return etat;
}

void Actuator::NewOrder(Actuator_Order order)
{
    currentOrder = order;
    etat = Actuator_State::NewMess;
}

Pavillon::Pavillon() : Actuator("Pav", MessageID::Pavillon_M)
{
    stepperMotor = new DRV8834(motorSteps, pinDir, pinStep, pinSleep, pinM0, pinM1);
    stepperMotor->begin(motorRPM,(short)1);
    stepperMotor->disable();
}

void Pavillon::Init(uint8_t pinDir, uint8_t pinStep, uint8_t pinSleep, uint8_t pinM0, uint8_t pinM1)
{
    this->pinDir = pinDir;
    this->pinStep = pinStep;
    this->pinSleep = pinSleep;
    this->pinM0 = pinM0;
    this->pinM1 = pinM1;
    etat = Actuator_State::Attente;
}

Actuator_State Pavillon::Update()
{
    switch (etat)
    {
    case Actuator_State::NewMess:
        switch (currentOrder)
        {
        case Actuator_Order::Monter:
            stepperMotor->enable();
            stepperMotor->move(-actionStep);
            stepperMotor->disable();
            break;

        case Actuator_Order::Descendre:
            stepperMotor->enable();
            stepperMotor->move(-actionStep);
            stepperMotor->disable();
            break;

        default:
            break;
        }
        etat = Actuator_State::MouvFinished;
        break;
    
    case Actuator_State::MouvFinished:
        etat = Actuator_State::Attente;
        break;

    default:
        break;
    }
}

Bras::Bras() : Actuator("Bras")
{
}

Actuator_State Bras::Update()
{
    switch (etat)
    {
    case Actuator_State::NewMess:
        switch (currentOrder)
        {
        case Actuator_Order::Sortir:
            servo.write(posSortie);
            break;

        case Actuator_Order::Rentrer:
            servo.write(posRentre);
            break;

        default:
            break;
        }
        etat = Actuator_State::MouvFinished;
        break;
    
    case Actuator_State::MouvFinished:
        etat = Actuator_State::Attente;
        break;
        
    default:
        break;
    }

    Actuator::Update();
}

void Bras::Init(uint8_t pinServo, MessageID ID, int posRentre, int posSortie)
{
    messID = ID;
    switch (messID)
    {
    case MessageID::BrasD_M:
        name += "D";
        break;

    case MessageID::BrasG_M:
        name += "G";
        break;
    
    default:
        break;
    }

    this->pinServo = pinServo; 
    this->posRentre = posRentre;
    this->posSortie = posSortie;

    servo.attach(pinServo);
    servo.write(posRentre);

    etat = Actuator_State::Attente;
}


Pince::Pince() : Actuator("Pince")
{
}

void Pince::Init(uint8_t pinServo, uint8_t pinDir, uint8_t pinStep, uint8_t pinSleep, uint8_t pinM0, uint8_t pinM1, MessageID ID)
{
    messID = ID;
    switch (messID)
    {
    case MessageID::PinceArr_M:
        name += "Arr";
        break;

    case MessageID::PinceAvD_M:
        name += "AvD";
        break;

    case MessageID::PinceAvG_M:
        name += "AvG";
        break;
    
    default:
        break;
    }

    this->pinServo = pinServo; 
    this->pinDir = pinDir;
    this->pinStep = pinStep;
    this->pinSleep = pinSleep;
    this->pinM0 = pinM0;
    this->pinM1 = pinM1;

    servo.attach(pinServo);
    servo.write(posFermee);

    stepperMotor = new DRV8834(motorSteps, pinDir, pinStep, pinSleep, pinM0, pinM1);
    stepperMotor->begin(motorRPM,(short)1);
    stepperMotor->disable();

    etat = Actuator_State::Attente;
}

Actuator_State Pince::Update()
{
    switch (etat)
    {
    case Actuator_State::NewMess:
        switch (currentOrder)
        {
        case Actuator_Order::Monter:
            stepperMotor->enable();
            stepperMotor->move(-actionStep);
            stepperMotor->disable();
            break;

        case Actuator_Order::Descendre:
            stepperMotor->enable();
            stepperMotor->move(-actionStep);
            stepperMotor->disable();
            break;
        
        case Actuator_Order::Ouvrir:
            servo.write(posOuverte);
            break;
        
        case Actuator_Order::Fermer:
            servo.write(posFermee);
            break;

        default:
            break;
        }
        etat = Actuator_State::MouvFinished;
        break;
    
    case Actuator_State::MouvFinished:
        etat = Actuator_State::Attente;
        break;

    default:
        break;
    }  
}