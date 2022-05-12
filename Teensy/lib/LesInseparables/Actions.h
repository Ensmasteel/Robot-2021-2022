#ifndef ACTIONS_H_
#define ACTIONS_H_
#include "Vector.h"
#include "Arduino.h"
#include "MoveProfile.h"
#include "Communication.h"
#include "SequenceName.h"
#include "ErrorManager.h"
#include <vector>
#include <cstdint> //for macro INT16_MAX

/**
 * Defines the generic actions for the robot.
 */

class Robot;
class Sequence;
typedef void (*Fct)(Robot *robot);

//========================================ACTION GENERIQUES========================================
#define NO_REQUIREMENT INT16_MAX

/**
 * ABSTRACT CLASS. DO NOT INSTANCE.
*/
class Action
{
public:
    /**
     *Action's name.
     */
    String name;

    /**
     * The start function is called once at action's beginning, if the action requirement did not fail.
     * Else the fail is immediate.
     * If the action is interrupted by Sequence::pause, start function will be re-called during resume.
     */
    virtual void start();

    /**
     * IsFinished is called endlessly. Returns if the action is done.
     * Can also be an update function.
     */
    virtual bool isFinished() { return done; }

    /**
     * HasFailed is called endlessly. Returns if the action failed and should be left aside.
     * The sequence will remember the function failed (useful for requirement).
     * By default, an action fails if it exceeds the time duration allowed by the timer.
     * @note A timeout set to -1 indicates the action has an unlimited time.
     */
    virtual bool hasFailed();

    /**
     *Gives to all current and future actions a pointer to the robot.
     */
    static void setPointer(Robot *robot);

    /**
     * Creates a basic action.
     * @warning This class is Abstract and cannot be instantiated.
     */
    Action(String name = "Action", float timeout = 0.1, int16_t require = NO_REQUIREMENT)
    {
        this->name = name;
        this->timeout = timeout;
        this->require = require;
        done = false;
        started = false;
    }

    /**
     * Returns whether the actions started or not.
     */
    bool hasStarted() { return started; }

    /**
     * This function is called in case of success. Doesn't do anything by default.
     */
    virtual void doAtEnd()
    { /*Ne fait rien par défaut. Il faudra override plus tard*/
    }

protected:
    bool done;
    bool started;
    float timeout;
    float timeStarted;
    static Robot *robot;
    Sequence *mySequence;
    int16_t require;

    friend class Sequence;
};

/**
 * This class instantiates a double action. A double action is a sequence of two consecutive actions.
 * The handle of those actions is encapsulated. There is nothing to do in particular.
 * A double action is finished if both actions are finished.
 * A double action fails if at least one of them fails.
 */
class Double_Action : public Action
{
protected:
    Action *action1;
    Action *action2;

public:
    virtual void start();
    virtual bool isFinished();
    virtual bool hasFailed();
    void doAtEnd() override;
    Double_Action(float timeout, String name = "Twin", int16_t require = NO_REQUIREMENT);
};

//========================================ACTION MOVES========================================

/**
 * A MoveAction will give a new order to the ghost.
 * When start is called, the target position is given to ghost which will then go to it. (so the ghost is unlocked if necessary)
 * By default PIDs are reset at the end of action.
 * @warning This class is abstract and cannot be instantiated.
 */
class Move_Action : public Action //Classe abstraite
{
public:
    virtual void start();      //(Action+Move) Dump les parametres dans le ghost et appelle Action::start() et debloque le ghost
    virtual bool isFinished(); //(Move) Verifie que le ghost est arrivé et que le robot est sur le ghost
    virtual bool hasFailed();  //(Action+Move) Verifie que le pid n'a pas retourné d'erreur ou que Action::hasFailed n'est pas true
    void doAtEnd() override;
    Move_Action(float timeout, VectorE posFinal, float deltaCurve,
                MoveProfileName profileName, bool pureRotation, bool backward, String name = "Move", int16_t require = NO_REQUIREMENT);

protected:
    VectorE posFinal;
    float deltaCurve;
    MoveProfileName profileName;
    bool pureRotation, backward;
};

