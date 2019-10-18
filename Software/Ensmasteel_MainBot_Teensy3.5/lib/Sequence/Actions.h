#ifndef CARDS_H_
#define CARDS_H
#include "Vector.h"
#include "Communication.h"

class Ghost;
class Sequence;

class Action //Classe abstraite
{
public:    
    void start() {timeStarted=millis()/1e3;}
    bool isFinished() {return done;}
    bool hasFailed() {return millis()/1e3 > timeStarted + timeout;}
    static void setPointers(Ghost * ghost, Sequence * sequence, Communication * communication);
    Action(float timeout){this->timeout=timeout;}

protected:
    bool done;
    float timeout;
    float timeStarted;
    static Ghost * ghost;
    static Sequence * sequence;
    static Communication * communication;
};

enum Pace {accurate,standard,fast};

class Move_Action : public Action //Classe abstraite
{
public:
    void start(); //(Action+Move)Dump les parametres dans le ghost et appelle Action::start()
    bool isFinished(); //(Move) Verifie que le ghost est arrive et que le robot est sur le ghost
    bool hasFailed(); //(Action+Move) Verifie que le pid n'a pas retourné d'erreur ou que Action::hasFailed n'est pas true
    Move_Action(float timeout,VectorE posFinal, float deltaCurve, 
                Pace pace, bool pureRotation, bool backward);
protected:
    VectorE posFinal; 
    float deltaCurve,speedRamps,cruisingSpeed;
    bool pureRotation, backward;
};

class Goto_Action : public Move_Action //Va a la position demandée (x,y,theta) avec une courbure deltaCurve et un rythme pace. (peut etre effectue en marche arriere)
{
public:
    Goto_Action(float timeout, float x, float y, float theta, float deltaCurve,Pace pace,bool backward=false);
    //start (Action+Move)
    //isFinished (Move)
    //hasFailed (Action+Move)
};

class Spin_Action : public Move_Action //Spin
{
public:
    Spin_Action(float timeout, float theta, Pace pace);
    void start(); //(Action + Spin) Le start doit etre redéfini car on ne connait pas posFinal a l'avance
    //isFinished(Move)
    //hasFailed(Action+Move)
};

class Forward_Action : public Move_Action
{
public:
    Forward_Action(float timeout, float dist, Pace pace);
    void start(); //(Action + Forward)Le start doit etre redéfini car on ne connait pas posFinal a l'avance
    //isFinished(Move)
    //hasFailed(Action+Move)
};

class Backward_Action : public Move_Action
{
public:
    Backward_Action(float timeout, float dist, Pace pace);
    void start(); //(Action + Backward)Le start doit etre redéfini car on ne connait pas posFinal a l'avance
    //isFinished(Move)
    //hasFailed(Action+Move)
};

class Send_Action : public Action
{
private:
    Message message;
public:
    Send_Action(Message message);
    void start(){Action::start(); communication->send(message);} //(Action+Send)
    //isFinished(Action)
    //hasFailed(Action)
};

enum Compar {inf,sup,equal};

class Set_Action : public Action
{
private:
    uint8_t globalIndex; //access the value globals[globalIndex]
    bool relativ; //Set it relatively or globaly
    uint8_t value; //to this value
public:
    Set_Action(uint8_t globalIndex, bool relativ, uint8_t value);
    void start();//(Action+Set) Modifie globals[globalIndex]. Et set done a true
    //isFinished(Action)
    //hasFailed(Action)

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
    void start(); //(Action) set le nextIndex de la sequence
    //isFinished(Action)
    //hasFailed(Action)
    Jump_Action(uint8_t globalIndex,Compar compar, uint8_t value, uint8_t whereToJumpTrue, uint8_t whereToJumpFalse);
};

class Sleep_Action : public Action
{
private:    
    float timeToWait;
public:
    Sleep_Action(float timeToWait);
    //start(Action)
    bool hasFinished(); //(Sleep) verifie que le temps prévu s'est ecoulé
    bool hasFailed(){return false;} //(Sleep) on en peut pas fail d'attendre
};

class Wait_Message_Action : public Action
{
public:
    Wait_Message_Action(Message message, float timeout);
    //start(Action)
    bool hasFinished(); //(Wait_Message) verifie que le message est recu
    //hasFailed(Action)
};



#endif // !CARDS_H_
