#ifndef ACTIONS_H_
#define ACTIONS_H_
#include "Vector.h"
#include "Arduino.h"
#include "Pace.h"
#include "Communication.h"
#include <vector>
#include <cstdint> //for macro INT16_MAX

class Robot;
class Sequence;
typedef void(*Fct)(Robot * robot);


//========================================ACTION GENERIQUES========================================
#define NO_REQUIREMENT INT16_MAX

class Action //Classe abstraite
{
public:    
    /*
    * Le nom de l'action
    */
    String name;

    /*
    * La fonction start est appellé une fois au début de l'action.
    * Si l'action est interrompu par Sequence::pause, le start sera rappellé lors du resume
    */
    virtual void start();

    /*
    * IsFinished est appelé en boucle. Il renvoie si l'action est terminé
    * Peut aussi servir de fonction d'update...
    */
    virtual bool isFinished() {return done;}

    /*
    * HasFailed est appelé en boucle. Il renvoie si l'action a foiré et devrait etre laissé de coté
    * La Sequence retiendra cepenant que cette fonction a foiré (utile pour les requirements)
    */
    virtual bool hasFailed();
    static void setPointer(Robot * robot);
    Action(String name="Action", float timeout=0.1, int16_t require=NO_REQUIREMENT){this->name=name;this->timeout=timeout;this->require=require;done=false;started=false;}
    bool hasStarted() {return started;}
    virtual void doAtEnd(){/*Ne fait rien par défaut. Il faudra override plus tard*/}  //S'execute en cas de réussite
    int16_t require;        //I require Action n°"require" to have succeeded to do this action

protected:
    bool done;
    bool started;
    float timeout;
    float timeStarted;
    static Robot* robot;
    Sequence * mySequence;  //Les actions n'appartiennent pas tous à la meme séquence
    
friend class Sequence;
};

class Double_Action : public Action
{
protected:
    Action* action1;
    Action* action2;

public:
    virtual void start();
    virtual bool isFinished();
    virtual bool hasFailed();
    void doAtEnd() override;
    Double_Action(float timeout,String name="Twin",int16_t require=NO_REQUIREMENT);
};

//========================================ACTION MOVES========================================

class Move_Action : public Action //Classe abstraite
{
public:
    virtual void start(); //(Action+Move)Dump les parametres dans le ghost et appelle Action::start()
    virtual bool isFinished(); //(Move) Verifie que le ghost est arrive et que le robot est sur le ghost
    virtual bool hasFailed(); //(Action+Move) Verifie que le pid n'a pas retourné d'erreur ou que Action::hasFailed n'est pas true
    void doAtEnd() override;
    Move_Action(float timeout,VectorE posFinal, float deltaCurve, 
                Pace pace, bool pureRotation, bool backward,String name="Move",int16_t require=NO_REQUIREMENT);

protected:
    VectorE posFinal; 
    float deltaCurve,speedRamps,cruisingSpeed;
    Pace pace;
    bool pureRotation, backward;
};

class Goto_Action : public Move_Action //Va a la position demandée (x,y,theta) avec une courbure deltaCurve et un rythme pace. (peut etre effectue en marche arriere)
{
public:
    Goto_Action(float timeout, TargetVectorE target, float deltaCurve,Pace pace,bool backward=false,int16_t require=NO_REQUIREMENT);
    //start (Action+Move)
    //isFinished (Move)
    //hasFailed (Action+Move)
};

class Spin_Action : public Move_Action //Spin
{
public:
    Spin_Action(float timeout, TargetVectorE target, Pace pace,int16_t require=NO_REQUIREMENT);
    void start(); //(Action + Spin) Le start doit etre redéfini car on ne connait pas posFinal a l'avance
    //isFinished(Move)
    //hasFailed(Action+Move)
};

class Rotate_Action : public Move_Action //Tourne en relatif
{
private:
    float deltaTheta;
public:
    Rotate_Action(float timeout, float deltaTheta, Pace pace,int16_t require=NO_REQUIREMENT);
    void start(); //(Action + Spin) Le start doit etre redéfini car on ne connait pas posFinal a l'avance
    //isFinished(Move)
    //hasFailed(Action+Move)
};

class Forward_Action : public Move_Action
{
private:
    float dist;
public:
    Forward_Action(float timeout, float dist, Pace pace,int16_t require=NO_REQUIREMENT);
    void start(); //(Action + Forward)Le start doit etre redéfini car on ne connait pas posFinal a l'avance
    //isFinished(Move)
    //hasFailed(Action+Move)
};

class Backward_Action : public Move_Action
{
private:
    float dist;
public:
    Backward_Action(float timeout, float dist, Pace pace,int16_t require=NO_REQUIREMENT);
    void start(); //(Action + Backward)Le start doit etre redéfini car on ne connait pas posFinal a l'avance
    //isFinished(Move)
    //hasFailed(Action+Move)
};

class StraightTo_Action : public Double_Action
{
private:
    Spin_Action* spin;
    Goto_Action* goTo;
    float x,y;
    Pace pace;
    float timeout;
public:
    void start();
    StraightTo_Action(float timeout, TargetVector target, Pace pace,int16_t require=NO_REQUIREMENT);
};

//========================================ACTION COMM========================================

class Send_Action : public Action
{
private:
    Message message;
public:
    Send_Action(Message message,int16_t require=NO_REQUIREMENT);
    void start(); //(Action+Send)
    //isFinished(Action)
    //hasFailed(Action)
};

class Wait_Message_Action : public Action
{
private:
    MessageID messageId;
public:
    Wait_Message_Action(MessageID messageId, float timeout,int16_t require=NO_REQUIREMENT);
    //start(Action)
    bool isFinished(); //(Wait_Message) verifie que le message est recu
    //hasFailed(Action)
};

class Switch_Message_Action : public Action
{
private:
    std::vector<MessageID>   onMessage;
    std::vector<Fct>         doFct;
    uint8_t size;
public:
    Switch_Message_Action(float timeout,int16_t require);
    void addPair(MessageID messageId,Fct fct);
    //start : inherited from Action
    bool isFinished();
    //has failed : inherited from Action
};

//========================================ACTION MISC========================================

class Sleep_Action : public Action
{
private:    
    float timeToWait;
public:
    Sleep_Action(float timeToWait,int16_t require=NO_REQUIREMENT);
    //start(Action)
    bool isFinished(); //(Sleep) verifie que le temps prévu s'est ecoulé
    bool hasFailed(){return false;} //(Sleep) on en peut pas fail d'attendre
};

class End_Action : public Action //Une End_Action ne passe jamais a la suite
{
private:
    bool loop;
public:
    End_Action(bool loop=false);
    void start();
    bool isFinished(){return done;}
    bool hasFailed(){return false;}
};


class Do_Action : public Action
{
private:
    Fct functionToCall;
public:
    void start();
    Do_Action(Fct functionToCall,int16_t require=NO_REQUIREMENT) :  Action("DoAc", 0.1, require) {this->functionToCall=functionToCall;}
};



#endif // !ACTION_H_