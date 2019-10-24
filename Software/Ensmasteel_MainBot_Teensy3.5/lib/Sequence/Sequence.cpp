#include "Sequence.h"

void Sequence::startNext()
{
    currentIndex = nextIndex;
    nextIndex = currentIndex + 1;
    queue[currentIndex]->start();
}

void Sequence::reStart()
{
    nextIndex = currentIndex + 1;
    queue[currentIndex]->start();
}

void Sequence::update()
{
    if (queue[currentIndex]->isFinished())
    {
        fails[currentIndex] = false;
        Logger::debugln("Action "+String(currentIndex)+"succeded !");
        startNext();
    }
    else if (queue[currentIndex]->hasFailed())
    {
        fails[currentIndex] = true;
        Logger::infoln("Action "+String(currentIndex)+"failed !");
        startNext();
    }
}

void Sequence::setNextIndex(uint8_t index)
{
    if (index >= TAILLESEQUENCE)
        Logger::infoln("Tried to reach an index out of range");
    else
        nextIndex = index;
}

void Sequence::setGlobal(uint8_t number, uint8_t value)
{
    if (number >= TAILLEGLOBALS)
        Logger::infoln("Tried to reach a global out of range");
    else
        globals[number] = value;
}

uint8_t Sequence::getGlobal(uint8_t number)
{
    if (number >= TAILLEGLOBALS)
    {
        Logger::infoln("Tried to reach a global out of range");
        return 0;
    }
    else
        return globals[number];
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
    lastIndex++;
}

void Sequence::toTelemetry()
{
    Logger::toTelemetry("i",String(currentIndex));
    for (int i = 0; i <= lastIndex; i++)
    {
        Logger::toTelemetry("A"+String(i),queue[i]->name);
    }
}