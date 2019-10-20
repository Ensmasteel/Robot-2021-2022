#include "PID.h"
#define RECONVERGENCE 0.1
//SI le robot est eloigné de plus de RECONVERGENCE metres, le PID angulaire s'occupe a 100% de rejoindre le ghost, pas de mimer le theta

PIDProfile newPIDProfile(float KP, float KI, float KD, float epsilon, float dEpsilon, float maxErr)
{
    PIDProfile out;
    out.KP = KP;
    out.KI = KI;
    out.KD = KD;
    out.epsilon = epsilon;
    out.dEpsilon = dEpsilon;
    out.maxErr = maxErr;
    return out;
}

void PID::reset()
{
    this->iTerm = 0;
}

void PID::setPIDProfile(uint8_t id, PIDProfile pidProfile)
{
    this->PIDProfiles[id] = pidProfile;
}

void PID::setCurrentProfile(uint8_t id)
{
    this->currentProfile = id;
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

    close = abs(error) <= PIDProfiles[currentProfile].epsilon && abs(dError) <= PIDProfiles[currentProfile].dEpsilon;
    if (abs(error) > PIDProfiles[currentProfile].maxErr)
        timeTooFar += dt;
    else
        timeTooFar = 0;

    tooFar = timeTooFar > TIMETOOFAR;

    return constrain(
        PIDProfiles[currentProfile].KP * error 
        + PIDProfiles[currentProfile].KI * iTerm 
        + PIDProfiles[currentProfile].KD * dError
        , -1.0, 1.0);
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

    *outTranslation = pidTranslation.compute(lagBehind, cGhost->_v, 0, cRobot->_v, dt);
    *outRotation = pidRotation.compute(cGhost->_theta, cGhost->_w, cRobot->_theta, cRobot->_w, dt);

    close = pidTranslation.close && pidRotation.close;
    tooFar = pidTranslation.tooFar || pidRotation.tooFar || (*cGhost - *cRobot).norm()>pidTranslation.getCurrentProfile().epsilon;
}

void Asservissement::compute_dev(float dt)
{
    //lag behind represente l'avance du ghost sur le robot
    //C'est une avance projetée selon la direction du robot
    Vector deltaPos = *cGhost - *cRobot;
    float lagBehind = deltaPos % (directeur(cRobot->_theta));
    if (lagBehind < 0)
        lagBehind = -1 * sqrt(-1 * lagBehind);
    else
        lagBehind = sqrt(lagBehind);

    float thetaToReachGhost;
    if (lagBehind>0)
        thetaToReachGhost=normalizeAngle(deltaPos.angle());
    else
        thetaToReachGhost=normalizeAngle(deltaPos.angle() + PI);

    needToGoForward = (lagBehind > 0);

    *outTranslation = pidTranslation.compute(lagBehind, cGhost->_v, 0, cRobot->_v, dt);

    //Plus on est pres, plus le but est de mimer le theta du Ghost
    //Plus on est loin (deltaPos.norm()), plus le but est de rejoindre le Ghost
    float farFromGhost = constrain(deltaPos.norm()*RECONVERGENCE,0,1);
    *outRotation = pidRotation.compute(cGhost->_theta, cGhost->_w, (1-farFromGhost)*cRobot->_theta + farFromGhost*thetaToReachGhost, cRobot->_w, dt);

    close = pidTranslation.close && pidRotation.close;
    tooFar = pidTranslation.tooFar || pidRotation.tooFar || (*cGhost - *cRobot).norm()>pidTranslation.getCurrentProfile().epsilon;
}

Asservissement::Asservissement(float *outTranslation, float *outRotation, Cinetique * cRobot, Cinetique * cGhost,float frequency)
{
    pidRotation = PID(true, frequency);
    pidTranslation = PID(false, frequency);
    this->outRotation=outTranslation;
    this->outRotation=outRotation;
    this->cRobot=cRobot;
    this->cGhost=cGhost;
    this->tooFar = false;
    this->close = true;
    this->needToGoForward = false;

    //Définition des différents profils
    pidRotation.setPIDProfile(0, newPIDProfile(5, 5, 50, 0.001, 0.001, 0.01));
    pidTranslation.setPIDProfile(0, newPIDProfile(5, 5, 50,  0.01, 0.01, 0.1));
}

void Asservissement::setCurrentProfile(uint8_t id)
{
    pidTranslation.setCurrentProfile(id);
    pidRotation.setCurrentProfile(id);
}
