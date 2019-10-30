#ifndef ROBOT_
#define ROBOT_

#include "Arduino.h"
#include "Vector.h"
#include "Codeuse.h"
#include "Moteur.h"
#include "PID.h"
#include "Communication.h"
#include "Sequence.h"
#include "Ghost.h"
#include "Actions.h"
#include "Functions.h"

class Robot
{
public:
    // GOAL / Constructor : Setup all reference to variables of other classes
    //        Definition of the sequence of actions
    // IN   / float xIni, yIni, thetaIni : Initial position of the bot
    //        Stream* commPort : pointer to current serial port (bluetooth or USB)
    Robot(float xIni = 0.0, float yIni = 0.0, float thetaIni = 0.0, Stream *commPort = &Serial);
    
    // GOAL / Update informations about real position, Ghost's position, compute output and send order to motors
    // IN   / float dt : time since last call
    //        odometrie
    //        ghost
    //        cinetiqueNext
    //        cinetiqueCurrent
    //        controller
    // OUT  / Motor motorLeft, motorRight : orders send
    void Update(float dt);

    // GOAL / Send current robot state on telemtry serial
    // IN   / cinetiqueCurrent
    //        odometrie
    //        ghost
    //        mainSequence
    //        controller
    void telemetry();

private:
    uint8_t controllerFrequency = 30;
    Odometrie odometrie;
    Cinetique cinetiqueCurrent, cinetiqueNext;
    Motor motorLeft, motorRight;
    Ghost ghost;
    Asservissement controller;
    Communication communication;
    Sequence mainSequence, communicationSequence;

    float translationOrderPID = 0.0, rotationOrderPID = 0.0;
};

#endif