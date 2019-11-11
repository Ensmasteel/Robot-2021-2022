#include "ErrorManager.h"

#include "Arduino.h"
#include "Logger.h"


#define ERROR_BOX_SIZE 10 //Taille des boites d'envoie et reception


class ErrorBox
{
public:
    Error pull();
    Error peek();
    void push(Error error);
    int size();
    bool empty = true;

private:
    Error box[ERROR_BOX_SIZE];
    uint8_t iFirstEntry = 0;
    uint8_t iNextEntry = 0;
};

ErrorBox* ErrorManager::errorBox;
float* ErrorManager::timeLastIn;

Error ErrorBox::pull()
{
    if (empty)
    {
        Logger::infoln("The errorbox is empty");
        //Dans ce cas on renvoie NO_ERROR
        return NO_ERROR;
    }
    else
    {
        //On recupere l'erreur et on fait avancer le buffer
        Error out = box[iFirstEntry];
        iFirstEntry = (iFirstEntry + 1) % ERROR_BOX_SIZE;
        empty = (iFirstEntry == iNextEntry);
        return out;
    }
}

Error ErrorBox::peek()
{
    if (empty)
    {
        Logger::infoln("The errorbox is empty");
        //Dans ce cas on renvoie NO_ERROR
        return NO_ERROR;
    }
    else
        return box[iFirstEntry];
}

void ErrorBox::push(Error error)
{
    if ((iFirstEntry == iNextEntry) && !empty)
        Logger::infoln("The mailbox is full");
    //Dans ce cas on n'empile pas
    else
    {
        //Sinon on empile et on fait avancer l'indice de la prochaine entrée
        box[iNextEntry] = error;
        iNextEntry = (iNextEntry + 1) % ERROR_BOX_SIZE;
        empty = false;
    }
}

int ErrorBox::size()
{
    if (empty)
        return 0;
    else if (iNextEntry == iFirstEntry)
        return ERROR_BOX_SIZE;
    else
        return (iNextEntry - iFirstEntry + ERROR_BOX_SIZE) % ERROR_BOX_SIZE; //Marche dans tous les cas
}



void ErrorManager::raise(Error error)
{
    if ((millis()/1e3 - timeLastIn[(int)error])>0.1)  //Une meme erreur ne peut rentrer qu'a 100ms d'intervalle
    {
        errorBox->push(error);
        timeLastIn[(int)error]=millis()/1e3;
    }
}

void ErrorManager::popOldestError()
{
    errorBox->pull();
}

Error ErrorManager::peekOldestError()
{
    return errorBox->peek();
}

uint8_t ErrorManager::inWaiting()
{
    return errorBox->size();
}

void ErrorManager::setup(){
    errorBox = new ErrorBox();
    timeLastIn = new float[__NBERROR__];
    for (int i=0;i<__NBERROR__;i++)
        timeLastIn[i]=0;
}




/*
void ErrorManager::toTelemetry()
{
    if (inWaitingRx()>0)
        Logger::toTelemetry("messId",String(extractID(peekOldestError())));
}*/
