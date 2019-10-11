#include <Arduino.h>
#include <Vector.h>
#include <Math_functions.h>

void setup()
{
  Serial.begin(9600);

    Vector coord_1(1.0,2.0 );
    Vector coord_2(0.0, 1.5);
    VectorE Coord_orient(1.0, 1.0, -3.24);

    Serial.println(Norm(Minus(coord_1,coord_2)));
    Serial.println(Coord_orient.NormalizeTheta());

    /*
    std::cout << "Norms : " << Norm(coord_1) << Norm(Minus(coord_1, coord_2))
              << Norm(Sum(coord_1, coord_2)) << std::endl
              << "Orientation : " << Coord_orient._theta
              << Coord_orient.NormalizeTheta() << std::endl;
              */
}

void loop()
{
  
}