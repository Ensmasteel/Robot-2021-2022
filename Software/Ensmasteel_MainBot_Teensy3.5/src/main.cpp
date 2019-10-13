#include <Arduino.h>
#include <Vector.h>
#include <Math_functions.h>
#include <Ghost.h>

VectorE posInitial(0.0, 0.0, 1.57);
VectorE posFinal(0.0, 20.0, 1.57);

float deltaCurve = 0.5;
Ghost botGhost(posInitial);
uint32_t timeLast = 0.0, timeCurrent = 0.0;
const uint32_t deltaTime = 10;

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

void PolynomePtrOperation_test()
{
  Polynome P1(1.0,4.0,-1.0,4.0);
  Polynome P2(-3.0,2.0,1.0);
  Polynome P12, P11, P11bis, P1_;

  //P12 = Multiplication_ptr(&P1,&P2);
  P11 = Square(P1);
  P11bis = Square_ptr(&P1);
  //P1_ = Derivative_ptr(&P1);

  P1.SerialPrint();
  P11.SerialPrint();
  P11bis.SerialPrint();
  //P11.SerialPrint();

  float x = 1.5847;
  Serial.println(P11.f(x));
  Serial.println(P11bis.f(x));
  Serial.println(P1.f(x) * P1.f(x));
}

void TrajectoryRAW_Test()
{
  if (!botGhost.Compute_Trajectory(posFinal, deltaCurve, 1.0, 1.0, false))
  {
    Serial.print("\nAimed position : ");
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
    Serial.print("\n");
    Serial.print("\nTrajecotry duration : ");
    Serial.print(botGhost.durationTrajectory);
  }
  else
  {
    Serial.println("Computation error");
  }
}

void Trajecotry_Init_Test()
{
  float speedRamp = 0.5;
  float cruisingSpeed = 1.0;

  timeLast = millis();
  timeCurrent = millis();

  if (!botGhost.Compute_Trajectory(posFinal, deltaCurve, speedRamp, cruisingSpeed, false))
  {
    botGhost.lock(false);
    Serial.print("Trajectory duration : ");
    Serial.print(botGhost.durationTrajectory);
    Serial.print("\nLocking state : ");
    Serial.print(botGhost.locked);
    Serial.print("\n");
    Serial.print("\nTrajectory X : ");
    PrintPolynome_Python(botGhost.trajectory_X);
    Serial.print("\nTrajectory Y : ");
    PrintPolynome_Python(botGhost.trajectory_Y);
    Serial.print("\n ///////////////////////////////////////////////////////////////////// \n");
  }
  else
  {
    Serial.println("Computation error");
  }
}

void Trajecotry_Loop_Test(float dt)
{
  if (!botGhost.ActuatePosition(dt))
  {
    Serial.print(botGhost.t);
    Serial.print(", ");
    Serial.print(botGhost.t_e);
    Serial.print(", ");
    Serial.print(botGhost.posCurrent._vec._x);
    Serial.print(", ");
    Serial.print(botGhost.posCurrent._vec._y);
    Serial.print(", ");
    Serial.print(botGhost.posCurrent._theta);
    Serial.print("\n");
  }
}

void setup()
{
  Serial.begin(9600);

  //PolynomePtrOperation_test();
  TrajectoryRAW_Test();
  //Trajecotry_Init_Test();
}

void loop()
{
  /*
  if (millis() - timeLast > deltaTime)
  {
    timeLast = timeCurrent;
    timeCurrent = millis();
    Trajecotry_Loop_Test((float)(timeCurrent - timeLast));
  }
  */
}