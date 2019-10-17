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

Cinetique::Cinetique(float x, float y, float theta, float v, float w) : VectorE(x, y, theta)
{
    _v = v;
    _w = w;
}