/*
Si tu veux faire un message de debug tu fais juste
Logger::debug("I'm in")

Pour faire un message d'info
Logger::infoln("That's what she said")

Et pour rajouter une variable a la telemetry
Logger::toTelemetry("dickSize",String(dick->size))

En sortie:
Entre un # et un \n tu as un message informatif
Entre un @ et un | tu as un nom de parametre
Entre un | et un \n tu as sa valeur
Tout ce qui est avant un @ ou un # est du debug
*/


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
    //Affiche un message d'une pertinance approximative sur le debugPort
    static void debug(const String& message);

    //Affiche un message d'une pertinance approximative sur le debugPort et passe a la ligne
    static void debugln(const String& message);

    //Affiche un message important sur le infoPort et passe a la ligne (obligatoire)
    static void infoln(const String& message);

    //Envoie un parametre nommé a la telemetry
    static void toTelemetry(const String& name, const String& value);           

    //Attention, les port doivent déja être ouvert avant d'etre passé en argument de cette fonction
    static void setup(Print* telemetryPort=&Serial,Print* infoPort=&Serial,Print* debugPort=&Serial,bool telemetry=true,bool info=true,bool debug=false);
};

#endif // !LOGGER_H_