/**
 * Send the ghost to the target position (x,y,theta) with a curve deltaCurve and a rhythm pace.
 * This action can be done backward.
 * @warning this action is team dependant.
 * @note deprecated for 2022 edition. Has to be updated with new actions.
 */
class Goto_Action : public Move_Action
{
public:
    Goto_Action(float timeout,
                TargetVectorE target,
                float deltaCurve,
                MoveProfileName profileName,
                bool backward = false,
                int16_t require = NO_REQUIREMENT);
    //start (Action+Move)
    //isFinished (Move)
    //hasFailed (Action+Move)
};

/**
 * Rotates on place to get to target position.
 * @warning this function is team dependant.
 * @note Deprecated for 2022 edition. Has to be updated.
 */
class Spin_Action : public Move_Action
{
public:
    Spin_Action(float timeout, TargetVectorE target, MoveProfileName profileName, int16_t require = NO_REQUIREMENT);
    void start(); //(Action + Spin) Le start doit etre redéfini car on ne connait pas posFinal a l'avance
    //isFinished(Move)
    //hasFailed(Action+Move)
};

/**
 * Rotates on place of a certain angle theta.
 * @note Not team dependant.
 */
class Rotate_Action : public Move_Action //Tourne en relatif
{
private:
    float deltaTheta;

public:
    Rotate_Action(float timeout, float deltaTheta, MoveProfileName profileName, int16_t require = NO_REQUIREMENT);
    void start(); //(Action + Spin) Le start doit etre redéfini car on ne connait pas posFinal a l'avance
    //isFinished(Move)
    //hasFailed(Action+Move)
};

/**
 * Goes straight ahead for a certain distance dist.
 * @note Not team dependant.
 */
class Forward_Action : public Move_Action
{
private:
    float dist;

public:
    Forward_Action(float timeout, float dist, MoveProfileName profileName, int16_t require = NO_REQUIREMENT);
    void start(); //(Action + Forward)Le start doit etre redéfini car on ne connait pas posFinal a l'avance
    //isFinished(Move)
    //hasFailed(Action+Move)
};

/**
 * Same as forward_action but doing it backward.
 * @see Forward_Action for further informations.
 */
class Backward_Action : public Move_Action
{
private:
    float dist;

public:
    Backward_Action(float timeout, float dist, MoveProfileName profileName, int16_t require = NO_REQUIREMENT);
    void start(); //(Action + Backward)Le start doit etre redéfini car on ne connait pas posFinal a l'avance
    bool isFinished();
    bool hasFailed();
    //hasFailed(Action+Move)
};

/**
 * Goes to target and does a spin and a straight line before.
 * @warning team dependant!
*/
class StraightTo_Action : public Double_Action
{
private:
    Spin_Action *spin;
    Goto_Action *goTo;
    float x, y;
    MoveProfileName profileName;
    float timeout;

public:
    void start();
    StraightTo_Action(float timeout, TargetVector target, MoveProfileName profileName, int16_t require = NO_REQUIREMENT);
};

/**
 * Brakes robot until its rotation speed goes below dEpsilon given in brake profile.
 * @see MoveProfile.cpp ->setup
 */
class Brake_Action : public Move_Action
{
public:
    Brake_Action(float timeout, int16_t require = NO_REQUIREMENT);
    void start();
};

//========================================ACTION COMM========================================

/**
 * Send an action message on communication port.
 */
class Send_Action : public Action
{
private:
    Message message;
    Communication* _commLocal;

public:
    Send_Action(Message message, Communication* comm, int16_t require = NO_REQUIREMENT);
    void start(); //(Action+Send)
    //isFinished(Action)
    //hasFailed(Action)
};

/**
 * Blocking instruction : waits for a message on communication port.
 */
class Wait_Message_Action : public Action
{
private:
    MessageID messageId;
    Communication* _commLocal;

public:
    Wait_Message_Action(MessageID messageId, float timeout, Communication* comm, int16_t require = NO_REQUIREMENT);
    //start(Action)
    bool isFinished(); //(Wait_Message) verifie que le message est recu
    //hasFailed(Action)
};

/**
 * Enables to assign a function to each possible message.
 */
