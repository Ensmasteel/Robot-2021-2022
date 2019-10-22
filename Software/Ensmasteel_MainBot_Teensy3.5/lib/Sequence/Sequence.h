/*
Chose qu'il est possible de faire:

0: Forward_Action 1.0 Fast      //L'action 0 avance de 1m rapidement
1: Set_Action 0 true 2          //L'action 1 incrémente de 2 la variable globale d'indice 0
2: Jump_Action 0 sup 10 3 0     //L'action 2 passe a l'instruction 3 si la variable globale d'indice 0 est superieur à 10. Sinon elle passe à 0
3: Send_Action Ouvre_Pince      //L'action 3 envoie le message Ouvre_Pince a l'arduino
4: Wait_Action 500              //L'action 4 bloque la queue pendant 500 secondes
5: Spin_Action 3.14 fast        //L'action 5 tourne le robot vers pi
*/

#ifndef SEQUENCE_H_
#define SEQUENCE_H_
#define TAILLESEQUENCE 50
#define TAILLEGLOBALS 50
#include "Arduino.h"
#include "Actions.h"

class Sequence
{
private:
    uint8_t globals[TAILLEGLOBALS];
    Action* queue[TAILLESEQUENCE];
    bool fails[TAILLESEQUENCE];
    uint8_t currentIndex;
    uint8_t nextIndex;
    int lastIndex;
public:
    void reStart();
    void startNext();
    void update();
    void setNextIndex(uint8_t index);
    void setGlobal(uint8_t number, uint8_t value);
    uint8_t getGlobal(uint8_t number);
    Sequence();
    void add(Action* action);
    void debug(bool showCurrentActionState, bool showCurrentIndex, bool showSequence);
};

#endif // !SEQUENCE_H_