#include "Actions.h"
#include "Ghost.h"
#include "PID.h"
#define DEBUGSEQUENCE

Cinetique *Action::robotCinetique;
Ghost *Action::ghost;
Sequence *Action::sequence;
Communication *Action::communication;
Asservissement *Action::asser;

bool Action::hasFailed()
{
    if (timeout<0)
        return false;
    return millis()/1e3 > timeStarted + timeout;
}

void Action::setPointers(Cinetique *robotCinetique_, Ghost *ghost_, Sequence *sequence_, Communication *communication_, Asservissement *asser_)
{
    robotCinetique = robotCinetique_;
    ghost = ghost_;
    sequence = sequence_;
    communication = communication_;
    asser = asser_;
}

void Move_Action::start()
{
    #ifdef DEBUGSEQUENCE
    Serial.print("My method start has been called, I am a ");Serial.println(name);
    #endif
    switch (pace)
    {
    case accurate:
        asser->setCurrentProfile(1);
        break;
    case standard:
        asser->setCurrentProfile(2);
        break;
    case fast:
        asser->setCurrentProfile(3);
        break;
    default:
        Serial.print("Unmatched PID profile");
    }
    int err;
    err = ghost->Compute_Trajectory(posFinal, deltaCurve, speedRamps, cruisingSpeed, pureRotation, backward);
    #ifdef DEBUGSEQUENCE
    if (err==0)
        Serial.println("Computation succeeded");
    else
        Serial.println("Computation failed");
    #endif
    Action::start();
}

bool Move_Action::isFinished()
{
    Serial.print("GHOST HAS FINISHED");Serial.println(ghost->trajectoryIsFinished());
    return ghost->trajectoryIsFinished() && asser->close;
}

bool Move_Action::hasFailed()
{
    return /*asser->tooFar ||*/ Action::hasFailed();
}

Move_Action::Move_Action(float timeout, VectorE posFinal, float deltaCurve, Pace pace, bool pureRotation, bool backward,String name) : Action(name,timeout)
{
    this->posFinal = posFinal;
    this->deltaCurve = deltaCurve;
    this->pace = pace;
    this->pureRotation = pureRotation;
    this->backward = backward;
    switch (pace)
    {
    case accurate:
        this->speedRamps = 0.5;
        this->cruisingSpeed = 0.1;
        break;
    case standard:
        this->speedRamps = 2;
        this->cruisingSpeed = 0.5;
        break;
    case fast:
        this->speedRamps = 3;
        this->cruisingSpeed = 1;
        break;
    }
}

Goto_Action::Goto_Action(float timeout, float x, float y, float theta, float deltaCurve, Pace pace, bool backward)
    : Move_Action(timeout, VectorE(x, y, theta), deltaCurve, pace, false, backward,"Goto")
{ /*Rien a faire d'autre*/
}

Spin_Action::Spin_Action(float timeout, float theta, Pace pace)
    : Move_Action(timeout, VectorE(0.0, 0.0, theta), 0.0, pace, true, false,"Spin") //x et y seront modifié par start
{                                                                            /*Rien a faire d'autre*/
}

void Spin_Action::start()
{
    posFinal._x = robotCinetique->_x;
    posFinal._y = robotCinetique->_y;
    Move_Action::start();
}


Forward_Action::Forward_Action(float timeout, float dist, Pace pace)
    : Move_Action(timeout, VectorE(0.0, 0.0, 0.0), 0.0, pace, false, false,"Forward")
{
    this->dist=dist;
}

void Forward_Action::start()
{
    posFinal._theta = robotCinetique->_theta;
    posFinal._x = (robotCinetique->_x) + dist * cos(normalizeAngle(posFinal._theta));
    posFinal._y = (robotCinetique->_y) + dist * sin(normalizeAngle(posFinal._theta));
    Move_Action::start();
}

Backward_Action::Backward_Action(float timeout, float dist, Pace pace)
    : Move_Action(timeout, VectorE(0.0, 0.0, 0.0), 0.0, pace, false, true,"Backward")
{
    this->dist=dist;
}

void Backward_Action::start()
{
    posFinal._theta = robotCinetique->_theta;
    posFinal._x = (robotCinetique->_x) + dist * cos( - normalizeAngle(posFinal._theta));
    posFinal._y = (robotCinetique->_y) + dist * sin( - normalizeAngle(posFinal._theta));
    Move_Action::start();
}

End_Action::End_Action() : Move_Action(0.0, VectorE(0, 0, 0), 0.0, accurate, false, false,"End") // x, y, theta initialize in End_Action::start to current position
{                                                                                          /*Rien a faire d'autre*/
}

void End_Action::start()
{
    posFinal._x = robotCinetique->_x;
    posFinal._y = robotCinetique->_y;
    posFinal._theta = robotCinetique->_theta;
    Move_Action::start();
}

Send_Action::Send_Action(Message message) : Action("Send",0.1)
{
    this->message=message;
}

void Send_Action::start()
{
    communication->send(message);
    done=true;
    Action::start();
}

Wait_Message_Action::Wait_Message_Action(MessageID messageId, float timeout) : Action("WaitMess",timeout)
{
    this->messageId=messageId;
}

bool Wait_Message_Action::isFinished()
{
    return communication->inWaiting()>0 && communication->peekOldestMessage().ID==messageId;
}