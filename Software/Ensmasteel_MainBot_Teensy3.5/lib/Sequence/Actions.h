#ifndef CARDS_H_
#define CARDS_H
#include "Vector.h"
#include "Communication.h"

class Ghost;
class Sequence;

class Action
{
public:    
    void start();
    bool isFinished() {return false;}
    bool hasFailed() {return millis()/1e3 > timeStarted + timeout;}
    static void setPointers(Ghost * ghost, Sequence * sequence, Communication * communication);

protected:
    static float timeout;
    float timeStarted;
    static Ghost * ghost;
    static Sequence * sequence;
    static Communication * communication;
};

enum Pace {accurate,standard,fast};

class Move_Action : public Action
{
public:
    void start();
    bool isFinished();
    bool hasFailed();

protected:
    static float timeout; //Le timeout d'un movement est plus élevé
    VectorE posFinal; 
    float deltaCurve,speedRamps,cruisingSpeed;
    bool pureRotation, backward;
    
    void setPace(Pace pace); 
};

class Goto_Action : public Move_Action
{
public:
    Goto_Action(float x, float y, float theta, float deltaCurve,Pace pace,bool backward=false);
};

class Spin_Action : public Move_Action
{
public:
    Spin_Action(float theta, Pace pace);
    void start(); //Le start doit etre redéfini car on ne connait pas posFinal a l'avance
};

class Forward_Action : public Move_Action
{
public:
    Forward_Action(float dist, Pace pace);
};

class Backward_Action : public Move_Action
{
public:
    Backward_Action(float dist, Pace pace);
};

class Send_Action : public Action
{
private:
    Message message;
public:
    Send_Action(Message message);
    void start();
    bool isFinished();
    bool hasFailed();
};

enum Compar {inf,sup,equal};

class Set_Action : public Action
{
private:
    uint8_t globalIndex; //access the value globals[globalIndex]
    bool relativ; //Set it relatively or globaly
    uint8_t value; //to this value
public:
    
    void start();
    bool isFinished();
    bool hasFailed();
    Set_Action(uint8_t globalIndex, bool relativ, uint8_t value);
};

class Jump_Action : public Action
{
private:
    uint8_t globalIndex; //if the value globals[globalIndex]
    Compar compar; //is (inf|sup|equal)
    uint8_t value; //to this value
    uint8_t whereToJumpTrue; //then jump to this instruction
    uint8_t whereToJumpFalse; //else jump to this one instruction
public:
    void start();
    bool isFinished();
    bool hasFailed();
    Jump_Action(uint8_t globalIndex,Compar compar, uint8_t value, uint8_t whereToJumpTrue, uint8_t whereToJumpFalse);
};

class Wait_Action : public Action
{
private:
    float timeToWait;
    float static timeout;
public:
    void start();
    bool isFinished();
    bool hasFailed();
    Wait_Action(float timeToWait);
};




#endif // !CARDS_H_
