#ifndef ACTUATORS_H_
#define ACTUATORS_H_

#include "Arduino.h"
#include "Communication.h"
#include "MessageID.h"

enum class Actuator_State
{
    Idle,
    Moving,
    High,
    Low,
    Open,
    Close
};


//// ACTION GENERIQUE ////
//////////////////////////

class Actuator
{
public:
    Actuator(String name = "Actuator");
    virtual void Update(Actuator_State order);
    
    String GetName(){return name;}
    Actuator_State GetEtat(){return etat;}

protected:
    String name;
    Actuator_State etat;
};

class Pavillon : public Actuator
{
public:
    Pavillon();
};

#endif