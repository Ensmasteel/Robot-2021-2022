#include "MoveProfile.h"

MoveProfile MoveProfiles::profiles[2 * __NBPROFILES__];

void MoveProfile::set(float KP,float KI,float KD,float KA,float epsilon,float dEpsilon,float maxErr,float speedRamps,float cruisingSpeed)
{
    this->KP = KP/400.0;
    this->KI = KI/400.0;
    this->KD = KD/400.0;
    this->KA = KA;
    this->epsilon = epsilon;
    this->dEpsilon = dEpsilon;
    this->maxErr = maxErr;
    this->speedRamps = speedRamps;
    this->cruisingSpeed = cruisingSpeed;
}

//un degré = 0.017 rad

void MoveProfiles::setup() {
    get(off,true)->set(0,0,0,1,100,100,100,0.001,0.001);
    get(off,false)->set(0,0,0,1,100,100,100,0.003,0.003);
    
    get(brake,true)->set(0,0,100,0,10,0.0005,100,0.001,0.001); //On est pas regardant quant a la position (monnitored by epsilon) mais on regarde la vitesse (monitored by dEpsilon)
    get(brake,false)->set(0,0,60,0,10,0.01,100,0.003,0.003); //On est pas regardant quant a la position (monnitored by epsilon) mais on regarde la vitesse (monitored by dEpsilon)

    get(accurate,true)->set(1000,4,150,1,0.001,0.0005,0.01,0.5,0.3);
    get(accurate,false)->set(350, 300, 100,1, 0.008, 0.001, 0.05,1.5,1.0);
    
    get(standard,true)->set(1000, 150, 95,1, 0.10, 0.05, 0.20,1.0,0.7); //<--- a regler
    get(standard,false)->set(350, 100, 100,1, 0.05, 0.01, 0.05,3.2,2.5); //<-- a regler

    get(fast,true)->set(700, 1, 180,1, 0.15, 0.05, 0.1,2.0,1.0);
    get(fast,false)->set(350, 100, 100,1, 0.05, 0.01, 0.05,6.5,3.5);
    
    get(recallage,true)->set(700,1,150,0,0.001,0.0005,0.01,0.2,0.1);
    get(recallage,false)->set(350, 100, 100,0, 0.008, 0.001, 100,0.1,0.1);
}

MoveProfile* MoveProfiles::get(MoveProfileName name, bool translation) {
    return &profiles[2*(int)name + ((translation)?0:1)];
}

float MoveProfiles::tweak(MoveProfile* profile,bool incr,int whichOne) {
    float coeff=(incr)?(1.10):(0.90);
    if (whichOne==0){
        profile->KP*=coeff;
        return profile->KP;
    }
    else if (whichOne==1){
        profile->KI*=coeff;
        return profile->KI;
    }
    else{
        profile->KD*=coeff;
        return profile->KD;
    }
}
