#ifndef ABSOLUTELYNOTROBOT_H_
#define ABSOLUTELYNOTROBOT_H_

#include "Vector.h"
#include "Codeuse.h"
#include "Moteur.h"
#include "Ghost.h"
#include "PID.h"
#include "Sequence.h"
#include "Simulator.h"
#include "Communication.h"

class AbsolutelyNotRobot
{
private:
    Cinetique cin;
    Odometrie odometrie;
    Simulator simu;
    Motor motorLeft, motorRight;
    Ghost ghost;
    Asservissement asservissement;
    Communication communication;
    Sequence mainSequence;
    Sequence telecommandeSequence;

    float outTranslation, outRotation;
    Cinetique target;
    bool useSimulator;

public:
    void update(float dt);
    void printCinetique();
    void telemetry();
    AbsolutelyNotRobot(float x, float y, float theta, bool useSimulator, Stream* commPort=&Serial);
    AbsolutelyNotRobot() {}
};

#endif // !ABSOLUTELYNOTROBOT_H_