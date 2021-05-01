#ifndef ACTUATORS_H_
#define ACTUATORS_H_

#include "Arduino.h"
#include "Communication.h"
#include "MessageID.h"
#include "Servo.h"
#include "DRV8834.h"

enum class Actuator_State
{
    Attente,
    NewMess,
    Mouvement,
    MouvFinished
};

//// ACTION GENERIQUE ////
/////////////////////////

class Actuator
{
public:
    Actuator(String name = "Actuator", MessageID messID = MessageID::Empty_M);
    
    // A appeler a chaque boucle, commande les actionneurs et mise a jour de l'etat.
    virtual Actuator_State Update();

    // Reception d'un nouvel ordre.
    void NewOrder(Actuator_Order order);

    // Genere le message de validation de l'action dedie au donneur d'ordre
    Message OrderCompleted(){return newMessage(messID,currentOrder,0,0,0);}

    String GetName(){return name;}
    Actuator_State GetEtat(){return etat;}
    Actuator_Order GetOrder(){return currentOrder;}
    MessageID GetID(){return messID;}
protected:
    String name;
    Actuator_State etat;
    Actuator_Order currentOrder;
    MessageID messID = MessageID::Empty_M;
};

//// ACTUATORS ////
//////////////////

class Pavillon : public Actuator
{
public:
    Pavillon();
    void Init(uint8_t pinDir, uint8_t pinStep, uint8_t pinSleep, uint8_t pinM0, uint8_t pinM1);
    Actuator_State Update() override;
private:

    // STEPPER PARAMETERS //
    uint8_t motorSteps = 200;
    float motorRPM = 240;
    uint8_t pinDir = 35;
    uint8_t pinStep = 34;
    uint8_t pinSleep = 22;
    uint8_t pinM0 = 37;
    uint8_t pinM1 = 36;
    long actionStep = 2000;
    DRV8834* stepperMotor;
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
    void Init(uint8_t pinServo, uint8_t pinDir, uint8_t pinStep, uint8_t pinSleep, uint8_t pinM0, uint8_t pinM1, MessageID ID, int ferme = 60, int ouvert = 90);
    Actuator_State Update() override;
private:
    Servo servo;
    DRV8834* stepperMotor;

    uint8_t pinServo;
    uint8_t pinDir;
    uint8_t pinStep;
    uint8_t pinSleep;
    uint8_t pinM0;
    uint8_t pinM1;

    uint8_t motorSteps = 200;
    float motorRPM = 60;
    long actionStep = 400;
    long miniStep = 40;
    int posFermee = 60;
    int posOuverte = 90;
    int posTresOuverte = 110;
};


#endif