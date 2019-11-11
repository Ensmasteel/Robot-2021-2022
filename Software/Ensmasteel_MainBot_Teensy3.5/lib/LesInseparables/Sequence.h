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

class Action;

class Sequence
{
private:
    Action* queue[TAILLESEQUENCE];
    bool fails[TAILLESEQUENCE];
    uint8_t currentIndex;
    int16_t lastIndex;
    bool paused;
public:
    uint8_t nextIndex;
    void startSelected();
    void startFollowing();
    void update();
    void setNextIndex(uint8_t index);
    Sequence();
    void add(Action* action);
    void toTelemetry();
    void pause();
    void resume();

};

#endif // !SEQUENCE_H_