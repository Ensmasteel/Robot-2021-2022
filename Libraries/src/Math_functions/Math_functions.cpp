#include "Math_functions.h"

float Polynome::f(float x)
{
    float out = 0.0;
    float xn = 1.0;
    for (int i = 0; i <= DEGRE_MAX; i++)
    {
        out = out + K[i] * xn;
        xn = xn * x;
    }
    return out;
}

float Polynome::df(float x)
{
    float out = 0.0;
    float xn = 1.0;
    for (int i = 1; i <= DEGRE_MAX; i++)
    {
        out = out + K[i] * xn * i;
        xn = xn * x;
    }
    return out;
}

void Polynome::set(float in_a0, float in_a1, float in_a2, float in_a3, float in_a4, float in_a5, float in_a6)
{
    K[0] = in_a0;
    K[1] = in_a1;
    K[2] = in_a2;
    K[3] = in_a3;
    K[4] = in_a4;
    K[5] = in_a5;
    K[6] = in_a6;
}

Polynome init_polynome(float a0, float a1, float a2, float a3, float a4, float a5, float a6)
{
    Polynome out;
    out.K[0] = a0;
    out.K[1] = a1;
    out.K[2] = a2;
    out.K[3] = a3;
    out.K[4] = a4;
    out.K[5] = a5;
    out.K[6] = a6;
    return out;
}

Polynome Derivative_ptr(Polynome* P)
{
    Polynome out;
    out=init_polynome();
    for (int d=1; d<=out.DEGRE_MAX; d++)
    {
        out.K[d-1]=d*P->K[d];
    }
    return out;
}

Polynome Multiplication_ptr(Polynome* P1,Polynome* P2)
{
    Polynome out;
    out=init_polynome();
    for (int degreP=0; degreP<=out.DEGRE_MAX; degreP++)
    {
        for (int d=0; d<=degreP; d++)
        {
            out.K[degreP]=out.K[degreP]+P1->K[d]*P2->K[degreP-d];
        }
    }
    return out;
}

Polynome Square_ptr(Polynome* P)
{
    Polynome out;
    out=init_polynome();
    out=Multiplication_ptr(P,P);
    return out;
}

Polynome Sum_ptr(Polynome* P1,Polynome* P2)
{
    Polynome out;
    out=init_polynome();
    for (int d=0; d<=out.DEGRE_MAX; d++)
    {
        out.K[d]=P1->K[d]+P2->K[d];
    }
    return out;
}

float Trapezoidal_Function::f(float x)
{
    float out = 0.0;
    if (x > 0.0)
    {
        if (x < _max / _upRamp)
        {
            out = x * _upRamp;
        }
        else if (x < _duration - (_max/_downRamp))
        {
            out = _max;
        }
        else if (x < _duration)
        {
            out = (_duration - x) * _downRamp;
        }
    }
    return out;
}

float Trapezoidal_Function::df(float x)
{
    float out = 0.0;
    if (x > 0.0)
    {
        if (x < _max / _upRamp)
        {
            out = _upRamp;
        }
        else if (x < _duration - (_max/_downRamp))
        {
            out = 0;
        }
        else if (x < _duration)
        {
            out = _downRamp;
        }
    }
    return out;
}

Trapezoidal_Function::Trapezoidal_Function()
{
    _upRamp = 1;
    _downRamp = 1;
    _max = 0;
    _duration = 0;
}

Trapezoidal_Function::Trapezoidal_Function(float upRamp, float downRamp, float max, float duration)
{
    _upRamp = upRamp;
    _downRamp = downRamp;
    _max = max;
    _duration = duration;
}
