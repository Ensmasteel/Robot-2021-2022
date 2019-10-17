/**   Ensmasteel Library - Vector modelisation
 * 
 * author : Arthur FINDELAIR - EnsmaSteel, github.com/ArthurFDLR
 * date : October 2019
*/

#ifndef VECTOR_H_
#define VECTOR_H_

#include "Arduino.h"

float normalizeAngle(float angle);

class Vector
{
public:
    float _x, _y;
    Vector operator+(const Vector &other);
    void operator+=(const Vector &other);
    Vector operator-(const Vector &other);
    float operator%(const Vector &other); //Produit scalaire
    Vector operator*(const float scalaire); //Produit par un scalaire (homotetie)
    float norm();
    float distanceWith(Vector &other);
    Vector(float x = 0.0, float y = 0.0);
};

Vector directeur(float theta);

class VectorE : public Vector
{
public:
    float _theta;
    void normalizeTheta();
    VectorE(float x = 0.0, float y = 0.0, float theta = 0.0);
};

class Cinetique : public VectorE
{
public:
    float _v;
    float _w;
    Cinetique(float x = 0.0, float y = 0.0, float theta = 0.0, float v = 0.0, float w = 0.0);
};

#endif
