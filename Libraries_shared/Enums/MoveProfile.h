#ifndef MOVEPROFILE_H_
#define MOVEPROFILE_H_
#include "Arduino.h"

enum MoveProfileName
{
    off,
    brake,
    accurate,
    standard,
    fast,
    recallage,
    __NBPROFILES__
};

class MoveProfile
{
public:
    float KP, KI, KD, KA;        //Proportionel, Intégrateur, Dérivateur, Anticipateur
    float epsilon, dEpsilon; //delta et delta de derivee en dessous de laquelle on considère qu'on est bon.
    float maxErr;            //Erreur maximal avant de considérer qu'on est trop loin.
    float speedRamps;   //Accélération/Deceleration en m.s-2 ou rad.s-2
    float cruisingSpeed; //Vitesse max en m/s ou rad/s

private:
    void set(float KP,float KI,float KD,float KA,float epsilon,float dEpsilon,float maxErr,float speedRamps,float cruisingSpeed);
    friend class MoveProfiles;
};

class MoveProfiles
{
private:
    static MoveProfile profiles[2 * __NBPROFILES__];
public:
    static void setup();
    static MoveProfile* get(MoveProfileName name, bool translation);
    static float tweak(MoveProfile* profile,bool incr,int whichOne);
};


#endif // !MOVEPROFILE_H_
