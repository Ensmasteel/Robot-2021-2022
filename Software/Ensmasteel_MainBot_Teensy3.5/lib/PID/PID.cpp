#include "PID.h"
#include "ErrorManager.h"
#define RECONVERGENCE 0.05
//SI le robot est eloigné de plus de RECONVERGENCE metres, le PID angulaire s'occupe a 100% de rejoindre le ghost, pas de mimer le theta


void Score::reset()
{
    this->cumulError=0;
    this->maxOvershoot=0;
    this->nbInversion=0;
}

void Score::toTelemetry(String prefix)
{
    Logger::toTelemetry(prefix+"cum",String(cumulError));
    Logger::toTelemetry(prefix+"ovs",String(maxOvershoot));
    Logger::toTelemetry(prefix+"inv",String(nbInversion));
}

void PID::reset()
{
    this->iTerm = 0;
    score.reset();
}

Score PID::getScore(){
    return score;
}

void PID::setCurrentProfile(MoveProfileName name){
    this->currentProfile = MoveProfiles::get(name,!modulo360);
}

MoveProfile* PID::getCurrentProfile(){
    return currentProfile;
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

    close = (abs(error) <= currentProfile->epsilon) && (abs(dError) <=currentProfile->dEpsilon);

    if (abs(error) > currentProfile->maxErr)
        timeTooFar += dt;
    else
        timeTooFar = 0;
    tooFar = timeTooFar > TIMETOOFAR;
    if (tooFar)
        ErrorManager::raise(PID_FAIL_ERROR);

    score.cumulError+=abs(error)*dt;
    if ((dxTarget>=0 && error<0) || (dxTarget<0 && error>0)) //Condition d'overshoot
        score.maxOvershoot = max(score.maxOvershoot, abs(error));

    float out = constrain(
        currentProfile->KP * error + currentProfile->KI * iTerm + currentProfile->KD * (currentProfile->KA*dxTarget - dxF.out()), -1.0, 1.0);
    if (lastOut*out<0 && (abs(lastOut)-abs(out))/dt > 0.01 ) //Changement de signe (1% a -1% d'output en une seconde)
        score.nbInversion = score.nbInversion + 1;

    lastOut=out;

    return out;
}

PID::PID(bool modulo360, float frequency)
{
    this->currentProfile = 0;
    this->dxF = Filtre(0, frequency);
    this->tooFar = false;
    this->close = true;
    this->modulo360 = modulo360;
    this->timeTooFar = 0;
    this->lastOut=0;
}

PID::PID() {}

void Asservissement::compute(float dt)
{
    //lag behind represente l'avance du ghost sur le robot
    //C'est une avance projetée selon la direction du robot
    float lagBehind = (*cGhost - *cRobot) % (directeur(cRobot->_theta));

    needToGoForward = (lagBehind > 0);
    *outRotation = pidRotation.compute(cGhost->_theta, cGhost->_w, cRobot->_theta, cRobot->_w, dt);
    *outTranslation = constrain(pidTranslation.compute(lagBehind, cGhost->_v, 0, cRobot->_v, dt)
                    , -(1 - abs(*outRotation)), 1 - abs(*outRotation)); //La rotation est prioritaire
    close = pidTranslation.close && pidRotation.close;
    tooFar = pidTranslation.tooFar || pidRotation.tooFar || (*cGhost - *cRobot).norm() > pidTranslation.getCurrentProfile()->epsilon;
}

void Asservissement::reset()
{
    pidRotation.reset();
    pidTranslation.reset();
}

void Asservissement::sendScoreToTelemetry()
{
    pidRotation.getScore().toTelemetry("R");
    pidTranslation.getScore().toTelemetry("T");
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
}

void Asservissement::setCurrentProfile(MoveProfileName name)
{
    pidTranslation.setCurrentProfile(name);
    pidRotation.setCurrentProfile(name);
}

float Asservissement::tweak(bool incr, bool translation, uint8_t whichOne)
{
    if (translation)
        return MoveProfiles::tweak(pidTranslation.currentProfile,incr,whichOne);
    else
        return MoveProfiles::tweak(pidRotation.currentProfile,incr,whichOne);
}