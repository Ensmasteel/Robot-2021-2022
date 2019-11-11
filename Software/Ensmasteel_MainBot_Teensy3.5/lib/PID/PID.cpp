#include "PID.h"
#include "ErrorManager.h"
#define RECONVERGENCE 0.05
//SI le robot est eloigné de plus de RECONVERGENCE metres, le PID angulaire s'occupe a 100% de rejoindre le ghost, pas de mimer le theta

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

Score PID::getScore()
{
    return score;
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
    if (tooFar)
        ErrorManager::raise(PID_FAIL_ERROR);

    score.cumulError+=abs(error)*dt;
    if ((dxTarget>=0 && error<0) || (dxTarget<0 && error>0)) //Condition d'overshoot
        score.maxOvershoot = max(score.maxOvershoot, abs(error));

    float out = constrain(
        PIDProfiles[currentProfile].KP * error + PIDProfiles[currentProfile].KI * iTerm + PIDProfiles[currentProfile].KD * dError, -1.0, 1.0);
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

//Définition des différents profils
    pidRotation.setPIDProfile(Pace::off, newPIDProfile(0, 0, 0, 0, 0, 100)); //OFF
    pidRotation.setPIDProfile(Pace::off, newPIDProfile(0, 0, 0, 0, 0, 100)); //OFF

    pidRotation.setPIDProfile(Pace::accurate, newPIDProfile(3500, 500, 500, 0.05, 0.01, 0.05));    //Accurate
    pidTranslation.setPIDProfile(Pace::accurate, newPIDProfile(5000, 500, 1000, 0.20, 0.05, 0.20)); //Accurate

    pidRotation.setPIDProfile(Pace::standard, newPIDProfile(3500, 300, 500, 0.05, 0.01, 0.05));    //Standard
    pidTranslation.setPIDProfile(Pace::standard, newPIDProfile(3500, 300, 1000, 0.10, 0.05, 0.20)); //Standard

    pidRotation.setPIDProfile(Pace::fast, newPIDProfile(1920, 300, 100, 0.05, 0.01, 0.01));    //Fast
    pidTranslation.setPIDProfile(Pace::fast, newPIDProfile(1200, 100, 2500, 0.15, 0.05, 0.1)); //Fast

    pidRotation.setPIDProfile(Pace::recallage, newPIDProfile(1920, 300, 100, 0.05, 0.01, 0.01));    //Recallage
    pidTranslation.setPIDProfile(Pace::recallage, newPIDProfile(1200, 100, 2500, 0.15, 0.05, 0.1)); //Recalage
}

void Asservissement::setCurrentProfile(Pace pace)
{
    pidTranslation.setCurrentProfile(pace);
    pidRotation.setCurrentProfile(pace);
}

float Asservissement::tweak(bool incr, bool translation, uint8_t whichOne)
{
    float coeff=(incr)?(1.01):(0.99);
    PID* toTweak=(translation)?(&pidTranslation):(&pidRotation);
    if (whichOne==0)
    {
        toTweak->PIDProfiles[toTweak->currentProfile].KP*=coeff;
        return toTweak->PIDProfiles[toTweak->currentProfile].KP;
    }
    else if (whichOne==1)
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