#include "MoveProfile.h"


void MoveProfile::set(float KP,float KI,float KD,float KA,float epsilon,float dEpsilon,float maxErr,float speedRamps,float cruisingSpeed)
{
    this->KP = KP;
    this->KI = KI;
    this->KD = KD;
    this->KA = KA;
    this->epsilon = epsilon;
    this->dEpsilon = dEpsilon;
    this->maxErr = maxErr;
    this->speedRamps = speedRamps;
    this->cruisingSpeed = cruisingSpeed;
}

MoveProfile MoveProfiles::profiles[2 * __NBPROFILES__];

//un degré = 0.017 rad

void MoveProfiles::setup() {
    get(off,true)->set(0,0,0,1,100,100,100,0.001,0.001);
    get(off,false)->set(0,0,0,1,100,100,100,0.001,0.001);
    
    get(brake,true)->set(0,0,1000,0,100,0.0005,100,0.001,0.001); //On est pas regardant quant a la position (monnitored by epsilon) mais on regarde la vitesse (monitored by dEpsilon)
    get(brake,false)->set(0,0,600,0,100,0.01,100,0.001,0.001); //On est pas regardant quant a la position (monnitored by epsilon) mais on regarde la vitesse (monitored by dEpsilon)

    get(accurate,true)->set(3000,500,1000,1,0.001,0.0005,0.01,0.5,0.3);
    get(accurate,false)->set(3500, 500, 500,1, 0.008, 0.001, 0.05,0.5,0.3);
    
    get(standard,true)->set(3500, 300, 1000,1, 0.10, 0.05, 0.20,1.0,0.7);
    get(standard,false)->set(3500, 300, 500,1, 0.05, 0.01, 0.05,1.0,0.7);

    get(fast,true)->set(1200, 100, 2500,1, 0.15, 0.05, 0.1,2.0,1.0);
    get(fast,false)->set(1920, 300, 100,1, 0.05, 0.01, 0.01,2.0,1.0);

    get(recallage,true)->set(3000,50,500,0,0.001,0.0005,0.01,0.2,0.1);
    get(recallage,false)->set(500, 0, 500,0, 0.008, 0.001, 100,0.2,0.1);
}

MoveProfile* MoveProfiles::get(MoveProfileName name, bool translation) {
    return &profiles[(int)name + ((translation)?0:1)];
}

float MoveProfiles::tweak(MoveProfile* profile,bool incr,int whichOne) {
    float coeff=(incr)?(1.01):(0.99);
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
