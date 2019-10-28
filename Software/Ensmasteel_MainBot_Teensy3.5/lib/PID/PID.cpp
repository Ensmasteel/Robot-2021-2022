#include "PID.h"
#define RECONVERGENCE 0.05
//SI le robot est eloigné de plus de RECONVERGENCE metres, le PID angulaire s'occupe a 100% de rejoindre le ghost, pas de mimer le theta
#define SIMULATOR

PIDProfile newPIDProfile(float KP, float KI, float KD, float epsilon, float dEpsilon, float maxErr)
{
    PIDProfile out;
    out.KP = KP / 4095.0; //Pour coller a l'ancien robot
    out.KI = KI / 4095.0; //Pour coller a l'ancien robot
    out.KD = KD / 4095.0; //Pour coller a l'ancien robot
    out.epsilon = epsilon;
    out.dEpsilon = dEpsilon;
    out.maxErr = maxErr;
    return out;
}

void PID::reset()
{
    this->iTerm = 0;
}

void PID::setPIDProfile(Pace pace, PIDProfile pidProfile)
{
    this->PIDProfiles[(int)pace] = pidProfile;
}

void PID::setCurrentProfile(Pace pace)
{
    this->currentProfile = pace;
}

PIDProfile PID::getCurrentProfile()
{
    return PIDProfiles[currentProfile];
}

float PID::compute(float xTarget, float dxTarget, float x, float dx, float dt)
{
    float error;
    float dError;

    if (modulo360)
        error = normalizeAngle(xTarget - x);
    else
        error = xTarget - x;

    dxF.in(dx, dt);
    dError = dxTarget - dxF.out();
    iTerm += error * dt;
    close = (abs(error) <= PIDProfiles[currentProfile].epsilon) && (abs(dError) <= PIDProfiles[currentProfile].dEpsilon);
    if (abs(error) > PIDProfiles[currentProfile].maxErr)
        timeTooFar += dt;
    else
        timeTooFar = 0;

    tooFar = timeTooFar > TIMETOOFAR;
    return constrain(
        PIDProfiles[currentProfile].KP * error + PIDProfiles[currentProfile].KI * iTerm + PIDProfiles[currentProfile].KD * dError, -1.0, 1.0);
}

PID::PID(bool modulo360, float frequency)
{
    this->currentProfile = 0;
    this->dxF = Filtre(0, frequency);
    this->tooFar = false;
    this->close = true;
    this->modulo360 = modulo360;
    this->timeTooFar = 0;
}

PID::PID() {}

void Asservissement::compute(float dt)
{
    //lag behind represente l'avance du ghost sur le robot
    //C'est une avance projetée selon la direction du robot
    float lagBehind = (*cGhost - *cRobot) % (directeur(cRobot->_theta));
    if (lagBehind < 0)
        lagBehind = -1 * sqrt(-1 * lagBehind);
    else
        lagBehind = sqrt(lagBehind);

    needToGoForward = (lagBehind > 0);
    *outRotation = pidRotation.compute(cGhost->_theta, cGhost->_w, cRobot->_theta, cRobot->_w, dt);
    *outTranslation = constrain(pidTranslation.compute(lagBehind, cGhost->_v, 0, cRobot->_v, dt)
                    , -(1 - abs(*outRotation)), 1 - abs(*outRotation)); //La rotation est prioritaire
    close = pidTranslation.close && pidRotation.close;
    tooFar = pidTranslation.tooFar || pidRotation.tooFar || (*cGhost - *cRobot).norm() > pidTranslation.getCurrentProfile().epsilon;
}

