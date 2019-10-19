#include "Actions.h"
#include "Ghost.h"
#include "PID.h"

void Action::setPointers(Cinetique * robotCinetique, Ghost * ghost, Sequence * sequence, Communication * communication, Asservissement * asser)
{
    Action::robotCinetique=robotCinetique;
    Action::ghost=ghost;
    Action::sequence=sequence;
    Action::communication=communication;
    Action::asser=asser;
}

void Move_Action::start()
{
    switch (pace)
    {
        case accurate: asser->setCurrentProfile(1); break;
        case standard: asser->setCurrentProfile(2); break;
        case fast: asser->setCurrentProfile(3); break;
    }
    ghost->Compute_Trajectory(posFinal,deltaCurve,speedRamps,cruisingSpeed,pureRotation,backward);
}

bool Move_Action::isFinished()
{
    //return ------TODO------ ghost->hasFinished() && asser->close
    return false;
}

bool Move_Action::hasFailed()
{
    return asser->tooFar;
}

Move_Action::Move_Action(float timeout,VectorE posFinal, float deltaCurve, Pace pace, bool pureRotation, bool backward) : Action(timeout)
{
    this->posFinal=posFinal;
    this->deltaCurve=deltaCurve;
    this->pace=pace;
    this->pureRotation=pureRotation;
    this->backward=backward;
    switch (pace)
    {
        case accurate: this->speedRamps=1; this->cruisingSpeed=1; break;
        case standard: this->speedRamps=2; this->cruisingSpeed=2; break;
        case fast: this->speedRamps=5; this->cruisingSpeed=3; break;
    }
}

Goto_Action::Goto_Action(float timeout, float x, float y, float theta, float deltaCurve,Pace pace,bool backward)
 : Move_Action(timeout,VectorE(x,y,theta),deltaCurve,pace,false,backward)
 {/*Rien a faire d'autre*/}

 Spin_Action::Spin_Action(float timeout, float theta, Pace pace)
  : Move_Action(timeout,VectorE(1.5,1,theta),0,pace,true,false)  //x et y seront modifié par start
{/*Rien a faire d'autre*/}

void Spin_Action::start()
{
    posFinal._x=robotCinetique->_x;
    posFinal._y=robotCinetique->_y;
    Move_Action::start();
}