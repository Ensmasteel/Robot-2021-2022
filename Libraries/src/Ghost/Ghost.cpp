#include "Ghost.h"
#include "Arduino.h"

void Ghost::Ghost(VectorE posEIni)
{
    posCurrent = posEIni;
    t = 0.0;
    t_e = 0.0;
}

void Ghost::Set_NewTrajectory(Polynome newTrajectoryX, Polynome newTrajectoryY, Trapezoidal_Function newSpeed)
{
    trajectoryX = newTrajectoryX;
    trajectoryY = newTrajectoryY;
    speedProfileLinear = newSpeed;
    t = 0.0;
    t_e = 0.0;
}

void Compute_Trajectory(VectorE posFinal, float deltaCurve, float speedRamps, float cruisingSpeed)
{
    posAim = posFinal;
    t = 0.0;
    t_e = 0.0;

    // Define Trajectory
    float x0 = posCurrent.vec.x;
    float y0 = posCurrent.vec.y;
    float x3 = posAim.vec.x;
    float y3 = posAim.vec.y;
    float x1 = x0 + deltaCurve * cos(posCurrent.theta);
    float y1 = y0 + deltaCurve * sin(posCurrent.theta);
    float x2 = x3 - deltaCurve * cos(posAim.theta);
    float y2 = y3 - deltaCurve * sin(posAim.theta);

    trajectoryX.set(x0, 3.0 * (x1 - x0), 3.0 * (x0 - 2 * x1 + x2), 3.0 * x1 - x0 - 3.0 * x2 + x3);
    trajectoryY.set(y0, 3.0 * (y1 - y0), 3.0 * (y0 - 2 * y1 + y2), 3.0 * y1 - y0 - 3.0 * y2 + y3);

    // Define lengthTrajectory
    Polynome SpeedX_e = Derivative_ptr(&trajectoryX);
    Polynome SpeedY_e = Derivative_ptr(&trajectoryY);
    speedSquare_e = Sum(Square_ptr(&SpeedX_e), Square_ptr(&SpeedY_e));

    float lengthTrajectory = 0.0;
    float t_e_integral = 0.0, deltaIntegral = 0.01;
    float V_e = speedSquare_e.f(t_e_integral), lastV_e = speedSquare_e.f(t_e_integral);

    while (t_e_integral <= 1.0)
    {
        lastV_e = V_e;
        V_e = sqrt(speedSquare_e.f(t_e_integral));
        lengthTrajectory += ((V_e + lastV_e) / 2.0) * deltaIntegral;
        t_e_integral += deltaIntegral;
    }

    // Determine duration of trajectory given speed profile and trajectory's length
    durationTrajectory = ((lengthTrajectory / cruisingSpeed) + (cruisingSpeed / (2.0 * speedRamps))) * (2.0 / 3.0) + (cruisingSpeed / speedRamps);

    speedProfileLinear.set(speedRamps, cruisingSpeed, speedRamps, durationTrajectory);
}

void Ghost::lock(bool state)
{
    locked = state;
}

void Ghost::rotate(float rotationTheta)
{
    posAim.theta += rotationTheta;
}