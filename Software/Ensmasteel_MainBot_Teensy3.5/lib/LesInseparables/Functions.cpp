#include "Functions.h"
#include "Actions.h"
#include "Robot.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter" //Retire le warning "unused parameter"

//Lorsque cette fonction est appellé, il y a un PID_tweak dans la mailbox
void PID_tweak(Robot* robot) {
    FourBytes fBytes = extract4Bytes(robot->communication.peekOldestMessage());
    bool incr=fBytes.byte0==1;
    bool translation=fBytes.byte1==1;
    uint8_t whichOne=fBytes.byte2; //0 = P, 1 = I, 2 = D
    Logger::infoln(String(robot->controller.tweak(incr,translation,whichOne)*400.0));
}

void ping(Robot* robot){
    Logger::infoln("ping");
}

void shutdown(Robot* robot)
{
    for (int i=0;i<__NBSEQUENCES__;i++)
        robot->getSequenceByName((SequenceName)i)->pause();
    robot->controller.setCurrentProfile(off);
    Logger::infoln("SHUTDOWN");
}

void setTimeStart(Robot* robot){
    robot->timeStarted = millis()/1e3;
}

void startBackHomeSeq(Robot* robot){
    //On arrete la sequence actuelle
    robot->getSequenceByName(mainSequenceName)->pause();
    //On lance la bonne sequence de retour
    if (robot->endNorth)
        robot->getSequenceByName(goNorthName)->resume();
    else
        robot->getSequenceByName(goSouthName)->resume();
}

void setNorth(Robot* robot){
    robot->endNorth=true;
    Logger::infoln("I will go North");
}

void setSouth(Robot* robot){
    robot->endNorth=false;
    Logger::infoln("I will go South");
}

void recallageBordure(Robot* robot){
    //TODO
    Logger::infoln("reccallage bordure");
}

void forceMainSeqNext(Robot* robot){
    robot->getSequenceByName(mainSequenceName)->forceFollowing();
}

#pragma GCC diagnostic pop