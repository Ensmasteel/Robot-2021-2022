#include "Vector.h"
#include "Arduino.h"

Vector initVector(float x,float y)
{
    Vector out;
    out.x=x;
    out.y=y;
    return out;
}

VectorE initVectorE(float x,float y,float theta)
{
    VectorE out;
    out.vec.x=x;
    out.vec.y=y;
    out.theta=theta;
    return out;
}