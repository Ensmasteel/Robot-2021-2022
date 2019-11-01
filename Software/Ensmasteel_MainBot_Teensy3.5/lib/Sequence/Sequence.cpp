#include "Sequence.h"
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
            Logger::infoln("Action "+String(currentIndex)+" failed !");
            startFollowing();
        }
    }
    else
        queue[currentIndex]->start();
}

void Sequence::update()
{
    if (queue[currentIndex]->isFinished())
    {
        fails[currentIndex] = false;
        Logger::debugln("Action "+String(currentIndex)+" succeded !");
        if (nextIndex<=lastIndex)
            startFollowing();
    }
    else if (queue[currentIndex]->hasFailed())
    {
        fails[currentIndex] = true;
        Logger::infoln("Action "+String(currentIndex)+" failed !");
        if (nextIndex<=lastIndex)
            startFollowing();
    }
}

void Sequence::setNextIndex(uint8_t index)
{
    if (index >= TAILLESEQUENCE)
        Logger::infoln("Tried to reach an index out of range");
    else
        nextIndex = index;
}

Sequence::Sequence()
{
    currentIndex = 0;
    nextIndex = 1;
    lastIndex = -1;
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