Asservissement::Asservissement(float *outTranslation, float *outRotation, Cinetique *cRobot, Cinetique *cGhost, float frequency)
{
    pidRotation = PID(true, frequency);
    pidTranslation = PID(false, frequency);
    this->outTranslation = outTranslation;
    this->outRotation = outRotation;
    this->cRobot = cRobot;
    this->cGhost = cGhost;
    this->tooFar = false;
    this->close = true;
    this->needToGoForward = false;

//Définition des différents profils
#ifdef SIMULATOR
    pidRotation.setPIDProfile(Pace::off, newPIDProfile(0, 0, 0, 0, 0, 100)); //OFF
    pidRotation.setPIDProfile(Pace::off, newPIDProfile(0, 0, 0, 0, 0, 100)); //OFF

    pidRotation.setPIDProfile(Pace::accurate, newPIDProfile(3500, 500, 500, 0.05, 0.01, 0.01));    //Accurate
    pidTranslation.setPIDProfile(Pace::accurate, newPIDProfile(5000, 500, 1000, 0.15, 0.05, 0.1)); //Accurate

    pidRotation.setPIDProfile(Pace::standard, newPIDProfile(3500, 300, 500, 0.05, 0.01, 0.01));    //Standard
    pidTranslation.setPIDProfile(Pace::standard, newPIDProfile(3500, 300, 1000, 0.15, 0.05, 0.1)); //Standard

    pidRotation.setPIDProfile(Pace::fast, newPIDProfile(1920, 300, 100, 0.05, 0.01, 0.01));    //Fast
    pidTranslation.setPIDProfile(Pace::fast, newPIDProfile(1200, 100, 2500, 0.15, 0.05, 0.1)); //Fast

    pidRotation.setPIDProfile(Pace::recallage, newPIDProfile(1920, 300, 100, 0.05, 0.01, 0.01));    //Recallage
    pidTranslation.setPIDProfile(Pace::recallage, newPIDProfile(1200, 100, 2500, 0.15, 0.05, 0.1)); //Recalage
#else
    pidRotation.setPIDProfile(Pace::off, newPIDProfile(0, 0, 0, 0, 0, 100)); //OFF
    pidRotation.setPIDProfile(Pace::off, newPIDProfile(0, 0, 0, 0, 0, 100)); //OFF

    pidRotation.setPIDProfile(Pace::accurate, newPIDProfile(6000, 500, 500, 0.05, 0.01, 0.01));    //Accurate
    pidTranslation.setPIDProfile(Pace::accurate, newPIDProfile(5000, 500, 1000, 0.15, 0.05, 0.1)); //Accurate

    pidRotation.setPIDProfile(Pace::standard, newPIDProfile(5000, 300, 500, 0.05, 0.01, 0.01));    //Standard
    pidTranslation.setPIDProfile(Pace::standard, newPIDProfile(4000, 300, 1000, 0.15, 0.05, 0.1)); //Standard

    pidRotation.setPIDProfile(Pace::fast, newPIDProfile(1920, 300, 100, 0.05, 0.01, 0.01));    //Fast
    pidTranslation.setPIDProfile(Pace::fast, newPIDProfile(1200, 100, 2500, 0.15, 0.05, 0.1)); //Fast

    pidRotation.setPIDProfile(Pace::recallage, newPIDProfile(1920, 300, 100, 0.05, 0.01, 0.01));    //Recallage
    pidTranslation.setPIDProfile(Pace::recallage, newPIDProfile(1200, 100, 2500, 0.15, 0.05, 0.1)); //Recalage
#endif
}

void Asservissement::setCurrentProfile(Pace pace)
{
    pidTranslation.setCurrentProfile(pace);
    pidRotation.setCurrentProfile(pace);
}

float Asservissement::tweak(bool incr, bool translation, bool P, bool I, bool D)
{
    float coeff=(incr)?(1.01):(0.99);
    PID* toTweak=(translation)?(&pidTranslation):(&pidRotation);
    if (P)
    {
        toTweak->PIDProfiles[toTweak->currentProfile].KP*=coeff;
        return toTweak->PIDProfiles[toTweak->currentProfile].KP;
    }
    else if (I)
    {
        toTweak->PIDProfiles[toTweak->currentProfile].KI*=coeff;
        return toTweak->PIDProfiles[toTweak->currentProfile].KI;
    }
    else
    {
        toTweak->PIDProfiles[toTweak->currentProfile].KD*=coeff;
        return toTweak->PIDProfiles[toTweak->currentProfile].KD;
    }
}