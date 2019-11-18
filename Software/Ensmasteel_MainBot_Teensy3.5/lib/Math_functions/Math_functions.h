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
    Trapezoidal_Function(float upRamp = 1.0, float downRamp = 1.0, float max = 0.0, float distance = 0.0);

    // GOAL / Create a new function given accelerations, max speed and the integer of the function.
    //      / If the max speed is unreachable given ramps and the distance, a triangle function is used.
    // IN   / float upRamp, downRamp : derivative at the beginning and the end of the function (downRamp is positive even if the real derivative is indeed negative)
    //      / float max : max value of the function
    //      / float distance : integer of the function (name given in reference to speed functions)
    // OUT  / float duration : (homogeneous to the IN variables) The function is define in [0,duration]
    float set(float upRamp, float downRamp, float max, float distance);

    // GOAL / Create a new constant function always returning 0
    void setZero();

    // GOAL / Get value of the function in x (0 < x < duration).
    float f(float x);

    // GOAL / Get value of the derivate function in x (0 < x < duration).
    float df(float x);

    float getDuration();
    bool isTriangle();

private:
    bool zero = true;
    bool _triangleFunction = false;
    float computeDuration();
    float _tMax; //time when f reach _max
    float _upRamp, _downRamp, _max, _duration, _distance; //upRamp : acceleration ; max : maximum value ; downRamp : deceleration ; duration : totale duration of the function (f(x)!=0 <=> 0<x<duration)
};

#endif
