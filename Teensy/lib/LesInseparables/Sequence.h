#ifndef SEQUENCE_H_
#define SEQUENCE_H_
/*
Chose qu'il est possible de faire:

0: Forward_Action 1.0 Fast      //L'action 0 avance de 1m rapidement
1: Set_Action 0 true 2          //L'action 1 incrémente de 2 la variable globale d'indice 0
2: Jump_Action 0 sup 10 3 0     //L'action 2 passe a l'instruction 3 si la variable globale d'indice 0 est superieur à 10. Sinon elle passe à 0
3: Send_Action Ouvre_Pince      //L'action 3 envoie le message Ouvre_Pince a l'arduino
4: Wait_Action 500              //L'action 4 bloque la queue pendant 500 secondes
5: Spin_Action 3.14 fast        //L'action 5 tourne le robot vers pi
*/


#define TAILLESEQUENCE 50
#include "Arduino.h"
#include "SequenceName.h"

class Action;

/**
 * Creates a sequence class defining the action queue and all parameters.
 */
class Sequence
{
private:
    Action* queue[TAILLESEQUENCE];
    bool fails[TAILLESEQUENCE];
    uint8_t currentIndex;
    int16_t lastIndex;
    bool paused;
    int mySeqIndex;
public:
    uint8_t nextIndex;

    SequenceName getName();

    /**
     * Start the action designated by currentIndex (unlock the sequence if necessary).
     */
    void startSelected();

    /**
     * Starts the action designated by nextIndex.
     */
    void startFollowing();

    /**
     * Starts the action designated by nextIndex and indicates the previous action ended normally.
     */
    void forceFollowing();

    /**
     * Verify the state of current action and acts if the action succeeds or fails.
     */
    void update();

    /**
     * Changes the nextIndex value.
     */
    void setNextIndex(uint8_t index);

    /**
     * Creates an empty sequence.
     * By default, sequence is set on pause.
     */
    Sequence(int mySeqIndex);

    /**
     * Add an action at the end of sequence queue.
     */
    void add(Action* action);

    /**
     * Uploads infos concerning this sequence.
     * @warning To use on only one sequence.
     */
    void toTelemetry();

    /**
     * Prevent the actualisation of sequence and lock the ghost.
     */
    void pause(bool lockGhost);

    /**
     * Set the currentIndex to first action.
     */
    void reset(bool lockGhost);

    /**
     * Unlock the sequence and restarts the current action.
     */
    void resume();

};

#endif // !SEQUENCE_H_