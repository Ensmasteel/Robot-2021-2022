#include "Sequence.h"
#include "Robot.h"
#include "Actions.h"

void Sequence::startFollowing()
{
    currentIndex = nextIndex;
    nextIndex = currentIndex + 1;
    startSelected();
    
}

void Sequence::startSelected()
{
    nextIndex = currentIndex + 1;
    Serial.println(NO_REQUIREMENT);
    if (queue[currentIndex]->require!=NO_REQUIREMENT)
    {
        //Si l'indice est négatif, on check en relatif. Sinon en absolu
        uint8_t indiceToCheck=(queue[currentIndex]->require>=0)?(queue[currentIndex]->require):(currentIndex + queue[currentIndex]->require);
        if (!fails[indiceToCheck])
        {
            queue[currentIndex]->start();
        }
        else
        {
            fails[currentIndex]=true;
            Logger::infoln(queue[currentIndex]->require);
            Logger::infoln("Action "+ queue[currentIndex]->name + String(currentIndex)+" failed(requirementNotFilled)"+"("+String(getName())+")" /*queue[currentIndex]->timeout + queue[currentIndex]->require*/);
            startFollowing();
        }
    }
    else

        queue[currentIndex]->start();
}

void Sequence::forceFollowing()
{
    fails[currentIndex] = true;
    startFollowing();
}

void Sequence::update()
{
    if (paused)
        return;

    if (queue[currentIndex]->isFinished())
    {
        fails[currentIndex] = false;
        Logger::debugln("Action "+String(currentIndex)+" succeded !");
        if (nextIndex<=lastIndex)
        {
            queue[currentIndex]->doAtEnd();
            startFollowing();
        }
    }
    else if (queue[currentIndex]->hasFailed())
    {
        fails[currentIndex] = true;
        Logger::infoln("Action "+String(currentIndex)+" failed "+"("+String(getName())+")");
        if (nextIndex<=lastIndex)
        {
            queue[currentIndex]->doAtEnd();   //<---------------------- DEBUUUUUUUG
            startFollowing();
        }
    }
}

void Sequence::setNextIndex(uint8_t index)
{
    if (index >= TAILLESEQUENCE)
        Logger::infoln("Tried to reach an index out of range");
    else
        nextIndex = index;
}

Sequence::Sequence(int mySeqIndex)
{
    this->mySeqIndex = mySeqIndex;
    currentIndex = 0;
    nextIndex = 1;
    lastIndex = -1;
    paused=false;
}

SequenceName Sequence::getName()
{
    return (SequenceName)mySeqIndex;
}

void Sequence::add(Action *action)
{
    queue[lastIndex + 1] = action;
    queue[lastIndex + 1]->mySequence=this;
    fails[lastIndex + 1] = false;
    lastIndex++;
}

void Sequence::toTelemetry()
{
    Logger::toTelemetry("i",String(currentIndex));
    for (int i = 0; i <= lastIndex; i++)
    {
        Logger::toTelemetry("A"+String(i),queue[i]->name);
        Logger::toTelemetry("F"+String(i),String(fails[i]));
    }
}

void Sequence::pause(bool lockGhost)
{
    paused=true;
    if (lockGhost)
    {
        Action::robot->controller.setCurrentProfile(brake);
        Action::robot->ghost.Lock(true);
    }
}

void Sequence::resume()
{
    paused=false;
    startSelected(); //On (re)lance l'action
}

void Sequence::reset(bool lockGhost){
    currentIndex = 0;
    nextIndex = 1;
    for (int i=0;i<lastIndex;i++)
        fails[i]=false;
    pause(lockGhost);
}
