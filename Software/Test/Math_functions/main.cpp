#include <iostream>
#include "Math_functions.h"

Trapezoidal_Function trapFun;

int main()
{
    trapFun.set(1.0,1.0,100.0,100.0);

    std::cout << trapFun._duration;

    float x = 0.0;
    while ( x <= trapFun._duration)
    {
        std::cout << " " << trapFun.f(x);
        x += 1.0;
    }
    return 1;
}
