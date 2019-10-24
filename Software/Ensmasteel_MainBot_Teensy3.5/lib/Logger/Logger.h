#ifndef LOGGER_H_
#define LOGGER_H_
#include <Print.h>
#include "Arduino.h"

#define nameValue(a) #a+"= "+String(a)

class Logger
{
private:
    static Print* telemetryPort;            //Fait passer des infos formattées pour l'affichage ensuite
    static Print* infoPort;                 //Infos ponctuelles
    static Print* debugPort;                //Conversation cancer

    static bool telemetryEnabled;    
    static bool infoEnabled;
    static bool debugEnabled;

public:
    static void debug(const String& message);                      //Envoie un message sur le debugPort
    static void debugln(const String& message);
    static void infoln(const String& message);
    static void toTelemetry(const String& name, const String& value);           //Envoie un parametre nommé a la telemetry
    static void setup(Print* telemetryPort=&Serial,Print* infoPort=&Serial,Print* debugPort=&Serial,bool telemetry=true,bool info=true,bool debug=false);
};

#endif // !LOGGER_H_