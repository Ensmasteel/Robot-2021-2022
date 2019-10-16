#ifndef PID_H_
#define PID_H_
#define NBPROFILES 5
#define TIMETOOFAR 0.2  //Temps qu'il faut rester trop loin pour etre considere tooFar
#include "Filtre.h"
#include "Vector.h"

struct PIDProfile
{
    float KP, KI, KD;        //Proportionel, Intégrateur, Dérivateur
    float epsilon, dEpsilon; //delta et delta de derivee en dessous de laquelle on considère qu'on est bon.
    float maxErr;            //Erreur maximal avant de considérer qu'on est trop loin.
};

PIDProfile newPIDProfile(float KP, float KI, float KD, float epsilon, float dEpsilon, float maxErr);

// La variable x définit une grandeur quelconque. dx est sa derivee.

class PID
{
private:
    PIDProfile PIDProfiles[NBPROFILES];
    int8_t currentProfile;
    float iTerm;
    bool modulo360;   //Permet de dire si les valeurs sont a interprété modulo 360
    Filtre dxF;       //Filtre de la derivee
    float timeTooFar; //Temps depuis lequel on est trop loin

public:
    bool close;  //Est ce qu'on est proche de la target (cf epsilon et depsilon)
    bool tooFar; //Est ce qu'on est trop loin (cf errMax)
    void reset();
    void setPIDProfile(uint8_t id, PIDProfile pidProfile);
    void setCurrentProfile(uint8_t id);
    float compute(float xTarget, float dxTarget, float x, float dx, float dt); //Renvoie un ordre entre -1 et 1
    PIDProfile getCurrentProfile();
    PID(bool modulo360, float frequency);
    PID();
};

class Asservissement
{
public:
    PID pidTranslation;
    PID pidRotation;
    bool close;           // Est ce qu'on est proche a la fois en position (projetée) ET en theta
                          //Il faut regarder la position projetée car le PID ne pourra rien y faire si on est à coté
   
    bool tooFar;          //Est ce qu'on est trop loin position (absolue) OU en theta
    bool needToGoForward; //Est ce qu'on va devoir avancer ? Utile pour l'évitemment

    //Place dans outTranslation et outRotation les deux ordres (entre -1 et 1)
    void compute(float *outTranslation, float *outRotation, Cinetique cRobot, Cinetique cGhost, float dt);
    Asservissement(float frequency);
};

#endif