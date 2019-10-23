#include "Actions.h"
#include "Ghost.h"
#include "PID.h"

Cinetique *Action::robotCinetique;
Ghost *Action::ghost;
Sequence *Action::sequence;
Communication *Action::communication;
Asservissement *Action::asser;

//========================================ACTION GENERIQUES========================================

bool Action::hasFailed()
{
    if (timeout < 0)
        return false;
    return millis() / 1e3 > timeStarted + timeout;
}

void Action::setPointers(Cinetique *robotCinetique_, Ghost *ghost_, Sequence *sequence_, Communication *communication_, Asservissement *asser_)
{
    robotCinetique = robotCinetique_;
    ghost = ghost_;
    sequence = sequence_;
    communication = communication_;
    asser = asser_;
}

void Double_Action::start()
{
    action1->start();
    Action::start();
}

bool Double_Action::isFinished()
{
    if (action2->hasStarted())
        return action2->isFinished();
    else //On s'occupe de action1
    {
        if (action1->isFinished()) //Il faut passer à 2
            action2->start();
        return false;
    }
}

bool Double_Action::hasFailed()
{
    if (action2->hasStarted())
        return action2->hasFailed();
    else //On s'occupe de action1
        return action1->hasFailed();
}

Double_Action::Double_Action(float timeout, String name) : Action(name, timeout)
{
    this->action1 = nullptr;
    this->action2 = nullptr;
}

//========================================ACTION MOVES========================================

void Move_Action::start()
{
    asser->setCurrentProfile(pace);
    int err;
    err = ghost->Compute_Trajectory(posFinal, deltaCurve, speedRamps, cruisingSpeed, pureRotation, backward);
    if (err == 0)
        Serial.println("Computation succeeded");
    else
        Serial.println("Computation failed");
    Action::start();
}

bool Move_Action::isFinished()
{
    return ghost->trajectoryIsFinished() && asser->close;
}

void Move_Action::debug()
{
    Action::debug();
    Serial.print("Ghost ");
    Serial.print((ghost->trajectoryIsFinished()) ? ("idle") : ("work"));
    Serial.print(" |PID ");
    Serial.print((asser->close) ? ("idle") : ("work"));
    Serial.print(" |");
}

bool Move_Action::hasFailed()
{
    return /*asser->tooFar ||*/ Action::hasFailed();
}

Move_Action::Move_Action(float timeout, VectorE posFinal, float deltaCurve, Pace pace, bool pureRotation, bool backward, String name) : Action(name, timeout)
{
    this->posFinal = posFinal;
    this->deltaCurve = deltaCurve;
    this->pace = pace;
    this->pureRotation = pureRotation;
    this->backward = backward;
    switch (pace)
    {
    case accurate:
        this->speedRamps = 0.2;
        this->cruisingSpeed = 0.1;
        break;
    case standard:
        this->speedRamps = 1.0;
        this->cruisingSpeed = 0.5;
        break;
    case fast:
        this->speedRamps = 2.0;
        this->cruisingSpeed = 1;
        break;
    case recallage:
        this->speedRamps = 0.2;
        this->cruisingSpeed = 0.1;
        break;
    case off:
        this->speedRamps = 0.01;
        this->cruisingSpeed = 0.01;
        break;
    default:
        this->speedRamps = 0.01;
        this->cruisingSpeed = 0.01;
        break;
    }
    if (pureRotation)
    {
        this->speedRamps *= 3.14;
        this->cruisingSpeed *= 3.14; //Un robot qui avance a 1m/s est aussi impressionnant qu'un robot qui fait un demi tour par seconde
    }
}

Goto_Action::Goto_Action(float timeout, float x, float y, float theta, float deltaCurve, Pace pace, bool backward)
    : Move_Action(timeout, VectorE(x, y, theta), deltaCurve, pace, false, backward, "Goto")
{ /*Rien a faire d'autre*/
}

Spin_Action::Spin_Action(float timeout, float theta, Pace pace)
    : Move_Action(timeout, VectorE(0.0, 0.0, theta), 0.0, pace, true, false, "Spin") //x et y seront modifié par start
{                                                                                    /*Rien a faire d'autre*/
}

void Spin_Action::start()
{
    posFinal._x = robotCinetique->_x;
    posFinal._y = robotCinetique->_y;
    Move_Action::start();
}

Forward_Action::Forward_Action(float timeout, float dist, Pace pace)
    : Move_Action(timeout, VectorE(0.0, 0.0, 0.0), 0.0, pace, false, false, "Forward")
{
    this->dist = dist;
}

void Forward_Action::start()
{
    posFinal._theta = robotCinetique->_theta;
    posFinal._x = (robotCinetique->_x) + dist * cos(normalizeAngle(posFinal._theta));
    posFinal._y = (robotCinetique->_y) + dist * sin(normalizeAngle(posFinal._theta));
    Move_Action::start();
}

Backward_Action::Backward_Action(float timeout, float dist, Pace pace)
    : Move_Action(timeout, VectorE(0.0, 0.0, 0.0), 0.0, pace, false, true, "Backward")
{
    this->dist = dist;
}

void Backward_Action::start()
{
    posFinal._theta = robotCinetique->_theta;
    posFinal._x = (robotCinetique->_x) + dist * cos(-normalizeAngle(posFinal._theta));
    posFinal._y = (robotCinetique->_y) + dist * sin(-normalizeAngle(posFinal._theta));
    Move_Action::start();
}

void StraightTo_Action::start()
{
    Vector delta = Vector(x, y) - *robotCinetique;
    float cap = delta.angle();
    spin = new Spin_Action(timeout, cap, pace);
    goTo = new Goto_Action(timeout, x, y, cap, 0.1, pace);
    action1 = spin;
    action2 = goTo;
    Double_Action::start();
}

StraightTo_Action::StraightTo_Action(float timeout, float x, float y, Pace pace) : Double_Action(timeout, "stTo")
{
    this->x = x;
    this->y = y;
    this->pace = pace;
    this->timeout = timeout;
}

//========================================ACTION COMM========================================

Send_Action::Send_Action(Message message) : Action("Send", 0.1)
{
    this->message = message;
}

void Send_Action::start()
{
    communication->send(message);
    done = true;
    Action::start();
}

Wait_Message_Action::Wait_Message_Action(MessageID messageId, float timeout) : Action("WaitMess", timeout)
{
    this->messageId = messageId;
}

bool Wait_Message_Action::isFinished()
{
    return communication->inWaiting() > 0 && communication->peekOldestMessage().ID == messageId;
}

//========================================ACTION MISC========================================

End_Action::End_Action() : Move_Action(-1, VectorE(0, 0, 0), 0.01, accurate, false, false, "End") // x, y, theta initialize in End_Action::start to current position
{                                                                                                 /*Rien a faire d'autre*/
}

void End_Action::start()
{
    posFinal._x = robotCinetique->_x;
    posFinal._y = robotCinetique->_y;
    posFinal._theta = robotCinetique->_theta;
    Move_Action::start();
}