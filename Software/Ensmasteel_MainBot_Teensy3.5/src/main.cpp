/**   Main bot teensy 3.5 tests
 *
 *  tests : 1 - polynome operation w\ pointers
 *          2 - Bezier Curves
 *          3 - Trajectories in real-time
 *          4 - Pure rotation
 *
 *  author : Arthur FINDELAIR - EnsmaSteel, github.com/ArthurFDLR
 *  date : October 2019
*/

// =============================
// ===       Libraries       ===
// =============================

#include <Arduino.h>
#include <Vector.h>
#include <Math_functions.h>
#include <Ghost.h>
// #include <PID.h>
// #include <Moteur.h>
// #include <Codeuse.h>

// ================================================
// ===       VARIABLES and INSTANTIATIONS       ===
// ================================================

const uint8_t MODE_TEST = 4;

VectorE posInitial(0.0, 0.0, 0.0);
VectorE posFinal(40.0, 15.0, 0.0);

Ghost botGhost(posInitial);

uint64_t timeLast = 0, timeCurrent = 0;
const uint64_t deltaTime = 1e5;

void PrintPolynome_Python(Polynome P);
void PolynomePtrOperation_test();
void TrajectoryRAW_Test();
void Trajecotry_Init_Test();
void Trajecotry_Loop_Test(float dt);
void Rotation_Init_Test();
void Rotation_Loop_Test(float dt);

// =====================================
// ===       ARDUINO PROCESSES       ===
// =====================================

void setup()
{
  Serial.begin(9600);

  switch (MODE_TEST)
  {
  case 1:
    PolynomePtrOperation_test();
    break;

  case 2:
    TrajectoryRAW_Test();
    break;

  case 3:
    Trajecotry_Init_Test();
    break;

  case 4:
    Rotation_Init_Test();
    break;

  default:
    break;
  }
}

void loop()
{
  switch (MODE_TEST)
  {

  case 3:
    if (micros() - timeLast > deltaTime)
    {
      timeCurrent = micros();
      //Serial.println((float)(timeCurrent - timeLast)/1e6);
      Trajecotry_Loop_Test((float)(timeCurrent - timeLast) / 1e6);
      timeLast = micros();
    }
    break;

  case 4:
    if (micros() - timeLast > deltaTime)
    {
      timeCurrent = micros();
      //Serial.println((float)(timeCurrent - timeLast)/1e6);
      Rotation_Loop_Test((float)(timeCurrent - timeLast) / 1e6);
      timeLast = micros();
    }
    break;

  default:
    break;
  }
}

// =============================
// ===       FUNCTIONS       ===
// =============================

/*------------*/
/*   TOOLS    */
/*------------*/

void PrintPolynome_Python(Polynome P)
{
  Serial.print("[");
  for (int i = 0; i < P.DEGRE_MAX; i += 1)
  {
    Serial.print(P.K[i]);
    if (i < P.DEGRE_MAX - 1)
      Serial.print(", ");
  }
  Serial.print("]\n");
}

/*------------------------------------*/
/*   TEST 1 : Polynomes operations    */
/*------------------------------------*/

