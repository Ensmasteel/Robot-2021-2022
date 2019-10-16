#include "PID.h"

//--------------ATTENTION, UTILISER LA VRAIE FONCTION DE NORMALISATION--------------------


PIDProfile newPIDProfile(float KP, float KI, float KD, float IRelax, float epsilon, float dEpsilon, float maxErr)
{
    PIDProfile out;
    out.KP = KP;
    out.KI = KI;
    out.KD = KD;
    out.IRelax = IRelax;
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
    iTerm *= PIDProfiles[currentProfile].IRelax;

    close = abs(error) <= PIDProfiles[currentProfile].epsilon && abs(dError) <= PIDProfiles[currentProfile].dEpsilon;
    if (abs(error) > PIDProfiles[currentProfile].maxErr)
        timeBlocked += dt;
    else
        timeBlocked = 0;

    blocked = timeBlocked > TIMEBLOCKED;
    
    return constrain( PIDProfiles[currentProfile].KP * error 
                    + PIDProfiles[currentProfile].KI * iTerm 
                    + PIDProfiles[currentProfile].KD * dError, -1.0, 1.0);
}

PID::PID(bool modulo360, float frequency)
{
    this->currentProfile = 0;
    this->dxF = Filtre(0, frequency);
    this->blocked = false;
    this->close = true;
    this->modulo360 = modulo360;
    this->timeBlocked = 0;
}

PID::PID() {}

void Asservissement::compute(float *outTranslation, float *outRotation, Cinetique cRobot, Cinetique cGhost, float dt)
{
    float lagBehind = ((Vector)(cGhost - cRobot))*(directeur(cRobot._theta));
    if (lagBehind < 0)
        lagBehind = -1 * sqrt(-1 * lagBehind);
    else
        lagBehind = sqrt(lagBehind);

    needToGoForward = (lagBehind > 0);

    *outTranslation = pidTranslation.compute(lagBehind, cGhost._v, 0, cRobot._v, dt);
    *outRotation = pidRotation.compute(cGhost._theta, cGhost._w, cRobot._theta, cRobot._w, dt);

    close = pidTranslation.close && pidRotation.close;
    blocked = pidTranslation.blocked || pidRotation.blocked;
}

Asservissement::Asservissement(float frequency)
{
    pidRotation = PID(true, frequency);
    pidTranslation = PID(false, frequency);
    this->blocked = false;
    this->close = true;
    this->needToGoForward = false;

    pidRotation.setPIDProfile(0, newPIDProfile(5, 5, 50, 0.9, 0.001, 0.001, 0.01));
    pidTranslation.setPIDProfile(0, newPIDProfile(5, 5, 50, 0.9, 0.01, 0.01, 0.1));
}
