#ifndef VECTOR_H
#define VECTOR_H

struct Vector
{
    Vector(float x = 0.0, float y = 0.0);
    float _x;
    float _y;
};

struct VectorE
{
    VectorE(float x, float y, float theta);

    Vector _vec;
    float _theta;
};

Vector const vecNull = {0.0, 0.0};
Vector initVector(float x, float y);
VectorE initVectorE(float x, float y, float theta);

#endif