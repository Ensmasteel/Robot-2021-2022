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
    virtual Actuator_State Update();
    void NewOrder(Actuator_Order order);

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
    long actionStep = 600;
    DRV8834* stepperMotor;

};


class Bras : public Actuator
{
public:
    Bras();
    void Init(uint8_t pinServo, String ID, int posRentre = 0, int posSortie = 100);
    Actuator_State Update() override;
private:
    int posRentre = 0;
    int posSortie = 100;

    uint8_t pinServo;
    String ID;
    Servo servo;
};

#endif