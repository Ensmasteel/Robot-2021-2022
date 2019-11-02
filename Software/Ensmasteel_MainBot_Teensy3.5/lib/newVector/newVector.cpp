#include "newVector.h"
#include "Arduino.h"

// ===    Constructors    ===
// ==========================

const float Vector::LENGTH_TABLE = 3.0;
const float Vector::WIDTH_TABLE = 2.0;

Vector::Vector(float x, float y, bool absolute)
{
    _xB = x;
    _yB = y;

    if (absolute)
    {
        type = ABSOLUTE;
        _xJ = x;
        _yJ = y;
    }
    else
    {
        type = RELATIVE;
        _xJ = LENGTH_TABLE - x;
        _yJ = y;
    }
}

Vector::Vector(float xB, float yB, float xJ, float yJ)
{
    _xB = xB;
    _yB = yB;
    _xJ = xJ;
    _yJ = yJ;
    type = SPECIFIC;
}

VectorE::VectorE(float x, float y, float theta, bool absolute) : Vector(x, y, absolute)
{
    if (absolute)
    {
        _thetaB = theta;
        _thetaJ = theta;
    }
    else
    {
        _thetaB = theta;
        _thetaJ = PI - theta;
    }
}

VectorE::VectorE(float xB, float yB, float thetaB, float xJ, float yJ, float thetaJ) : Vector(xB, yB, xJ, yJ)
{
    _thetaB = thetaB;
    _thetaJ = thetaJ;
}

Cinetique::Cinetique(float x, float y, float theta, float v, float w, bool absolute) : VectorE(x, y, theta, absolute)
{
    _v = v;
    _w = w;
}

Cinetique::Cinetique(float xB, float yB, float thetaB, float xJ, float yJ, float thetaJ, float v, float w) : VectorE(xB, yB, thetaB, xJ, yJ, thetaJ)
{
    _v = v;
    _w = w;
}

// ===    Methods Vector    ===
// ============================

float Vector::getX()
{
    return((color==BLEU) ? _xB : _xJ);
}

float Vector::getY()
{
    return((color==BLEU) ? _yB : _yJ);
}

// ===    Methods VectorE    ===
// =============================

float VectorE::getTheta()
{
    return((color==BLEU) ? _thetaB : _thetaJ);
}
