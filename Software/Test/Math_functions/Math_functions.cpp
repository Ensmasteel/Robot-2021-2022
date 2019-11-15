#include "Math_functions.h"

float Trapezoidal_Function::f(float x)
{
    float out = 0.0;
    if (x >= 0.0)
    {
        if (_triangleFunction)
        {
            if (x < _tMax)
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
            if (x < _tMax)
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
        if (_triangleFunction)
        {
            //Logger::infoln("triangle function");
            if (x < _tMax)
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
            if (x < _tMax)
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

Trapezoidal_Function::Trapezoidal_Function(float upRamp, float downRamp, float max, float distance)
{
    _upRamp = upRamp;
    _downRamp = downRamp;
    _max = max;
    _distance = distance;

    computeDuration();
}

void Trapezoidal_Function::set(float upRamp, float downRamp, float max, float distance)
{
    _upRamp = upRamp;
    _downRamp = downRamp;
    _max = max;
    _distance = distance;

    computeDuration();

}

float Trapezoidal_Function::computeDuration()
{
    float distanceMin_maxReached =  ((_max*_max)/2.0) * ((1.0/_upRamp) + (1.0/_downRamp));

    if (distanceMin_maxReached > _distance) //triangle function
    {
        _triangleFunction = true;
        _max = sqrt((2.0*_distance) / ((1.0/_downRamp) + 1.0/_upRamp));
        _tMax = _max / _upRamp;
        _duration = _max / _upRamp + _max / _downRamp;
    }
    else // True trapezoidale function
    {
        _triangleFunction = false;

        _tMax = _max / _upRamp;
        _duration = (_distance / _max) + ((_max / 2.0) * ((1.0/_downRamp) + (1.0/_upRamp)));
    }
    
}
