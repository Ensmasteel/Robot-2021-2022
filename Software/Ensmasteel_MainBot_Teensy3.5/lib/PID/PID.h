#ifndef PID_H_
#define PID_H_
#include "Pace.h"
#define NBPROFILES ((int)Pace::NB_PACE)
#define TIMETOOFAR 0.2 //Temps qu'il faut rester trop loin pour etre considere tooFar
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


class Score
{
public:
    float cumulError=0;
    float maxOvershoot=0;
    uint16_t nbInversion=0;
    void reset();
    void toTelemetry(String prefix);
};

class PID
{
private:
    PIDProfile PIDProfiles[NBPROFILES];
    uint8_t currentProfile;
    float iTerm;
    bool modulo360;   //Permet de dire si les valeurs sont a interprété modulo 360
    Filtre dxF;       //Filtre de la derivee
    float timeTooFar; //Temps depuis lequel on est trop loin
    bool close;  //Est ce qu'on est proche de la target (cf epsilon et depsilon)
    bool tooFar; //Est ce qu'on est trop loin (cf errMax)
    float lastOut;
    Score score;

public:
    Score getScore();
    void reset();
    void setPIDProfile(Pace pace, PIDProfile pidProfile);
    void setCurrentProfile(Pace pace);
    float compute(float xTarget, float dxTarget, float x, float dx, float dt); //Renvoie un ordre entre -1 et 1
    PIDProfile getCurrentProfile();
    PID(bool modulo360, float frequency);
    PID();

friend class Asservissement;   //Rien n'est privé pour l'asservissement
};

class Asservissement
{
    PID pidTranslation;
    PID pidRotation;
    Cinetique *cGhost, *cRobot;
    float *outTranslation, *outRotation;

public:
    bool close;           // Est ce qu'on est proche a la fois en position (projetée) ET en theta
                          //Il faut regarder la position projetée car le PID ne pourra rien y faire si on est à coté
    bool tooFar;          //Est ce qu'on est trop loin position (absolue) OU en theta
    bool needToGoForward; //Est ce qu'on va devoir avancer ? Utile pour l'évitemment

    //Place dans outTranslation et outRotation les deux ordres (entre -1 et 1)
    void compute(float dt);
    void compute_dev(float dt);
    void setCurrentProfile(Pace pace);
    void reset();
    void sendScoreToTelemetry();

    Asservissement(float *outTranslation, float *outRotation, Cinetique *cRobot, Cinetique *cGhost, float frequency);
    Asservissement() {}
    float tweak(bool incr, bool translation, uint8_t whichOne);
};

#endif