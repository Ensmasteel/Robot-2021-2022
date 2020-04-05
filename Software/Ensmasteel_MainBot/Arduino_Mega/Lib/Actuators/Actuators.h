#ifndef ACTUATORS_H_
#define ACTUATORS_H_

#include "Arduino.h"
#include "Communication.h"
#include "MessageID.h"
#include "Servo.h"

enum class Actuator_State
{
    Attente,
    NewMess,
    Mouvement,
    MouvFinished
};

//// ACTION GENERIQUE ////
//////////////////////////

class Actuator
{
public:
    Actuator(String name = "Actuator");
    virtual Actuator_State Update();
    void NewOrder(Actuator_Order order);

    String GetName(){return name;}
    Actuator_State GetEtat(){return etat;}
    Actuator_Order GetOrder(){return currentOrder;}

protected:
    String name;
    Actuator_State etat;
    Actuator_Order currentOrder;
};

class Pavillon : public Actuator
{
public:
    Pavillon();
};

class Bras : public Actuator
{
public:
    Bras();
    void Init(uint8_t pinServo, String ID, int posRentre = 0, int posSortie = 100);
    Actuator_State Update();
private:
    int posRentre = 0;
    int posSortie = 100;

    uint8_t pinServo;
    String ID;
    Servo servo;
};

#endif