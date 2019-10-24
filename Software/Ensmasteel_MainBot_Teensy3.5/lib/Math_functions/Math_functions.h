/**   Ensmasteel Library - Polynome and trapezoidale functions modelisation
 * 
 * author : Arthur FINDELAIR - EnsmaSteel, github.com/ArthurFDLR
 * date : October 2019
*/

#ifndef MATH_FUNCTION_H
#define MATH_FUNCTION_H

#include "Arduino.h"

class Polynome
{
public:
    Polynome(float a0 = 0.0, float a1 = 0.0, float a2 = 0.0, float a3 = 0.0, float a4 = 0.0, float a5 = 0.0, float a6 = 0.0);
    static const int DEGRE_MAX = 7;

    float K[DEGRE_MAX]; //Les 10 coefficients du polynome, K[i] est le coefficient devant x^i
    float f(float x);   //value of the function in x
    float df(float x);  //value of the derivative function in x
    float ddf(float x);
    void set(float a0 = 0.0, float a1 = 0.0, float a2 = 0.0, float a3 = 0.0, float a4 = 0.0, float a5 = 0.0, float a6 = 0.0);
    void toDebug();
};

Polynome init_polynome(float a0 = 0.0, float a1 = 0.0, float a2 = 0.0, float a3 = 0.0, float a4 = 0.0, float a5 = 0.0, float a6 = 0.0);
Polynome Derivative_ptr(Polynome *P);
Polynome Derivative(Polynome P);
Polynome Multiplication_ptr(Polynome *P1, Polynome *P2); // Need K[i>=4] = 0 !
Polynome Multiplication(Polynome P1, Polynome P2);
Polynome Square_ptr(Polynome *P);
Polynome Square(Polynome P);
Polynome Sum_ptr(Polynome *P1, Polynome *P2);
Polynome Sum(Polynome P1, Polynome P2);

class Trapezoidal_Function
{
public:
    Trapezoidal_Function(float upRamp = 1.0, float downRamp = 1.0, float max = 0.0, float duration = 0.0);

    void set(float upRamp, float downRamp, float max, float duration);
    float _upRamp, _downRamp, _max, _duration; //upRamp : acceleration ; max : maximum value ; downRamp : deceleration ; duration : totale duration of the function (f(x)!=0 <=> 0<x<duration)
    float f(float x);                          //value of the function in x
    float df(float x);                         //value of the derivative function in x
    //float ddf(float x);
};

#endif
