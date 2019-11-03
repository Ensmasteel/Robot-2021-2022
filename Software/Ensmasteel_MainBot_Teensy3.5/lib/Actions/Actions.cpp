#include "Actions.h"
#include "Sequence.h"
#include "Ghost.h"
#include "PID.h"


Cinetique *Action::robotCinetique;
Ghost *Action::ghost;
Sequence* Action::mainSequence;
Communication *Action::communication;
Asservissement *Action::asser;

//========================================ACTION GENERIQUES========================================
void Action::start()
{
    timeStarted=millis()/1e3; started=true;
}

bool Action::hasFailed()
{
    if (timeout < 0)
        return false;
    return millis() / 1e3 > timeStarted + timeout;
}

void Action::setPointers(Cinetique *robotCinetique_, Ghost *ghost_, Sequence* mainSequence_, Communication *communication_, Asservissement *asser_)
{
    robotCinetique = robotCinetique_;
    ghost = ghost_;
    mainSequence=mainSequence_;
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

Double_Action::Double_Action(float timeout, String name,int16_t require) : Action(name, timeout, require)
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
        Logger::debugln("Computation succeeded");
    else
        Logger::infoln("Computation failed");
    Action::start();
}

bool Move_Action::isFinished()
{
    return ghost->trajectoryIsFinished() && asser->close;
}

bool Move_Action::hasFailed()
{
    return /*asser->tooFar ||*/ Action::hasFailed();
}

Move_Action::Move_Action(float timeout, VectorE posFinal, float deltaCurve, Pace pace, bool pureRotation, bool backward, String name, int16_t require) : Action(name, timeout, require)
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

Goto_Action::Goto_Action(float timeout, float x, float y, float theta, float deltaCurve, Pace pace, bool backward, int16_t require)
    : Move_Action(timeout, VectorE(x, y, theta), deltaCurve, pace, false, backward, "Goto", require)
{ /*Rien a faire d'autre*/
}

Spin_Action::Spin_Action(float timeout, float theta, Pace pace, int16_t require)
    : Move_Action(timeout, VectorE(0.0, 0.0, theta), 0.0, pace, true, false, "Spin", require) //x et y seront modifié par start
{                                                                                    /*Rien a faire d'autre*/
}

void Spin_Action::start()
{
    posFinal._x = robotCinetique->_x;
    posFinal._y = robotCinetique->_y;
    Move_Action::start();
}

Rotate_Action::Rotate_Action(float timeout, float deltaTheta, Pace pace, int16_t require)
    : Move_Action(timeout, VectorE(0.0, 0.0, 0.0), 0.0, pace, true, false, "Rota", require) //x et y et theta seront modifié par start
{
    this->deltaTheta=deltaTheta;
}

void Rotate_Action::start()
{
    posFinal._x = robotCinetique->_x;
    posFinal._y = robotCinetique->_y;
    posFinal._theta = robotCinetique->_theta + deltaTheta;
    Move_Action::start();
}


Forward_Action::Forward_Action(float timeout, float dist, Pace pace, int16_t require)
    : Move_Action(timeout, VectorE(0.0, 0.0, 0.0), 0.0, pace, false, false, "Forward", require)
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

Backward_Action::Backward_Action(float timeout, float dist, Pace pace, int16_t require)
    : Move_Action(timeout, VectorE(0.0, 0.0, 0.0), 0.0, pace, false, true, "Backward", require)
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

StraightTo_Action::StraightTo_Action(float timeout, float x, float y, Pace pace, int16_t require) : Double_Action(timeout, "stTo", require)
{
    this->x = x;
    this->y = y;
    this->pace = pace;
    this->timeout = timeout;
}

//========================================ACTION COMM========================================

Send_Action::Send_Action(Message message, int16_t require) : Action("Send", 0.1, require)
{
    this->message = message;
}

void Send_Action::start()
{
    communication->send(message);
    done = true;
    Action::start();
}

Wait_Message_Action::Wait_Message_Action(MessageID messageId, float timeout, int16_t require) : Action("WaitMess", timeout, require)
{
    this->messageId = messageId;
}

bool Wait_Message_Action::isFinished()
{
    return communication->inWaiting() > 0 && communication->peekOldestMessage().ID == messageId;
}

Switch_Message_Action::Switch_Message_Action(float timeout,int16_t require) : Action("swch",timeout,require)
{
    this->doFct.clear();
    this->onMessage.clear();
    size=0;
}

void Switch_Message_Action::addPair(MessageID messageId,Fct fct)
{
    this->onMessage.push_back(messageId);
    this->doFct.push_back(fct);
    size++;
}

bool Switch_Message_Action::isFinished()
{
    if (communication->inWaiting() > 0)
    {
        for (int i=0;i<size;i++)
        {
            if (communication->peekOldestMessage().ID == onMessage[i])
            {
                doFct[i](robotCinetique,ghost,mainSequence,communication,asser); //Les functions agissent sur la mainSequence uniquement
                return true;
            }
        }
    }
    return false;
}

//========================================ACTION MISC========================================

End_Action::End_Action(bool loop) : Action("End_", -1, NO_REQUIREMENT)
{
    this->loop=loop;
}

void End_Action::start()
{
    if (loop)
    {
        mySequence->nextIndex=0; //Une end action, boucle sa propre sequence
        done=true;
    }
    Action::start();
}

void Do_Action::start()
{
    functionToCall(robotCinetique,ghost,mainSequence,communication,asser); //Les functions agissent sur la mainSequence uniquement
    done=true;
    Action::start();
}

Sleep_Action::Sleep_Action(float timeToWait,int16_t require) : Action("ZZzz",-1,require)
{
    this->timeToWait=timeToWait;
}

bool Sleep_Action::isFinished()
{
    return millis()/1e3 - timeStarted>timeToWait;
}