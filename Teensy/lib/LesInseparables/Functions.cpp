#include "Functions.h"
#include "Actions.h"
#include "Robot.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter" //Retire le warning "unused parameter"

//Lorsque cette fonction est appellé, il y a un PID_tweak dans la mailbox
void PID_tweak(Robot *robot)
{
    FourBytes fBytes = extract4Bytes(robot->communication.peekOldestMessage());
    bool incr = fBytes.byte0 == 1;
    bool translation = fBytes.byte1 == 1;
    uint8_t whichOne = fBytes.byte2; //0 = P, 1 = I, 2 = D
    Logger::infoln(String(robot->controller.tweak(incr, translation, whichOne)));
}

void ping(Robot *robot)
{
    Logger::infoln("ping");
}

void shutdown(Robot *robot)
{
    for (int i = 0; i < __NBSEQUENCES__; i++)
        robot->getSequenceByName((SequenceName)i)->pause(true);
    robot->controller.setCurrentProfile(off);
    Logger::infoln("SHUTDOWN");
}

void setTimeStart(Robot *robot)
{
    robot->timeStarted = millis() / 1e3;
}

void startBackHomeSeq(Robot *robot)
{
    //On arrete la sequence actuelle
    robot->getSequenceByName(mainSequenceName)->pause(true);
    //On lance la bonne sequence de retour
    if (robot->endNorth)
        robot->getSequenceByName(goNorthName)->resume();
    else
        robot->getSequenceByName(goSouthName)->resume();
}

void setNorth(Robot *robot)
{
    robot->endNorth = true;
    Logger::infoln("I will go North");
}

void setSouth(Robot *robot)
{
    robot->endNorth = false;
    Logger::infoln("I will go South");
}

void recallageBordure(Robot *robot)
{
    Logger::infoln("recallage bordure");
    if (abs(normalizeAngle(robot->cinetiqueCurrent._theta - 0.0)) < DEG_TO_RAD * 45) //On fait un recallage X en regardant vers theta==0
    {
        if (abs(robot->cinetiqueCurrent._x - robot->backLength) < 0.40)              // C'est un recallage arrière
            robot->move(VectorE(robot->backLength, robot->cinetiqueCurrent._y, 0.0));
        else if (abs(robot->cinetiqueCurrent._x - (3.0 - robot->frontLength)) < 0.40) //C'est un recallage avant
            robot->move(VectorE(3.0 - robot->frontLength, robot->cinetiqueCurrent._y, 0.0));
        else
            Logger::infoln("ERREUR RECALLAGE x look east");
    }
    else if (abs(normalizeAngle(robot->cinetiqueCurrent._theta - PI / 2.0)) < DEG_TO_RAD * 45) //On fait un recallage Y en regardant vers theta==PI/2
    {
        if (abs(robot->cinetiqueCurrent._y - robot->backLength) < 0.40)                        // C'est un recallage arrière
            robot->move(VectorE(robot->cinetiqueCurrent._x, robot->backLength, PI / 2.0));
        else if (abs(robot->cinetiqueCurrent._y - (2.0 - robot->frontLength)) < 0.40) //C'est un recallage avant
            robot->move(VectorE(robot->cinetiqueCurrent._x, 2.0 - robot->frontLength, PI / 2.0));
        else
            Logger::infoln("ERREUR RECALLAGE y look north");
    }
    else if (abs(normalizeAngle(robot->cinetiqueCurrent._theta - PI)) < DEG_TO_RAD * 45) //On fait un recallage X en regardant vers theta==PI
    {
        if (abs(robot->cinetiqueCurrent._x - robot->frontLength) < 0.40)                 // C'est un recallage avant
            robot->move(VectorE(robot->frontLength, robot->cinetiqueCurrent._y, PI));
        else if (abs(robot->cinetiqueCurrent._x - (3.0 - robot->backLength)) < 0.40) //C'est un recallage arriere
            robot->move(VectorE(3.0 - robot->backLength, robot->cinetiqueCurrent._y, PI));
        else
            Logger::infoln("ERREUR RECALLAGE x look west");
    }
    else if (abs(normalizeAngle(robot->cinetiqueCurrent._theta - (-PI / 2.0))) < DEG_TO_RAD * 45) //On fait un recallage Y en regardant vers theta== -PI/2
    {
        if (abs(robot->cinetiqueCurrent._y - robot->frontLength) < 0.40)                          // C'est un recallage avant
            robot->move(VectorE(robot->cinetiqueCurrent._x, robot->frontLength, -PI / 2.0));
        else if (abs(robot->cinetiqueCurrent._y - (2.0 - robot->backLength)) < 0.40) //C'est un recallage arriere
            robot->move(VectorE(robot->cinetiqueCurrent._x, 2.0 - robot->backLength, -PI / 2.0));
        else
            Logger::infoln("ERREUR RECALLAGE y look south");
    }   
    else
        Logger::infoln("ERREUR RECALLAGE bad angle");
}

void forceMainSeqNext(Robot *robot)
{
    robot->getSequenceByName(mainSequenceName)->forceFollowing();
}

void startTimeSeq(Robot* robot) {
    robot->getSequenceByName(timeSequenceName)->resume();
}

#pragma GCC diagnostic pop