void PolynomePtrOperation_test()
{
  Polynome P1(1.0, 4.0, -1.0, 4.0);
  Polynome P2(-3.0, 2.0, 1.0);
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

/*-----------------------------*/
/*   TEST 2 : Bezier Curves    */
/*-----------------------------*/

void TrajectoryRAW_Test()
{
  float deltaCurve = 0.4;
  float speedRamp = 50.0;
  float cruisingSpeed = 50.0;

  if (!botGhost.Compute_Trajectory(posFinal, deltaCurve, speedRamp, cruisingSpeed, false))
  {
    Serial.print("\nAimed position : ");
    Serial.print(botGhost.posAim._x);
    Serial.print(" ");
    Serial.print(botGhost.posAim._y);
    Serial.print(" ");
    Serial.print(botGhost.posAim._theta);
    Serial.print("\n");
    Serial.print("\nTrajectory X : ");
    PrintPolynome_Python(botGhost.trajectory_X);
    Serial.print("Trajectory Y : ");
    PrintPolynome_Python(botGhost.trajectory_Y);
    Serial.print("\n");
    Serial.print("Trajectory length : ");
    Serial.print(botGhost.lengthTrajectory);
    Serial.print("\n");
    Serial.print("Trajectory duration : ");
    Serial.print(botGhost.durationTrajectory);
    Serial.print("\n");
    Serial.print("Bezier speed squared : ");
    botGhost.speedSquare_e.SerialPrint();
    Serial.print("\n");

    for ( int i=0; i<=100; i+=1)
    {
      Serial.println(botGhost.speedProfileLinear.f((float)(i/100.0)*botGhost.durationTrajectory));
    }
  }
  else
  {
    Serial.println("Computation error");
  }
}

/*----------------------------*/
/*   TEST 3 : Trajecotries    */
/*----------------------------*/

void Trajecotry_Init_Test()
{
  float deltaCurve = 0.4;
  float speedRamp = 1.5;
  float cruisingSpeed = 5.0;

  timeLast = micros();
  timeCurrent = micros();

  if (!botGhost.Compute_Trajectory(posFinal, deltaCurve, speedRamp, cruisingSpeed))
  {
    botGhost.Lock(false);
    Serial.print("Trajectory duration : ");
    Serial.print(botGhost.durationTrajectory);
    Serial.print("\nTrajectory length : ");
    Serial.print(botGhost.lengthTrajectory);
    Serial.print("\nLocking state : ");
    Serial.print(botGhost.locked);
    Serial.print("\n");
    Serial.print("\nTrajectory X : ");
    PrintPolynome_Python(botGhost.trajectory_X);
    Serial.print("\nTrajectory Y : ");
    PrintPolynome_Python(botGhost.trajectory_Y);
    Serial.print("\n ///////////////////////////////////////////////////////////////////// \n\n");
    Serial.print("t;t_e;t_e_delayed;V(t);x;y;theta\n");
  }
  else
  {
    Serial.println("Computation error");
  }
}

void Trajecotry_Loop_Test(float dt)
{
  if (botGhost.IsMoving())
  {
    if (!botGhost.ActuatePosition(dt))
    {
      Serial.print(botGhost.t);
      Serial.print(";");
      Serial.print(botGhost.t_delayed);
      Serial.print(";");
      Serial.print(botGhost.t_e_delayed);
      Serial.print(";");
      Serial.print(botGhost.speedProfileLinear.f(botGhost.t));
      Serial.print(";");
      Serial.print(botGhost.posDelayed._x);
      Serial.print(";");
      Serial.print(botGhost.posDelayed._y);
      Serial.print(";");
      Serial.print(botGhost.posDelayed._theta);
      Serial.print("\n");
    }
  }
}

/*------------------------*/
/*   TEST 4 : Rotation    */
/*------------------------*/

void Rotation_Init_Test()
{
  float deltaCurve = 0.4;
  float speedRamp = 1.5;
  float cruisingSpeed = 1.0;
  float rotationRad = - 1.57 * 3.0;
  
  VectorE posFinalRotation(botGhost.posCurrent._x, botGhost.posCurrent._y, botGhost.posCurrent._theta + rotationRad);

  timeLast = micros();
  timeCurrent = micros();

  if (!botGhost.Compute_Trajectory(posFinalRotation, deltaCurve, speedRamp, cruisingSpeed, true))
  {
    botGhost.Lock(false);
    Serial.print("Rotation duration : ");
    Serial.print(botGhost.durationTrajectory);
    Serial.print("\nRotation length : ");
    Serial.print(botGhost.lengthTrajectory);
    Serial.print("\nLocking state : ");
    Serial.print(botGhost.locked);
    Serial.print("\n");
    Serial.print("\n ///////////////////////////////////////////////////////////////////// \n\n");
    Serial.print("t;t_e;V(t);x;y;theta\n");
  }
  else
  {
    Serial.println("Computation error");
  }
}

void Rotation_Loop_Test(float dt)
{
  if (botGhost.IsMoving())
  {
    if (!botGhost.ActuatePosition(dt))
    {
      Serial.print(botGhost.t);
      Serial.print(";");
      Serial.print(botGhost.t_e);
      Serial.print(";");
      Serial.print(botGhost.t_e_delayed);
      Serial.print(";");
      Serial.print(botGhost.speedProfileRotation.f(botGhost.t));
      Serial.print(";");
      Serial.print(botGhost.posDelayed._theta);
      Serial.print("\n");
    }
  }
}