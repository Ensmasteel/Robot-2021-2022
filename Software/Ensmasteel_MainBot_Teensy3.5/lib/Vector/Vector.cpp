#include "Vector.h"

Vector::Vector(float x, float y)
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

VectorE::VectorE(float x, float y, float theta)
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
    return sqrt( (V._x * V._x) + (V._y * V._y) );
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

VectorE Sum(VectorE v1, VectorE v2)
{
    VectorE out;
    out._vec._x = v1._vec._x + v2._vec._x;
    out._vec._y = v1._vec._y + v2._vec._y;
    out._theta = v1._theta + v2._theta;
    out.NormalizeTheta();
    return out;
}

float VectorE::NormalizeTheta()
{
    float out;
    out=_theta-(2*PI)*((int)(_theta/(2*PI)));
    if (out>PI)
        return (out-2*PI);
    else if (out<=-PI)
        return (out+2*PI);
    
    _theta = out;
    return out;
}

float NormalizeAngle(float angle)
{
    float out;
    out=angle-(2*PI)*((int)(angle/(2*PI)));
    if (out>PI)
        return (out-2*PI);
    else if (out<=-PI)
        return (out+2*PI);
    
    return out;
}
