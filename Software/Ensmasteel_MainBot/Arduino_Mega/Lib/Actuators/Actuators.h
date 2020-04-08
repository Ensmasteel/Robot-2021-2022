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

#define MOTOR_STEPS 200
#define RPM 250
#define DIR 35
#define STEP 34
#define SLEEP 22
#define M0 37
#define M1 36

class Pavillon : public Actuator
{
public:
    Pavillon();
    Actuator_State Update() override;
private:
    uint8_t motorSteps = 200;
    uint8_t motorRPM = 200;
    uint8_t pinDir = 35;
    uint8_t pinStep = 34;
    uint8_t pinSleep = 22;
    uint8_t pinM0 = 37;
    uint8_t pinM1 = 36;
    uint16_t actionStep = 200;
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