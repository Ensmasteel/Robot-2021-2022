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