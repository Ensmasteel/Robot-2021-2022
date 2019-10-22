#include "Sequence.h"
#define DEBUGSEQUENCE

void Sequence::startNext()
{
    currentIndex=nextIndex;
    nextIndex=currentIndex+1;
    #ifdef DEBUGSEQUENCE
    Serial.println("Sequence is calling start method of an Action: ");Serial.println(queue[currentIndex]->name);
    #endif
    queue[currentIndex]->start();
}

void Sequence::reStart()
{
    nextIndex=currentIndex+1;
    queue[currentIndex]->start();
}

void Sequence::update()
{
    if (queue[currentIndex]->isFinished())
    {
        fails[currentIndex]=false;
        #ifdef DEBUGSEQUENCE
        Serial.print("Action ");
        Serial.print(currentIndex);
        Serial.print(" succeded !\n");
        #endif
        startNext();
    }
    else if (queue[currentIndex]->hasFailed())
    {
        fails[currentIndex]=true;
        Serial.print("Action ");
        Serial.print(currentIndex);
        Serial.print(" failed !\n");
        startNext();
    }
}

void Sequence::setNextIndex(uint8_t index)
{
    if (index>=TAILLESEQUENCE)
        Serial.println("Tried to reach an index out of range");
    else
        nextIndex=index;
}

void Sequence::setGlobal(uint8_t number, uint8_t value)
{
    if (number>=TAILLEGLOBALS)
        Serial.println("Tried to reach a global out of range");
    else
        globals[number]=value;
}

uint8_t Sequence::getGlobal(uint8_t number)
{
    if (number>=TAILLEGLOBALS)
    {
        Serial.println("Tried to reach a global out of range");
        return 0;
    }
    else
        return globals[number];
}

Sequence::Sequence()
{
    currentIndex=0;
    nextIndex=1;
    lastIndex=-1;
}

void Sequence::add(Action* action)
{
    queue[lastIndex+1]=action;
    lastIndex++;
}

void Sequence::debug()
{
    #ifdef DEBUGSEQUENCE
    Serial.print("Current index: ");
    Serial.print(currentIndex);Serial.print("\t");
    for (int i=0;i<=lastIndex;i++)
        {Serial.print(queue[i]->name);Serial.print("\t");}
    Serial.println();
    #endif
}