#include "Actions.h"
#include "Robot.h"
#include "Sequence.h"

//========================================ACTION GENERIQUES========================================
Robot *Action::robot;

void Action::setPointer(Robot *robot_){
    robot=robot_;
}

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

void Double_Action::doAtEnd(){
    action2->doAtEnd();
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
        {
            action1->doAtEnd();
            action2->start();
        }
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
    robot->recalibrateGhost();
    robot->controller.setCurrentProfile(pace);
    int err;
    err = robot->ghost.Compute_Trajectory(posFinal, deltaCurve, speedRamps, cruisingSpeed, pureRotation, backward);
    if (err == 0)
        Logger::debugln("Computation succeeded");
    else
        Logger::infoln("Computation failed");
    Action::start();
}

void Move_Action::doAtEnd()
{
    robot->controller.sendScoreToTelemetry();
    robot->controller.reset();
}

bool Move_Action::isFinished()
{
    return robot->ghost.trajectoryIsFinished() && robot->controller.close;
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
    robot->ghost.Lock(false);
}

Goto_Action::Goto_Action(float timeout, TargetVectorE target, float deltaCurve, Pace pace, bool backward, int16_t require)
    : Move_Action(timeout, target.getVectorE(), deltaCurve, pace, false, backward, "Goto", require)
{ /*Rien a faire d'autre*/
}

Spin_Action::Spin_Action(float timeout, TargetVectorE target, Pace pace, int16_t require)
    : Move_Action(timeout, target.getVectorE() , 0.0, pace, true, false, "Spin", require) //x et y seront modifié par start
{                                                                                    /*Rien a faire d'autre*/
}

void Spin_Action::start()
{
    posFinal._x = robot->cinetiqueCurrent._x;
    posFinal._y = robot->cinetiqueCurrent._y;
    Move_Action::start();
}

Rotate_Action::Rotate_Action(float timeout, float deltaTheta, Pace pace, int16_t require)
    : Move_Action(timeout, VectorE(0.0, 0.0, 0.0), 0.0, pace, true, false, "Rota", require) //x et y et theta seront modifié par start
{
    this->deltaTheta=deltaTheta;
}

void Rotate_Action::start()
{
    posFinal._x = robot->cinetiqueCurrent._x;
    posFinal._y = robot->cinetiqueCurrent._y;
    posFinal._theta = robot->cinetiqueCurrent._theta + deltaTheta;
    Move_Action::start();
}


Forward_Action::Forward_Action(float timeout, float dist, Pace pace, int16_t require)
    : Move_Action(timeout, VectorE(0.0, 0.0, 0.0), 0.0, pace, false, false, "Forward", require)
{
    this->dist = dist;
}

void Forward_Action::start()
{
    posFinal._theta = robot->cinetiqueCurrent._theta;
    posFinal._x = (robot->cinetiqueCurrent._x) + dist * cos(normalizeAngle(posFinal._theta));
    posFinal._y = (robot->cinetiqueCurrent._y) + dist * sin(normalizeAngle(posFinal._theta));
    Move_Action::start();
}

Backward_Action::Backward_Action(float timeout, float dist, Pace pace, int16_t require)
    : Move_Action(timeout, VectorE(0.0, 0.0, 0.0), 0.0, pace, false, true, "Backward", require)
{
    this->dist = dist;
}

void Backward_Action::start()
{
    posFinal._theta = robot->cinetiqueCurrent._theta;
    posFinal._x = (robot->cinetiqueCurrent._x) + dist * cos(-normalizeAngle(posFinal._theta));
    posFinal._y = (robot->cinetiqueCurrent._y) + dist * sin(-normalizeAngle(posFinal._theta));
    Move_Action::start();
}

void StraightTo_Action::start()
{
    //X et Y sont déja miroiré à ce moment. 
    Vector delta = Vector(x, y) - robot->cinetiqueCurrent;
    float cap = delta.angle();
    spin = new Spin_Action(timeout, TargetVectorE(cap,true), pace);  //Donc il faut etre en absolu
    goTo = new Goto_Action(timeout, TargetVectorE(x,y,cap,true), 0.1, pace);
    action1 = spin;
    action2 = goTo;
    Double_Action::start();
}

StraightTo_Action::StraightTo_Action(float timeout, TargetVector target, Pace pace, int16_t require) : Double_Action(timeout, "stTo", require)
{
    Vector targetV = target.getVector();
    this->x = targetV._x;
    this->y = targetV._y;
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
    robot->communication.send(message);
    done = true;
    Action::start();
}

Wait_Message_Action::Wait_Message_Action(MessageID messageId, float timeout, int16_t require) : Action("WaitMess", timeout, require)
{
    this->messageId = messageId;
}

bool Wait_Message_Action::isFinished()
{
    return robot->communication.inWaitingRx() > 0 && extractID(robot->communication.peekOldestMessage()) == messageId;
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
    if (robot->communication.inWaitingRx() > 0)
    {
        for (int i=0;i<size;i++)
        {
            if (extractID(robot->communication.peekOldestMessage()) == onMessage[i])
            {
                doFct[i](robot); //Les functions agissent sur la mainSequence uniquement
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
    functionToCall(robot); //Les functions agissent sur la mainSequence uniquement
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

PauseSeq_Action::PauseSeq_Action(SequenceName nameSeq, int16_t require) : Action("paus",0.1,require){
    this->nameSeq=nameSeq;
}

void PauseSeq_Action::start(){
    robot->getSequenceByName(nameSeq)->pause();
    done=true;
    Action::start();
}

ResumeSeq_Action::ResumeSeq_Action(SequenceName nameSeq, int16_t require) : Action("resu",0.1,require){
    this->nameSeq=nameSeq;
}

void ResumeSeq_Action::start(){
    robot->getSequenceByName(nameSeq)->resume();
    done=true;
    Action::start();
}