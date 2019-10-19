#ifndef ABSOLUTELYNOTROBOT_H_
#define ABSOLUTELYNOTROBOT_H_

#include "Vector.h"
#include "Codeuse.h"
#include "Moteur.h"
#include "Ghost.h"
#include "PID.h"
#include "Sequence.h"
#ifdef SIMULATOR
#include "Simulator.h"
#endif

class AbsolutelyNotRobot
{
private:
    Cinetique cin;
    #ifndef SIMULATOR
    Odometrie odometrie;
    #else
    Simulator simu;
    #endif
    Motor motorLeft,motorRight;
    Ghost ghost;
    Asservissement asservissement;
    Communication communication;
    Sequence sequence;

    float outTranslation,outRotation;
    Cinetique target;
public:
    void update(float dt);
    void printCinetique();
    AbsolutelyNotRobot(float x,float y,float theta);
    AbsolutelyNotRobot(){}
};

#endif // !ABSOLUTELYNOTROBOT_H_