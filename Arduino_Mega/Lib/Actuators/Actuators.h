
// Copyright (c) 2021.
// This code has been written by student members of ISAE-ENSMA Ensmasteel association.
/*
 * This file provides a generic implementation of robotics actuators in place in the
 * Ensmasteel robot. This module may be re-written if there is any changes in the
 * general structure of the robot or in the general code architecture.
 */

#ifndef ACTUATORS_H_
#define ACTUATORS_H_

#include "Arduino.h"
#include "Communication.h"
#include "MessageID.h"
#include "Servo.h"
#include "StepperMotorJ.h"

/**
 * Enum class to give the different states of an Actuator.
 */
enum class Actuator_State
{
    Attente, ///< Waiting for an order
    NewMess, ///< getting a new order
    Mouvement, ///< During the movement
    MouvFinished ///< Just ending the action asked

};

//// ACTION GENERIQUE ////
/////////////////////////

/**
 * Basic abstract class to define an actuator
 *
 * This class defines a basic class to represent an actuator. It should be decorated with the target actuator
 * class.
 * The constructor is public and embeddes a name and an ID wich is 'Empty_M' as default.
 *
 */
class Actuator
{
public:
    /**
     * Basic public constructor for Actuator class
     * @param name : the actuator name
     * @param messID : The Id of the message embedded. Default is 'Empty_M'
     * @returns Actuator : An instance of Actuator.
     *
     */
    Actuator(String name = "Actuator", MessageID messID = MessageID::Empty_M);
    
    // A appeler a chaque boucle, commande les actionneurs et mise a jour de l'etat.
    /**
     * Virtual method, to call before each loop.
     *
     * Commands each actuator and updates the robot state.
     *
     */
    virtual Actuator_State Update();

    // Reception d'un nouvel ordre.
    /**
     * Getter of a new order.
     * @param order
     */
    void NewOrder(Actuator_Order order);

    // Genere le message de validation de l'action dedie au donneur d'ordre
    /**
     * Generates the validating message from the action dedicated to the order sender.
     * @return the validation message
     */
    Message OrderCompleted(){return newMessage(messID,currentOrder,0,0,0);}

    /**
     * Returns the actuator name
     * @return name : the actuator name
     */
    String GetName(){return name;}

    /**
     * Returns the actuator state
     * @return state : the current state
     */
    Actuator_State GetEtat(){return etat;}

    /**
     * Returns the current order the actuator has.
     * @return current Order : the current order.
     */
    Actuator_Order GetOrder(){return currentOrder;}

    /**
     * Returns the message ID of the current message actually in the actuator.
     * @return messID
     */
    MessageID GetID(){return messID;}

protected:
    String name;
    Actuator_State etat;
    Actuator_Order currentOrder;
    MessageID messID = MessageID::Empty_M;
};

//// ACTUATORS ////
//////////////////

/**
 * Public class extending the actuator abstract class.
 *
 * Used to define the 'Pavillon' actuator.
 *Deprecated for the 2022 edition.
 *
 *
class [[deprecated ("Deprecated because unused after 2021 cup. Will be removed soon")]]
        Pavillon : public Actuator
{
public:
    /**
     * Public constructor of the Pavillon class.
     *
    Pavillon();
    void Init(uint8_t pinDir, uint8_t pinStep, uint8_t pinSleep, uint8_t pinM0, uint8_t pinM1);
    Actuator_State Update() override;
private:

    // STEPPER PARAMETERS //
    uint8_t motorSteps = 200;
    long actionStep = 2000;
    uint8_t pinDir = 35;
    uint8_t pinStep = 34;
    uint8_t pinSleep = 22;
    uint8_t pinM0 = 37;
    uint8_t pinM1 = 36;
    StepperMotorJ* stepperMotor;
};/

/**
 * Public class that extends the abstract actuator class.
 *
 * Enables you to create a class containting the information to control a robot arm equipped with
 * a servomotor and 2 positions : the initial pos and the deployed position. Both are given in degrees.
 */



class PositionBras 
{
public:
    PositionBras();
    /**
    @param posServo1 //angle du servo 1
    @param posServo2 //angle du servo 2
    @param posServo3 //angle du servo 3
    */
   PositionBras Init(int posServo1, int posServo2, int posServo3);

   PositionBras &PositionBras::operator=(const PositionBras &source);

   int getPosServo1();
   int getPosServo2();
   int getPosServo3();