class Switch_Message_Action : public Action
{
private:
    std::vector<MessageID> onMessage;
    std::vector<Fct> doFct;
    uint8_t size;
    Communication* _commLocal;

public:
    Switch_Message_Action(float timeout, Communication* comm, int16_t require);
    void addPair(MessageID messageId, Fct fct);
    //start : inherited from Action
    bool isFinished();
    //has failed : inherited from Action
};

/**
 * Send an order action to the actuator electronic card.
 * waitCompletion : Waits for the execution of action by the actuator electronic card.
 */
class Send_Order_Action : public Double_Action
{
private:
    Send_Action* sendAction;
    Wait_Message_Action* waitAction;

    Message message;
public:
    Send_Order_Action(MessageID actuatorID,
                      Actuator_Order actuatorOrder,
                      float timeout,
                      Communication *comm,
                      boolean waitCompletion,
                      int16_t require = NO_REQUIREMENT);
    
    bool isFinished();
};

//========================================ACTION MISC========================================

/**
 * Does nothing during the sleeping time.
 * Ghost continues on last action.
 */
class Sleep_Action : public Action
{
private:
    float timeToWait;

public:
    Sleep_Action(float timeToWait, int16_t require = NO_REQUIREMENT);
    //start(Action)
    bool isFinished();                 //(Sleep) verifie que le temps prévu s'est ecoulé
    bool hasFailed() { return false; } //(Sleep) on en peut pas fail d'attendre
};

/**
 * Does nothing.
 * Can disable an action of a double action.
 */
class Null_Action : public Action
{
public:
    Null_Action();
    bool isFinished() {return true;}
    bool hasFailed() {return false;}
};

/**
 * Does nothing and is impossible to skip.
 * If loop is activated, this action enables to go back to the first action of the queue.
 * If pause is activated, the sequence does not actualise until not resumed.
 * If lockGhost is activated, ghost does not actualise anymore.
 * By default, an endAction put to sleep a sequence.
 */
class End_Action : public Action //Une End_Action ne passe jamais a la suite
{
private:
    bool loop;
    bool pause;
    bool lockGhost;

public:
    End_Action(bool loop = false, bool pause = true, bool lockGhost = false);
    void start();
    bool isFinished() { return done; }
    bool hasFailed() { return false; }
};

/**
 * Do the action stocked as 'functionToCall' when action starts.
 */
class Do_Action : public Action
{
private:
    Fct functionToCall;

public:
    void start();
    Do_Action(Fct functionToCall, int16_t require = NO_REQUIREMENT)
    : Action("DoAc", 0.1, require) { this->functionToCall = functionToCall; }
};

/**
 * Set a sequence to pause when action starts.
 */
class PauseSeq_Action : public Action
{
private:
    SequenceName nameSeq;
    bool lockGhost;

public:
    void start();
    PauseSeq_Action(SequenceName nameSeq, bool lockGhost, int16_t require = NO_REQUIREMENT);
};

/**
 * Resume a sequence during start.
 */
class ResumeSeq_Action : public Action
{
private:
    SequenceName nameSeq;

public:
    void start();
    ResumeSeq_Action(SequenceName nameSeq, int16_t require = NO_REQUIREMENT);
};

/**
 * Waiting for an error.
 */
class Wait_Error_Action : public Action
{
private:
    Error error;

public:
    Wait_Error_Action(Error error, float timeout, int16_t require = NO_REQUIREMENT);
    //void start();
    bool isFinished(); //(Wait_Error) verifie que l'erreur s'est produite
    //hasFailed(Action)
};

/**
 * Recalibrate the PID and position.
 */
class Recallage_Action : public Double_Action
{
public:
    Recallage_Action(bool arriere, float dist, float timeout);
};

//========================================ACTION INPUT========================================

/**
 * Wait for the zipper to be pulled out.
 * The pin is set to low when the zipper is in.
 */
class Wait_Tirette_Action : public Action
{
private:
    uint8_t pinIN;
    bool initOK = false;

public:
    Wait_Tirette_Action(uint8_t pinIN, int16_t require = NO_REQUIREMENT);
    void start();
    bool isFinished();
    bool hasFailed() { return false; } // On ne peut pas fail l'attente
};

#endif // !ACTION_H_
