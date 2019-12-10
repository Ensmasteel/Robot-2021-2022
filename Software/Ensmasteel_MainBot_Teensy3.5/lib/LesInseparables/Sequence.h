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

    /*
    * Demarre l'action désigné par currentIndex (débloque la séquence si nécessaire)
    */
    void startSelected();

    /*
    * Demarre l'action désigné par nextIndex
    */
    void startFollowing();

    /*
    * Demarre l'action désigné par nextIndex et indique que l'action a terminé anormalement
    */
    void forceFollowing();

    /*
    * Verifie l'etat de l'action en cours et agit si l'action réussi/foire
    */
    void update();

    /*
    * Change la valeur du nextIndex
    */
    void setNextIndex(uint8_t index);

    /*
    * Cree une sequence vide
    * Par défaut, la séquence est en pause
    */
    Sequence(int mySeqIndex);

    /*
    * Ajoute une action au bout de la séquence
    */
    void add(Action* action);

    /*
    * Upload les informations liée a cette séquence
    * (A n'utiliser que sur une des séquences...)
    */
    void toTelemetry();

    /*
    * Empèche l'actualisation de la séquence et lock le ghost (ou pas)
    */
    void pause(bool lockGhost);

    /*
    * Met le currentIndex sur la première action.
    */
    void reset(bool lockGhost);

    /*
    * Debloque la sequence et redémarre l'action qui était en cours
    */
    void resume();

};

#endif // !SEQUENCE_H_