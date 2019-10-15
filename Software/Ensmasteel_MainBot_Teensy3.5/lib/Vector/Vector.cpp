#include "Vector.h"

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

Vector Vector::operator-(const Vector &other)
{
    return Vector(_x - other._x, _y - other._y);
}

float Vector::operator*(const Vector &other)
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

VectorE VectorE::operator+(const VectorE &other)
{
    return VectorE(_x + other._x, _y + other._y, _theta);
}
VectorE VectorE::operator-(const VectorE &other)
{
    return VectorE(_x - other._x, _y - other._y, _theta);
}
VectorE VectorE::operator*(float scalaire)
{
    return VectorE(_x * scalaire, _y * scalaire, _theta);
}

Cinetique::Cinetique(float x, float y, float theta, float v, float w) : VectorE(x, y, theta)
{
    _v = v;
    _w = w;
}

Cinetique Cinetique::operator+(const Cinetique &other)
{
    return Cinetique(_x + other._x, _y + other._y, _theta, _v, _w);
}
Cinetique Cinetique::operator-(const Cinetique &other)
{
    return Cinetique(_x - other._x, _y - other._y, _theta, _v, _w);
}
Cinetique Cinetique::operator*(float scalaire)
{
    return Cinetique(_x * scalaire, _y * scalaire, _theta, _v, _w);
}