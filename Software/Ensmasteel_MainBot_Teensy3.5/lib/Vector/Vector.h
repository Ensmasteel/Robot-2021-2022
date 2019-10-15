/**   Ensmasteel Library - Vector modelisation
 * 
 * author : Arthur FINDELAIR - EnsmaSteel, github.com/ArthurFDLR
 * date : October 2019
*/

#ifndef VECTOR_H
#define VECTOR_H

#include "Arduino.h"

float normalizeAngle(float angle);

class Vector
{
public:
    float _x, _y;
    Vector operator+(Vector &other);
    Vector operator-(Vector &other);
    //Produit Scalaire
    float operator*(Vector &other);
    //Produit par un scalaire
    Vector operator*(float scalaire);
    float norm();
    Vector(float x = 0, float y = 0);
};

class VectorE : public Vector
{
public:
    float _theta;
    void normalizeTheta();
    VectorE(float x = 0, float y = 0, float theta = 0);

    VectorE operator+(VectorE &other);
    VectorE operator-(VectorE &other);
    VectorE operator*(float scalaire);
};

class Cinetique : public VectorE
{
public:
    float _v;
    float _w;
    Cinetique(float x = 0, float y = 0, float theta = 0, float v = 0, float w = 0);

    Cinetique operator+(Cinetique &other);
    Cinetique operator-(Cinetique &other);
    Cinetique operator*(float scalaire);
};

#endif
