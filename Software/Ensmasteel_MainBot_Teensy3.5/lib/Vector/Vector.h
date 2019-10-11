#ifndef VECTOR_H
#define VECTOR_H

#include "Arduino.h"

struct Vector
{
    Vector(float x = 0.0, float y = 0.0);
    float _x;
    float _y;
};

struct VectorE
{
    VectorE(float x = 0.0, float y = 0.0, float theta = 0.0);

    Vector _vec;
    float _theta;

    float NormalizeTheta(); // Normalize theta in [+PI ; -PI]
};

Vector const vecNull = {0.0, 0.0};
Vector initVector(float x, float y);
VectorE initVectorE(float x, float y, float theta);
float Norm(Vector V);
Vector Minus(Vector v1, Vector v2);
Vector Sum(Vector v1, Vector v2);
VectorE Sum(VectorE v1, VectorE v2);

#endif
