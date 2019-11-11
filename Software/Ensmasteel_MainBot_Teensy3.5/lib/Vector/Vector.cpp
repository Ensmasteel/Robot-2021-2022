#include "Vector.h"
#include "Arduino.h"

float normalizeAngle(float angle)
{
    float out;
    out = angle - (2 * PI) * ((int)(angle / (2 * PI)));
    if (out > PI)
        return (out - 2 * PI);
    else if (out <= -PI)
        return (out + 2 * PI);

    return out;
}

Vector::Vector(float x, float y)
{
    _x = x;
    _y = y;
}

Vector Vector::operator+(const Vector &other)
{
    return Vector(_x + other._x, _y + other._y);
}

void Vector::operator+=(const Vector &other)
{
    _x+=other._x;
    _y+=other._y;
}

Vector Vector::operator-(const Vector &other)
{
    return Vector(_x - other._x, _y - other._y);
}

float Vector::operator%(const Vector &other)
{
    return _x * other._x + _y * other._y;
}

Vector Vector::operator*(float scalaire)
{
    return Vector(_x * scalaire, _y * scalaire);
}

float Vector::norm()
{
    return sqrt(_x * _x + _y * _y);
}

float Vector::distanceWith(Vector &other)
{
    return sqrt(((_x - other._x) * (_x - other._x)) + ((_y - other._y) * (_y - other._y)));
    //Equivalent a:
    //return operator-(other).norm()
}

void Vector::print(const String& prefix,bool info)
{
    if (info)
        Logger::infoln(prefix+":: x= "+String(_x)+" |y= "+String(_y));
    else
        Logger::debugln(prefix+":: x= "+String(_x)+" |y= "+String(_y));
}

void Vector::toTelemetry(const String& prefix)
{
    Logger::toTelemetry(prefix+"x",String(_x));
    Logger::toTelemetry(prefix+"y",String(_y));
}

bool Vector::operator==(Vector const &other)
{
    return (abs(_x - other._x) <= 1e-6) && (abs(_y - other._y) <= 1e-6);
}

float Vector::angle()
{
    return atan2(_y,_x);
}

Vector Vector::rotate(float theta)
{
    Vector out;
    out._x = _x * cos(theta) - _y * sin(theta);
    out._y = _y * cos(theta) + _x * sin(theta);
    return out;
}

Vector directeur(float theta)
{
    return Vector(cos(theta),sin(theta));
}

VectorE::VectorE(float x, float y, float theta) : Vector(x, y)
{
    _theta = theta;
}

void VectorE::normalizeTheta()
{
    _theta = normalizeAngle(_theta);
}

void VectorE::print(const String& prefix,bool info)
{
    if (info)
        Logger::infoln(prefix+":: x= "+String(_x)+" |y= "+String(_y)+" |Th= "+String(_theta));
    else
        Logger::debugln(prefix+":: x= "+String(_x)+" |y= "+String(_y)+" |Th= "+String(_theta));
}

void VectorE::toTelemetry(const String& prefix)
{
    Logger::toTelemetry(prefix+"x",String(_x));
    Logger::toTelemetry(prefix+"y",String(_y));
    Logger::toTelemetry(prefix+"Th",String(_theta));
}

bool VectorE::operator==(VectorE const &other)
{
    return (Vector::operator==(other)) && (abs(_theta - other._theta) <= 1e-9);
}


Cinetique::Cinetique(float x, float y, float theta, float v, float w) : VectorE(x, y, theta)
{
    _v = v;
    _w = w;
}

void Cinetique::print(const String& prefix,bool info)
{
    if (info)
        Logger::infoln(prefix+":: x= "+String(_x)+" |y= "+String(_y)+" |Th= "+String(_theta)+" |v= "+String(_v)+" |w= "+String(_w));
    else
        Logger::debugln(prefix+":: x= "+String(_x)+" |y= "+String(_y)+" |Th= "+String(_theta)+" |v= "+String(_v)+" |w= "+String(_w));
}

void Cinetique::toTelemetry(const String& prefix)
{
    Logger::toTelemetry(prefix+"x",String(_x));
    Logger::toTelemetry(prefix+"y",String(_y));
    Logger::toTelemetry(prefix+"Th",String(_theta));
    Logger::toTelemetry(prefix+"v",String(_v));
    Logger::toTelemetry(prefix+"w",String(_w));
}

