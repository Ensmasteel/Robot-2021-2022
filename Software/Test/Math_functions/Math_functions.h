/**   Ensmasteel Library - Polynome and trapezoidale functions modelisation
 * 
 * author : Arthur FINDELAIR - EnsmaSteel, github.com/ArthurFDLR
 * date : October 2019
*/

#ifndef MATH_FUNCTION_H
#define MATH_FUNCTION_H

#include <cmath>

class Trapezoidal_Function
{
public:
    Trapezoidal_Function(float upRamp = 1.0, float downRamp = 1.0, float max = 0.0, float distance = 0.0);

    void set(float upRamp, float downRamp, float max, float distance);
    float _upRamp, _downRamp, _max, _duration, _distance; //upRamp : acceleration ; max : maximum value ; downRamp : deceleration ; duration : totale duration of the function (f(x)!=0 <=> 0<x<duration)
    float f(float x);                                    //value of the function in x
    float df(float x);                                  //value of the derivative function in x

private:
    bool _triangleFunction = false;
    float computeDuration();
    float _tMax; //time when f reach _max
};

#endif
