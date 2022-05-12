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

/**
 * Defines a robot class, including all parameters to set it up.
 */
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

    Stream *espPort;
    int rangeAdversaryFoward = 500;
    int rangeAdversaryBackward = 500;
    String readString;
    bool stopped = false;
    //===============

    //=== Composants ===
    Cinetique cinetiqueCurrent;
    Ghost ghost;
    Asservissement controller;
    Communication communication;
    Communication commActionneurs;
    //==================
    
    // GOAL / Constructor : Setup all reference to variables of other classes
    //        Definition of the sequence of actions
    // IN   / float xIni, yIni, thetaIni : Initial position of the bot
    //        Stream* commPort : pointer to current serial port (bluetooth or USB)
    /**
     * Constructor of the robot class. Set up all references to variables of other classes.
     * Definition of all the sequence of the actions.
     * @param xIni : float, initial X-axis coordinates of the robot.
     * @param yIni : float, initial Y-axis coordinates of the robot.
     * @param thetaIni : float, initial angle of the robot.
     * @param commPort : pointer to current communication port.
     * @param actuPort : pointer to the actuator port.
     */
    Robot(float xIni = 0.0,
          float yIni = 0.0,
          float thetaIni = 0.0,
          Stream *commPort = &Serial,
          Stream *actuPort = &Serial,
          Stream *espPort = &Serial4);
    
    /**
     * Updates information about real position, ghost's position, compute output and send orders to motors.
     * @param dt : time since last call.
     */
    void Update(float dt);

    // GOAL / Send current robot state on telemtry serial
    // IN   / cinetiqueCurrent
    //        odometrie
    //        ghost
    //        mainSequence
    //        controller
    /**
     * Send current robot state on telemetry serial.
     * @param odometrie : bool.
     * @param other : bool.
     */
    void telemetry(bool odometrie, bool other);

    /**
     * Return sequence named 'name'
     * @param name : String, name of the wanted sequence.
     * @return Sequence, the sequences searched.
     */
    Sequence* getSequenceByName(SequenceName name);

    float getTime();

    void setTeamColor(TeamColor teamColor);

    TeamColor getTeamColor();

    void move(VectorE where);
    void move(TargetVectorE whereTarget);

    // GOAL / Teleport Ghost on Robot's position
    /**
     * Teleports Ghost on Robot's actual position.
     */
    void recalibrateGhost();
};

#endif