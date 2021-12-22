#ifndef PID_H_
#define PID_H_
#include "MoveProfile.h"
#define NBPROFILES ((int)Pace::NB_PACE)
#define TIMETOOFAR 0.2 //Temps qu'il faut rester trop loin pour etre considere tooFar
#include "Filtre.h"
#include "Vector.h"


// La variable x définit une grandeur quelconque. dx est sa derivee.

/**
 * Defines a class to control PID factors and behaviour.
 */

/**
 * Class to save score and predictions.
 */
class Score
{
public:
    float cumulError=0;
    float maxOvershoot=0;
    uint16_t nbInversion=0;
    void reset();
    void toTelemetry(String prefix);
};

/**
 * Defines the PID class and its factors.
 */
class PID
{
private:
    MoveProfile* currentProfile; ///<Pointer to ensure there is only one instance of profile (no intern copy)
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
    void setCurrentProfile(MoveProfileName pace);
    float compute(float xTarget, float dxTarget, float x, float dx, float dt); //Renvoie un ordre entre -1 et 1
    MoveProfile* getCurrentProfile();
    PID(bool modulo360, float frequency);
    PID();

friend class Asservissement;   //Rien n'est privé pour l'asservissement
};

/**
 * Defines the enslavement class.
 */
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
    //void compute_dev(float dt);
    void setCurrentProfile(MoveProfileName pace);
    void reset();
    void sendScoreToTelemetry();

    Asservissement(float *outTranslation, float *outRotation, Cinetique *cRobot, Cinetique *cGhost, float frequency);
    Asservissement() {}
    float tweak(bool incr, bool translation, uint8_t whichOne);
};

#endif