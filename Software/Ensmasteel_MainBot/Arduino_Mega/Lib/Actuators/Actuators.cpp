#include "Actuators.h"

Actuator::Actuator(String name = "Actuator")
{
    this->name = name;
    etat = Actuator_State::Idle;
}

void Actuator::Update(Actuator_State order)
{
    etat = order;
}

Pavillon::Pavillon() : Actuator("Pav")
{
    etat = Actuator_State::Low;
}
