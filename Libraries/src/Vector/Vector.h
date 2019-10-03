#ifndef VECTOR_INCLUDED
#define VECTOR_INCLUDED

#include "1_CONSTANT.h"

struct Vector
{
    float x;
    float y;
};

struct VectorE
{
    Vector vec;
    float theta;
};

Vector const vecNull= {0.0,0.0};
Vector initVector(float x,float y);
VectorE initVectorE(float x,float y,float theta);