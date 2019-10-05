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