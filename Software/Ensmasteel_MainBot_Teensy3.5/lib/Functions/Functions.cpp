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
    Logger::infoln(String(robot->controller.tweak(incr,translation,whichOne)*4095.0));
}

void pauseNlockMainSequence(Robot* robot)
{
    robot->mainSequence.pause(true);
    Logger::infoln("MAIN SEQUENCE PAUSED");
}

void ping(Robot* robot)
{
    Logger::infoln("ping");
}

void shutdown(Robot* robot)
{
    pauseNlockMainSequence(robot);
    robot->controller.setCurrentProfile(Pace::off);
    Logger::infoln("SHUTDOWN");
}

#pragma GCC diagnostic pop