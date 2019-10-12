#include <Arduino.h>
#include <Vector.h>
#include <Math_functions.h>
#include <Ghost.h>

void PrintPolynome_Python(Polynome P)
{
  Serial.print("[");
  for (int i = 0; i < P.DEGRE_MAX; i += 1)
  {
    Serial.print(P.K[i]);
    if (i < P.DEGRE_MAX - 1)
      Serial.print(", ");
  }
  Serial.print("]");
}

void trajectoryRAW_test()
{
  VectorE posInitial(0.0, 0.0, 0.0);
  VectorE posFinal(10.0, 10.0, -1.57);

  float deltaCurve = 0.3;

  Ghost botGhost(posInitial);
  botGhost.Compute_Trajectory(posFinal, deltaCurve, 1.0, 5.0, false);

  Serial.print("Aimed position : ");
  Serial.print(botGhost.posAim._vec._x);
  Serial.print(" ");
  Serial.print(botGhost.posAim._vec._y);
  Serial.print(" ");
  Serial.print(botGhost.posAim._theta);
  Serial.print("\n");
  Serial.print("\nTrajectory X : ");
  PrintPolynome_Python(botGhost.trajectory_X);
  Serial.print("\nTrajectory Y : ");
  PrintPolynome_Python(botGhost.trajectory_Y);
}

void setup()
{
  Serial.begin(9600);

  trajectoryRAW_test();
}

void loop()
{
}