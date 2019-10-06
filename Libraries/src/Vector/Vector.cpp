#include "Vector.h"

Vector::Vector(float x = 0.0, float y = 0.0)
{
    _x = x;
    _y = y;
}

Vector initVector(float x, float y)
{
    Vector out;
    out._x = x;
    out._y = y;
    return out;
}

VectorE::VectorE(float x = 0.0, float y = 0.0, float theta = 0.0)
{
    _vec._x = x;
    _vec._y = y;
    _theta = theta;
}

VectorE initVectorE(float x, float y, float theta)
{
    VectorE out;
    out._vec._x = x;
    out._vec._y = y;
    out._theta = theta;
    return out;
}

float Norm(Vector V)
{
    float out = sqrt(V._x * V._x + V._y * V._y);
    return out;
}

Vector Minus(Vector v1, Vector v2)
{
    Vector out;
    out._x = v1._x - v2._x;
    out._y = v1._y - v2._y;
    return out;
}

Vector Sum(Vector v1, Vector v2)
{
    Vector out;
    out._x = v1._x + v2._x;
    out._y = v1._y + v2._y;
    return out;
}

float VectorE::NormalizeTheta()
{
    float out;
    out=_theta-(2*PI)*((int)(theta/(2*PI)));
    if (out>PI)
        return (out-2*PI);
    else if (out<=-PI)
        return (out+2*PI);
    
    _theta = out;
    return out;
}