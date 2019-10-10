// compile with // g++ -o main Test_Math_functions.cpp .\src\Math_functions\Math_functions.cpp src/Vector/Vector.cpp

#include <iostream>
#include <fstream>
#include <string>
#include "src/Math_functions/Math_functions.h"
//#include "src/Vector/Vector.h"

const int delta = 1000;

const int testMode = 3;

void test_speedRamps()
{
    Trapezoidal_Function Speed(2.0, 1.0, 1.0, 3.0);

    std::cout << "Pentes : " << Speed._upRamp << ", " << Speed._downRamp << std::endl;

    std::ofstream fileResults;
    fileResults.open("TrapFunction_tests.csv");
    float x = 0.0;
    for (int compteur = 0; compteur < delta; compteur += 1)
    {
        x = (float)compteur * Speed._duration / delta;
        fileResults << x << ";" << Speed.f(x) << ";" << Speed.df(x) << "\n";
    }
}

void test_Polynomes()
{
    Polynome P1 = init_polynome(10.0, 2.0, -3.0);
    Polynome P2 = Derivative_ptr(&P1);
    Polynome P3 = init_polynome(-20.0);
    Polynome P4 = Sum_ptr(&P1, &P3);

    float x = 0.0;

    for (int compteur = 0; compteur < P1.DEGRE_MAX; compteur++)
        std::cout << P1.K[compteur] << std::endl;

    std::ofstream fileResults;

    fileResults.open("Polynomes_tests.csv");

    for (int compteur = 0; compteur < delta; compteur++)
    {
        x = (float)(compteur / 100.0) - 5.0;
        fileResults << x << ";" << P1.f(x) << ";" << P2.f(x) << ";" << P4.f(x) << "\n";
    }
}

/*
int test_vectors()
{
    Vector coord_1(1.0, -1.5);
    Vector coord_2(0.0, 1.5);
    VectorE Coord_orient(1.0, 1.0, 3.14);

    std::cout << "Norms : " << Norm(coord_1) << Norm(Minus(coord_1, coord_2))
              << Norm(Sum(coord_1, coord_2)) << std::endl
              << "Orientation : " << Coord_orient._theta
              << Coord_orient.NormalizeTheta() << std::endl;
}
*/

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

    case 3:
        test_vectors();

    default:
        std::cout << "No test executed" << std::endl;
    }
    return 0;
}
