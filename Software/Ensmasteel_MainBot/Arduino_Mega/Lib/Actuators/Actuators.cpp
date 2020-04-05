#include "Actuators.h"

Actuator::Actuator(String name)
{
    this->name = name;
    etat = Actuator_State::Attente;
}

Actuator_State Actuator::Update()
{
    if (etat == Actuator_State::MouvFinished)
    {
        etat = Actuator_State::Attente;
    }

    return etat;
}

void Actuator::NewOrder(Actuator_Order order)
{
    currentOrder = order;
    etat = Actuator_State::NewMess;
}

Pavillon::Pavillon() : Actuator("Pav")
{
    etat = Actuator_State::Attente;
}

Bras::Bras() : Actuator("Bras")
{
}

Actuator_State Bras::Update()
{
    if (etat == Actuator_State::NewMess)
    {
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
    }
    Actuator::Update();
}

void Bras::Init(uint8_t pinServo, String ID, int posRentre, int posSortie)
{
    this->posRentre = posRentre;
    this->posSortie = posSortie;
    this->pinServo = pinServo;
    this->ID = ID;
    name += ID;
    servo.attach(pinServo);
}