#include "Math_functions.h"
#include "Logger.h"

Polynome::Polynome(float a0, float a1, float a2, float a3, float a4, float a5, float a6)
{
    K[0] = a0;
    K[1] = a1;
    K[2] = a2;
    K[3] = a3;
    K[4] = a4;
    K[5] = a5;
    K[6] = a6;
}

float Polynome::f(float x)
{
    float out = 0.0;
    float xn = 1.0;
    for (int i = 0; i < DEGRE_MAX; i += 1)
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
    for (int i = 1; i < DEGRE_MAX; i += 1)
    {
        out = out + K[i] * xn * i;
        xn = xn * x;
    }
    return out;
}

void Polynome::set(float a0, float a1, float a2, float a3, float a4, float a5, float a6)
{
    K[0] = a0;
    K[1] = a1;
    K[2] = a2;
    K[3] = a3;
    K[4] = a4;
    K[5] = a5;
    K[6] = a6;
}

void Polynome::toDebug()
{
    for (int i = 0; i < DEGRE_MAX; i += 1)
    {
        Logger::debug(String(K[i]));
        Logger::debug(" x^");
        Logger::debug(String(i));
        if (i < DEGRE_MAX - 1)
            Logger::debug(" + ");
    }
    Logger::debug("\n");
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

Polynome Derivative_ptr(Polynome *P)
{
    Polynome out;
    out = init_polynome();
    for (int d = 0; d < out.DEGRE_MAX - 1; d += 1)
    {
        out.K[d] = (float)(d + 1) * P->K[d + 1];
    }
    return out;
}

Polynome Derivative(Polynome P)
{
    Polynome out;
    out = init_polynome();
    for (int d = 0; d < out.DEGRE_MAX - 1; d += 1)
    {
        out.K[d] = (float)(d + 1) * P.K[d + 1];
    }
    return out;
}

Polynome Multiplication_ptr(Polynome *P1, Polynome *P2)
{
    Polynome out;
    out = init_polynome();
    for (int k = 0; k < out.DEGRE_MAX; k += 1)
    {
        for (int i = 0; i <= k; i += 1)
        {
            out.K[k] += P1->K[i] * P2->K[k - i];
        }
    }
    return out;
}

Polynome Multiplication(Polynome P1, Polynome P2)
{
    Polynome out;
    out = init_polynome();
    for (int k = 0; k < out.DEGRE_MAX; k += 1)
    {
        for (int i = 0; i <= k; i += 1)
        {
            out.K[k] += P1.K[i] * P2.K[k - i];
        }
    }
    return out;
}

Polynome Square_ptr(Polynome *P)
{
    Polynome out;
    out = init_polynome();
    out = Multiplication_ptr(P, P);
    return out;
}

Polynome Square(Polynome P)
{
    Polynome out;
    out = init_polynome();
    out = Multiplication(P, P);
    return out;
}

Polynome Sum_ptr(Polynome *P1, Polynome *P2)
{
    Polynome out;
    out = init_polynome();
    for (int d = 0; d < out.DEGRE_MAX; d += 1)
    {
        out.K[d] = P1->K[d] + P2->K[d];
    }
    return out;
}

Polynome Sum(Polynome P1, Polynome P2)
{
    Polynome out;
    out = init_polynome();
    for (int d = 0; d < out.DEGRE_MAX; d++)
    {
        out.K[d] = P1.K[d] + P2.K[d];
    }
    return out;
}

float Trapezoidal_Function::f(float x)
{
    float out = 0.0;
    if (x >= 0.0)
    {
        if (((_max / _upRamp) + (_max / _downRamp)) > _duration) // If _max nerver achieved
        {
            //Logger::infoln("Trapezoidal_Function._max never achieved");
            if (x < (_duration * _downRamp) / (_upRamp + _downRamp))
            {
                out = x * _upRamp;
            }
            else if (x < _duration)
            {
                out = (_duration - x) * _downRamp;
            }
        }
        else 
        {
            if (x < _max / _upRamp)
            {
                out = x * _upRamp;
            }
            else if (x < _duration - (_max / _downRamp))
            {
                out = _max;
            }
            else if (x < _duration)
            {
                out = (_duration - x) * _downRamp;
            }
        }
    }
    return out;
}

float Trapezoidal_Function::df(float x)
{
    float out = 0.0;

    if (x >= 0.0)
    {
        if (((_max / _upRamp) + (_max / _downRamp)) > _duration) // If _max nerver achieved
        {
            Logger::infoln("Trapezoidal_Function._max never achieved");
            if (x < (_duration * _downRamp) / (_upRamp + _downRamp))
            {
                out = _upRamp;
            }
            else if (x < _duration)
            {
                out = _downRamp;
            }
        }
        else
        {
            if (x < _max / _upRamp)
            {
                out = _upRamp;
            }
            else if (x < _duration - (_max / _downRamp))
            {
                out = 0;
            }
            else if (x < _duration)
            {
                out = _downRamp;
            }
        }
    }
    return out;
}

Trapezoidal_Function::Trapezoidal_Function(float upRamp, float downRamp, float max, float duration)
{
    _upRamp = upRamp;
    _downRamp = downRamp;
    _max = max;
    _duration = duration;
}

void Trapezoidal_Function::set(float upRamp, float downRamp, float max, float duration)
{
    _upRamp = upRamp;
    _downRamp = downRamp;
    _max = max;
    _duration = duration;
}
