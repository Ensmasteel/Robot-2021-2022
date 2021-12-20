
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

enum class Actuator_State
{
    Attente,
    NewMess,
    Mouvement,
    MouvFinished

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
 *
 *
 */
class [[deprecated ("Deprecated because unused after 2021 cup. Will be removed soon")]]
        Pavillon : public Actuator
{
public:
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
};


class Bras : public Actuator
{
public:
    Bras();
    void Init(uint8_t pinServo, MessageID ID, int posRentre = 0, int posSortie = 100);
    Actuator_State Update() override;
private:
    int posRentre = 0;
    int posSortie = 100;

    uint8_t pinServo;
    Servo servo;
};


class Pince : public Actuator
{
public:
    Pince();
    void Init(uint8_t pinServo, uint8_t pinDir, uint8_t pinStep, uint8_t pinSleep, uint8_t pinM0, uint8_t pinM1, MessageID ID, int ferme = 70, int ouvert = 100, int actionStep = 400);
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
};



#endif