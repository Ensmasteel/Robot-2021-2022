#include "Ghost.h"
#include "Arduino.h"

Ghost::Ghost(VectorE posEIni)
{
    posCurrent = posEIni;
    t = 0.0;
    t_e = 0.0;
}

void Ghost::Set_NewTrajectory(Polynome newTrajectoryX, Polynome newTrajectoryY, Trapezoidal_Function newSpeed)
{
    trajectory_X = newTrajectoryX;
    trajectory_Y = newTrajectoryY;
    speedProfileLinear = newSpeed;
    t = 0.0;
    t_e = 0.0;
}

int Ghost::Compute_Trajectory(VectorE posFinal, float deltaCurve, float speedRamps, float cruisingSpeed, bool pureRotation)
{
    posAim = posFinal;
    t = 0.0;
    t_e = 0.0;
    moving = true;

    posAim.normalizeTheta();
    posCurrent.normalizeTheta();
    rotating = pureRotation;

    VectorE out = posAim - posCurrent;
    float normRawMove = (posAim - posCurrent).norm();

    if (pureRotation) // Only update orientation
    {
        float rotationTheta = normalizeAngle(posAim._theta - posCurrent._theta);
        // If the orientation is unchanged or a move is needed
        if ((abs(rotationTheta) < epsilonOrientation) or (normRawMove > epsilonPosition))
        {
            return 1;
        }

        posAim._theta += rotationTheta;
        posAim.normalizeTheta();

        durationTrajectory = ((abs(posAim._theta - posCurrent._theta)) / cruisingSpeed) + (cruisingSpeed / speedRamps);
        speedProfileRotation.set(speedRamps, speedRamps, cruisingSpeed, durationTrajectory);
        return 0;
    }
    else
    {
        // If there is no move to do
        if (normRawMove < epsilonPosition)
        {
            return 1;
        }

        // Define Trajectory
        float x0 = posCurrent._x;
        float y0 = posCurrent._y;
        float x3 = posAim._x;
        float y3 = posAim._y;
        float x1 = x0 + deltaCurve * normRawMove * cos(posCurrent._theta);
        float y1 = y0 + deltaCurve * normRawMove * sin(posCurrent._theta);
        float x2 = x3 - deltaCurve * normRawMove * cos(posAim._theta);
        float y2 = y3 - deltaCurve * normRawMove * sin(posAim._theta);

        trajectory_X.set(x0, 3.0 * (x1 - x0), 3.0 * (x0 - 2 * x1 + x2), 3.0 * x1 - x0 - 3.0 * x2 + x3);
        trajectory_Y.set(y0, 3.0 * (y1 - y0), 3.0 * (y0 - 2 * y1 + y2), 3.0 * y1 - y0 - 3.0 * y2 + y3);

        // Define lengthTrajectory
        Polynome SpeedX_e = Derivative_ptr(&trajectory_X);
        Polynome SpeedY_e = Derivative_ptr(&trajectory_Y);
        speedSquare_e = Sum(Square_ptr(&SpeedX_e), Square_ptr(&SpeedY_e));

        lengthTrajectory = 0.0; // [...] = cm
        float t_e_integral = 0.0, deltaIntegral = 0.0005;
        float V_e = sqrt(speedSquare_e.f(t_e_integral));
        float lastV_e = sqrt(speedSquare_e.f(t_e_integral));

        while (t_e_integral < 1.0)
        {
            lastV_e = V_e;
            t_e_integral += deltaIntegral;
            V_e = sqrt(speedSquare_e.f(t_e_integral));
            lengthTrajectory += ((V_e + lastV_e) / 2.0) * deltaIntegral;
        }

        // Determine duration of trajectory given speed profile and trajectory's length
        durationTrajectory = (lengthTrajectory / cruisingSpeed) + (cruisingSpeed / speedRamps);
        speedProfileLinear.set(speedRamps, speedRamps, cruisingSpeed, durationTrajectory);

        return 0;
    }
}

void Ghost::lock(bool state)
{
    locked = state;
}

int Ghost::failSafe_position()
{
    float normMove = (posCurrent - posPrevious).norm();
    if (normMove > deltaPositionMax)
    {
        locked = true;
        moving = false;
        posCurrent = posPrevious;
        return 1;
    }
    else
    {
        return 0;
    }
}

int Ghost::ActuatePosition(float dt)
{
    int errorStatus = 0;

    t += dt;

    if ((!locked) and (t_e < 1.0))
    {
        if (rotating)
        {
            //Serial.println("Rotating");
        }
        else
        {
            float speed_e = sqrt(speedSquare_e.f(t_e)); // Virtual speed - associated to Bezier curves
            float speed = speedProfileLinear.f(t);      // Real (wanted) speed


            if (speed_e != 0.0)
            {
                t_e += (speed / speed_e) * dt;
            }
            else
            {
                errorStatus = 1;
            }

            t_e = ((t_e > 1.0) ? 1.0 : t_e);

            posPrevious = posCurrent;

            posCurrent._x = trajectory_X.f(t_e);
            posCurrent._y = trajectory_Y.f(t_e);
            posCurrent._theta = atan2(trajectory_Y.df(t_e), trajectory_X.df(t_e));
        }

        moving = true;
    }
    else // Bot is locked in position, don't move
    {
        moving = false;
        errorStatus = 1;
        //Serial.println("Fail");
    }

    //errorStatus = failSafe_position();
    return errorStatus;
}