bool Cinetique::operator==(Cinetique const &other)
{
    return VectorE::operator==(other) && _v==other._v && _w==other._w;
}

// =======     TARGETS    =======
// ==============================
TeamColor Target::teamColor = BLEU;

void Target::setTeamColor(TeamColor color) {teamColor=color;}

Target::Target() {type=RELATIVE;}

// =======     TargetVector    =======
// ===================================

Vector TargetVector::getVector()
{
    switch (type)
    {
        case ABSOLUTE: return Vector(_xB,_yB); break;
        case RELATIVE: return (teamColor==BLEU) ? (Vector(_xB,_yB)) : (Vector(LENGTH_TABLE - _xB,_yB)) ; break;
        case SPECIFIC: return (teamColor==BLEU) ? (Vector(_xB,_yB)) : (Vector(_xJ,_yJ)) ; break;
    }
}

TargetVector::TargetVector(float x, float y, bool absolute) : Target()
{
    type = (absolute)?(ABSOLUTE):(RELATIVE);
    _xB=x;
    _yB=y;
}

TargetVector::TargetVector(float xB, float yB, float xJ, float yJ) : Target()
{
    type = SPECIFIC;
    _xB=xB;
    _yB=yB;
    _xJ=xJ;
    _yJ=yJ;
}

// =======     TargetVectorE    =======
// ====================================

VectorE TargetVectorE::getVectorE()
{
    switch (type)
    {
        case ABSOLUTE: return VectorE(_xB,_yB,_thetaB); break;
        case RELATIVE: return (teamColor==BLEU) ? (VectorE(_xB,_yB,_thetaB)) : (VectorE(LENGTH_TABLE - _xB,_yB,PI - _thetaB)) ; break;
        case SPECIFIC: return (teamColor==BLEU) ? (VectorE(_xB,_yB,_thetaB)) : (VectorE(_xJ,_yJ,_thetaJ)) ; break;
    }
}

TargetVectorE::TargetVectorE(float x, float y, float theta, bool absolute) : Target()
{
    type = (absolute)?(ABSOLUTE):(RELATIVE);
    _xB=x;
    _yB=y;
    _thetaB=theta;
}

TargetVectorE::TargetVectorE(float xB, float yB, float thetaB, float xJ, float yJ, float thetaJ) : Target()
{
    type = SPECIFIC;
    _xB=xB;
    _yB=yB;
    _thetaB=thetaB;
    _xJ=xJ;
    _yJ=yJ;
    _thetaJ=thetaJ;
}

TargetVectorE::TargetVectorE(float theta, bool absolute)
{
    _xB=0;
    _yB=0;
    type = (absolute)?(ABSOLUTE):(RELATIVE);
    _thetaB=theta;
}

TargetVectorE::TargetVectorE(float thetaB, float thetaJ)
{
    type = SPECIFIC;
    _xB=0;
    _yB=0;
    _xJ=0;
    _yJ=0;
    _thetaB=thetaB;
    _thetaJ=thetaJ;
}

// =======     TargetCinetique    =======
// ====================================

Cinetique TargetCinetique::getCinetique()
{
    switch (type)
    {
        case ABSOLUTE: return Cinetique(_xB,_yB,_thetaB,_v,_w); break;
        case RELATIVE: return (teamColor==BLEU) ? (Cinetique(_xB,_yB,_thetaB,_v,_w)) : (Cinetique(LENGTH_TABLE - _xB,_yB,PI - _thetaB,_v,_w)) ; break;
        case SPECIFIC: return (teamColor==BLEU) ? (Cinetique(_xB,_yB,_thetaB,_v,_w)) : (Cinetique(_xJ,_yJ,_thetaJ,_v,_w)) ; break;
    }
}

TargetCinetique::TargetCinetique(float x, float y, float theta, float v, float w, bool absolute) : Target()
{
    type = (absolute)?(ABSOLUTE):(RELATIVE);
    _xB=x;
    _yB=y;
    _thetaB=theta;
    _v=v;
    _w=w;
}

TargetCinetique::TargetCinetique(float xB, float yB, float thetaB, float xJ, float yJ, float thetaJ, float v, float w) : Target()
{
    type = SPECIFIC;
    _xB=xB;
    _yB=yB;
    _thetaB=thetaB;
    _xJ=xJ;
    _yJ=yJ;
    _thetaJ=thetaJ;
    _v=v;
    _w=w;
}