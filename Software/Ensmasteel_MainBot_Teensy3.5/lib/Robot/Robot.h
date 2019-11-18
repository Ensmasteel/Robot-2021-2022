#ifndef ROBOT_H_
#define ROBOT_H_

#include "Arduino.h"
#include "Codeuse.h"
#include "Moteur.h"
#include "PID.h"
#include "Ghost.h"
#include "Communication.h"
#include "Sequence.h"
#include "SequenceName.h"
#include <vector>

class Robot
{
protected:
    uint8_t filterFrequency = 20;
    Odometrie odometrie;
    Cinetique cinetiqueNext;
    Motor motorLeft, motorRight;
    float translationOrderPID = 0.0, rotationOrderPID = 0.0;
    uint8_t compteur=0;
    virtual void Update_Cinetique(float dt);
    Sequence** sequences;
    TeamColor teamColor = BLEU;

public :
    //=== Globals ===
    bool endNorth = true;
    float timeStarted;
    float backLength = 0.15; //Longueur entre le contact arrière du robot et son CG
    float frontLength = 0.15; //Longueur entre le contact avant du robot et son CG
    float length = backLength + frontLength; //Longueur du robot
    //===============

    //=== Composants ===
    Cinetique cinetiqueCurrent;
    Ghost ghost;
    Asservissement controller;
    Communication communication;
    //==================
    
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

    Sequence* getSequenceByName(SequenceName name);

    float getTime();

    void setTeamColor(TeamColor teamColor);

    TeamColor getTeamColor();

    void move(VectorE where);
    void move(TargetVectorE whereTarget);

    // GOAL / Teleport Ghost on Robot's position
    void recalibrateGhost();
};

#endif