#include "MoveProfile.h"
#define RATIOPID 1000.0
#define mm 0.001
#define s 1.0

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
    
    get(brake,true)->set(0,0,0.01,0,10,0.0005,100,0.001,0.001); //On est pas regardant quant a la position (monnitored by epsilon) mais on regarde la vitesse (monitored by dEpsilon)
    get(brake,false)->set(0,0,0.01,0,10,0.01,100,0.003,0.003); //On est pas regardant quant a la position (monnitored by epsilon) mais on regarde la vitesse (monitored by dEpsilon)

    get(accurate,true)->set(34000,0.01,0.01,1,1*mm,0.5*mm/s,20*mm,0.3,200*mm/s);
    get(accurate,false)->set(4500,0.01,0.01,1, 1*DEG_TO_RAD, 1*DEG_TO_RAD/s, 15*DEG_TO_RAD,1.5,90*DEG_TO_RAD/s);
    
    get(standard,true)->set(40000,0.02,0.01,1, 10*mm, 3*mm/s,50*mm,0.1,0.5); 
    get(standard,false)->set(2500,0.005,0.02,1, 0.05, 0.01, 0.05,3.2,2.5);

    get(fast,true)->set(34000,0.01,1500,0.8, 0.15, 0.01, 0.10,2.1,1.5);
    get(fast,false)->set(4500,0.01,200,1, 0.05, 0.01, 0.05,30.0,999.0);
    
    /*get(recallage,true)->set(34000,0.01,0.01,1,1*mm,0.5*mm/s,20*mm,0.3,50*mm/s);
    get(recallage,false)->set(500,0.01,200,1,1*DEG_TO_RAD, 1*DEG_TO_RAD/s, 90*DEG_TO_RAD,1.5,90*DEG_TO_RAD/s);*/
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
