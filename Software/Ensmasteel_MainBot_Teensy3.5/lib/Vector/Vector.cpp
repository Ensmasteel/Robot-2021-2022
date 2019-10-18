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

void Vector::print()
{
    Serial.print("x= ");
    Serial.print(_x);
    Serial.print("\t|y= ");
    Serial.print(_y);
    Serial.print("\t|");
}

bool Vector::operator==(Vector const &other)
{
    return _x==other._x && _y==other._y;
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

void VectorE::print()
{
    Vector::print();
    Serial.print("theta= ");
    Serial.print(_theta);
    Serial.print("\t|");
}

bool VectorE::operator==(VectorE const &other)
{
    return Vector::operator==(other) && _theta==other._theta;
}


Cinetique::Cinetique(float x, float y, float theta, float v, float w) : VectorE(x, y, theta)
{
    _v = v;
    _w = w;
}

void Cinetique::print()
{
    VectorE::print();
    Serial.print("v= ");
    Serial.print(_v);
    Serial.print("\t|w= ");
    Serial.print(_w);
}

bool Cinetique::operator==(Cinetique const &other)
{
    return VectorE::operator==(other) && _v==other._v && _w==other._w;
}