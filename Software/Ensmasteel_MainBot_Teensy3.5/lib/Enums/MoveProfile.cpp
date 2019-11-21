#include "MoveProfile.h"
#define RATIOPID 1000.0

MoveProfile MoveProfiles::profiles[2 * __NBPROFILES__];

void MoveProfile::set(float KP,float KI,float KD,float KA,float epsilon,float dEpsilon,float maxErr,float speedRamps,float cruisingSpeed)
{
    this->KP = KP/RATIOPID;
    this->KI = KI/RATIOPID;
    this->KD = KD/RATIOPID;
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
    
    get(brake,true)->set(0,0,1,0,10,0.0005,100,0.001,0.001); //On est pas regardant quant a la position (monnitored by epsilon) mais on regarde la vitesse (monitored by dEpsilon)
    get(brake,false)->set(0,0,200,0,10,0.01,100,0.003,0.003); //On est pas regardant quant a la position (monnitored by epsilon) mais on regarde la vitesse (monitored by dEpsilon)

    get(accurate,true)->set(0.01,0.01,0.01,1,0.001,0.0005,0.01,0.5,0.2);
    get(accurate,false)->set(4500,0.01,0.01,1, 0.008, 0.001, 0.05,1.5,1.0);
    
    get(standard,true)->set(0.01,0.01,0.01,1, 0.10, 0.05, 0.20,1.0,0.7); 
    get(standard,false)->set(4500,0.01,0.01,1, 0.05, 0.01, 0.05,3.2,2.5);

    get(fast,true)->set(0.01,0.01,0.01,1, 0.15, 0.05, 0.1,2.5,1.2);
    get(fast,false)->set(4500,0.01,200,1, 0.05, 0.01, 0.05,30.0,999.0);
    
    get(recallage,true)->set(0.01,0.01,0.01,1,0.005,0.05,0.01,0.2,0.1);
    get(recallage,false)->set(4500,0.01,0.01,1, 0.008, 0.001, 100,0.1,0.1);
}

MoveProfile* MoveProfiles::get(MoveProfileName name, bool translation) {
    return &profiles[2*(int)name + ((translation)?0:1)];
}

float MoveProfiles::tweak(MoveProfile* profile,bool incr,int whichOne) {
    float coeff=(incr)?(1.10):(0.90);
    if (whichOne==0){
        profile->KP*=coeff;
        return profile->KP*RATIOPID;
    }
    else if (whichOne==1){
        profile->KI*=coeff;
        return profile->KI*RATIOPID;
    }
    else{
        profile->KD*=coeff;
        return profile->KD*RATIOPID;
    }
}
