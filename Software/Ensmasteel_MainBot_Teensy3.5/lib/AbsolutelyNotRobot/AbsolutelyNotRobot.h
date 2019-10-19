#ifndef ABSOLUTELYNOTROBOT_H_
#define ABSOLUTELYNOTROBOT_H_

#include "Vector.h"
#include "Codeuse.h"
#include "Moteur.h"
#include "Ghost.h"
#include "PID.h"
#include "Sequence.h"

class AbsolutelyNotRobot
{
private:
    Cinetique cin;
    Odometrie odometrie;
    Motor motorLeft,motorRight;
    Ghost ghost;
    Asservissement asservissement;
    Communication communication;
    Sequence sequence;

    float outTranslation,outRotation;
    Cinetique target;
public:
    void update(float dt);
    AbsolutelyNotRobot(float x,float y,float theta);
};

#endif // !ABSOLUTELYNOTROBOT_H_