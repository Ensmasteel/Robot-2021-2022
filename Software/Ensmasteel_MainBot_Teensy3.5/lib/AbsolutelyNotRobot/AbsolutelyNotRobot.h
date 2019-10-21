#ifndef ABSOLUTELYNOTROBOT_H_
#define ABSOLUTELYNOTROBOT_H_

#include "Vector.h"
#include "Codeuse.h"
#include "Moteur.h"
#include "Ghost.h"
#include "PID.h"
#include "Sequence.h"
#include "Simulator.h"

class AbsolutelyNotRobot
{
private:
    Cinetique cin;
    Odometrie odometrie;
    Simulator simu;
    Motor motorLeft,motorRight;
    Ghost ghost;
    Asservissement asservissement;
    Communication communication;
    Sequence sequence;

    float outTranslation,outRotation;
    Cinetique target;
    bool useSimulator;
public:
    void update(float dt);
    void printCinetique();
    void debug(){sequence.debug();}
    AbsolutelyNotRobot(float x,float y,float theta,bool useSimulator);
    AbsolutelyNotRobot(){}
};

#endif // !ABSOLUTELYNOTROBOT_H_