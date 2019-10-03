// compile with // g++ -o main Test_Math_functions.cpp .\src\Math_functions\Math_functions.cpp

#include <iostream>
#include <fstream>
#include <string>
#include "src/Math_functions/Math_functions.h"


Trapezoidal_Function Speed(2.0, 1.0, 1.0, 3.0);
Polynome P1, P2, P3, P4;
const int delta = 1000;

const int testMode = 2;


int test_speedRamps()
{
    std::cout << "Pentes : " << Speed._upRamp << ", " << Speed._downRamp << std::endl;

    for (int compteur = 0; compteur < (int)Speed._duration * delta; compteur++)
    {
        std::cout << Speed.f((float)compteur / delta) << std::endl;
    }
}

int test_Polynomes()
{
    P1 = init_polynome(10.0,2.0,-3.0);
    P2 = Derivative_ptr(&P1);
    P3 = init_polynome(-20.0);
    P4 = Sum_ptr(&P1, &P3);

    float x = 0.0;

    std::ofstream fileResults;
    fileResults.open ("Polynomes_tests.csv");

    for (int compteur = 0; compteur < delta; compteur++)
    {
        x = (float)(compteur/100.0)-5.0;
        fileResults << x << ";" << P1.f(x) << ";" << P2.f(x) << ";" << P4.f(x) << "\n";
    }
}

int main()
{
    switch (testMode)
    {
    case 1:
        test_speedRamps();
        break;

    case 2:
        test_Polynomes();
        break;

    default:
        std::cout << "No test executed" << std::endl;
    }
    return 0;
}
