
/**
 * This class is used to write debug or info messages.
 *
 * To write a debug message use :
 * ''Logger::debug(String message)''
 *
 * To write an info message use :
 * ''Logger::infoln(String infoMessage)
 *
 * To add a variable to telemetry use :
 * ''Logger::toTelemetry(String message, String(variable to get in telemetry))''
 *
 * Returns :
 * Between # and EndLine, you'll have an informative message
 * Between @ and | you'll have a parameter's name
 * Between | and EndLine, you'll have its value
 * Everything before @ or # will be debugging information.
 */

#ifndef LOGGER_H_
#define LOGGER_H_
#include <Print.h>
#include "Arduino.h"

#define nameValue(a) #a+"= "+String(a)

/**
 * Defines the Logger class that will enables you to write messages.
 */
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
    /**
     * Print a message on debugPort. Works same as System.out.print in Java or std::cout<<message in C++.
     * @param message : String containing the message you want to print.
     */
    static void debug(const String& message);

    //Affiche un message d'une pertinance approximative sur le debugPort et passe a la ligne
    /**
     * Same as debug, but add an EndLine character. Works same as System.out.println in Java.
     * @param message : String, the message you want to print.
     */
    static void debugln(const String& message);

    //Affiche un message important sur le infoPort et passe a la ligne (obligatoire)
    /**
     * Print an important message on infoPort and EndLine (mandatory).
     * @param message : String, The message you want to print on infoPort.
     */
    static void infoln(const String& message);

    //Envoie un parametre nommé a la telemetry
    /**
     * Send a named parameter to telemetry.
     * @param name : String, the parameter's name.
     * @param value : String, the parameter's value, converted as a String.
     */
    static void toTelemetry(const String& name, const String& value);           

    //Attention, les port doivent déja être ouvert avant d'etre passé en argument de cette fonction
    /**
     * Set up all the necessary information to use the other functions. The port have to be initialized before
     * being passed in argument of this function. Must be used before any other function in this module.
     * @param telemetryPort : The telemetry port.
     * @param infoPort : The info Port
     * @param debugPort : The debug Port
     * @param telemetry : bool, Telemetry enabled? Default is true.
     * @param info : bool, infoPort enabled? Default is true.
     * @param debug : bool, debugPort enabled? Default is false.
     */
    static void setup(Print* telemetryPort=&Serial,Print* infoPort=&Serial,Print* debugPort=&Serial,bool telemetry=true,bool info=true,bool debug=false);
};

#endif // !LOGGER_H_