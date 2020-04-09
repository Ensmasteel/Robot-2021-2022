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
    etat = Actuator_State::Attente;
    stepperMotor = new DRV8834(motorSteps, pinDir, pinStep, pinSleep, pinM0, pinM1);
    stepperMotor->begin(motorRPM,(short)1);
    stepperMotor->disable();
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

Bras::Bras() : Actuator("Bras", MessageID::Bras_M)
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

void Bras::Init(uint8_t pinServo, String ID, int posRentre, int posSortie)
{
    this->pinServo = pinServo;
    this->ID = ID;
    name += ID;
    servo.attach(pinServo);
    this->posRentre = posRentre;
    this->posSortie = posSortie;
    servo.write(posRentre);
}