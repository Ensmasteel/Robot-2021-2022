#include "Logger.h"

Print* Logger::telemetryPort;            
Print* Logger::infoPort;                 
Print* Logger::debugPort;                

bool Logger::telemetryEnabled;    
bool Logger::infoEnabled;
bool Logger::debugEnabled;

void Logger::debug(const String &message)
{
    if (debugEnabled)
        debugPort->print(message);
}

void Logger::debugln(const String &message)
{
    if (debugEnabled)
        debugPort->println(message);
}

void Logger::toTelemetry(const String &name, const String &value) 
{
    if (telemetryEnabled)
        telemetryPort->print("@"+name+"|"+value+"\n");
}

void Logger::infoln(const String &message)
{
    if (infoEnabled)
        infoPort->print("#"+message+"\n");
}


void Logger::setup(Print* telemetryPort_,Print* infoPort_,Print* debugPort_,bool telemetry,bool info,bool debug)
{
    telemetryPort=telemetryPort_;
    infoPort=infoPort_;
    debugPort=debugPort_;
    telemetryEnabled=telemetry;
    infoEnabled=info;
    debugEnabled=debug;
}