   protected:
    int posServo1;
    int posServo2;
    int posServo3;
};

class Tourelle : public Actuator 
{
public:
    /**
     * Public basic constructor. Does not require any parameters.
     */
    Tourelle();

    /**
    @param pinStep 
    @param pinDir 
    @param pinM0 
    @param pinM1 
    */
   void Init(uint8_t pinStep, uint8_t pinDir, uint8_t pinM0, uint8_t pinM1,MessageID ID);
   Actuator_State Update() override;

   protected:
    uint8_t motorSteps = 200;
    long actionStep = 2000;
    StepperMotorJ* stepperMotor;
    uint8_t pinDir;
    uint8_t pinStep;
    uint8_t pinM0;
    uint8_t pinM1;
};

class Pompe : public Actuator
{
public: 

    Pompe();

    void Init(uint8_t pinTinterrupt,MessageID ID);
    Actuator_State Update() override;
    uint8_t getState();

private:
    uint8_t pinInterrupt;
    MessageID ID;

};

class Bras : public Actuator
{
public:
    /**
     * Public basic constructor. Does not require any parameters.
     */
    Bras();

    /**
     * Initialises the robot arm with some parameters.
     *
     * Enables you to give the initial parameters to the arm previously instantiated.
     * @param pinServo : the pin number on wich the servomotor is connected.
     * @param ID : The message ID of the default state of the arm
     * @param posRentre The angle given in degree of the initial position.
     * @param posSortie The angle given in degree of the final position.
     */
    void Init(uint8_t pinServo1, uint8_t pinServo2, uint8_t pinServo3, MessageID ID);
    Actuator_State Update() override;

private:
    PositionBras posRepos;
    PositionBras posStockagePalet;
    PositionBras posPaletSol;
    PositionBras posPaletDistributeur;
    PositionBras posPaletStatuette;
    PositionBras posRamassageStatuette;
    PositionBras posDepotStatuette;
    PositionBras posStockageStatuette;
    PositionBras posDepotReplique;
    PositionBras posEchange;
    PositionBras posDepotPaletGallerieB;
    PositionBras posDepotPaletGallerieH;
    PositionBras posIntermediaire;
    PositionBras posStart;

    MessageID ID;

    uint8_t pinServo1;
    uint8_t pinServo2;
    uint8_t pinServo3;

    Servo servo1;
    Servo servo2;
    Servo servo3;

};

/**
 * The public class to construct a robot gripper. Extends the abstract class Actuator.
 *
 * This class is deprecated in the 2022 edition.
 */
/*class [[deprecated ("Deprecated in the 2022 edition")]] Pince : public Actuator
{
public:
    /**
     * Basic public constructor with no params.
     
    Pince();

    /**
     * Initializing function. Enables you to give the primary parameters to the pince.
     * @param pinServo : the pin number on wich the gripper is connected.
     * @param pinDir : the pin number on wich the direction is given to the stepperMotor.
     * @param pinStep : the pin number giving the step information to the stepperMotor
     * @param pinSleep : the pin number giving the sleep information to the stepperMotor
     * @param pinM0 : the pin number of the first motor of the stepperMotor.
     * @param pinM1 : the second pin number of the stepperMotor.
     * @param ID : Default message ID of the stepperMotor
     * @param ferme : The initial position given in degrees.
     * @param ouvert : The final position given in degrees.
     * @param actionStep : The step of the stepperMotor.
     
    void Init(uint8_t pinServo, uint8_t pinDir, uint8_t pinStep, uint8_t pinSleep, uint8_t pinM0, uint8_t pinM1,
              MessageID ID, int ferme = 70, int ouvert = 100, int actionStep = 400);

    Actuator_State Update() override;
protected:
    Servo servo;
    StepperMotorJ* stepperMotor;

    uint8_t pinServo;
    uint8_t pinDir;
    uint8_t pinStep;
    uint8_t pinSleep;
    uint8_t pinM0;
    uint8_t pinM1;

    uint8_t motorSteps = 200;
    long actionStep =  400; //200 pince arr
    int posFermee = 70;
    int posOuverte = 100;
    int standardDelay = 2000;
};

class PinceAvant : public Pince
{
public:
    Actuator_State Update() override;
private:

    long miniStep = 40;
    int posTresOuverte = 110;
};

class PinceArriere : public Pince
{
public:
    Actuator_State Update() override;
};*/



#endif