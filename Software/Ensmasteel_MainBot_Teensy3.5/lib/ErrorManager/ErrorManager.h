#ifndef ERRORMANAGER_H
#define ERRORMANAGER_H
#include "Arduino.h"

class ErrorBox;

enum Error
{
    NO_ERROR,
    PID_FAIL_ERROR,
    __NBERROR__
};


class ErrorManager
{
public:
    static void raise(Error error); //Signal l'erreur
    static void popOldestError(); //Supprime la dernière erreur
    static Error peekOldestError(); //Renvoie l'erreur la plus ancienne (sachant qu'a chaque tour, une Erreur est archivée)
    static uint8_t inWaiting(); //Renvoie le nombre de message en attente dans la boite de reception
    static void setup();
//    void toTelemetry();

private:
    static ErrorBox* errorBox;
    static float* timeLastIn;

};
#endif
