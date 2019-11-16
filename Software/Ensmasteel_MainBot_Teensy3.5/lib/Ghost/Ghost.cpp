#include "Ghost.h"
#include "Arduino.h"

#ifdef STM32BOTH
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

Ghost::Ghost(VectorE posEIni)
{
    posCurrent = posEIni;
    posPrevious = posEIni;
    posAim = posEIni;
    posDelayed = posEIni;
    locked = true;
    t = 0.0;
    t_e = 0.0;
    t_e_delayed = 0.0;
}

void Ghost::Set_NewTrajectory(Polynome newTrajectoryX, Polynome newTrajectoryY, Trapezoidal_Function newSpeed)
{
    trajectory_X = newTrajectoryX;
    trajectory_Y = newTrajectoryY;
    speedProfileLinear = newSpeed;
    t = 0.0;
    t_e = 0.0;
}

int Ghost::Compute_Trajectory(VectorE posFinal, float deltaCurve, float speedRamps, float cruisingSpeed, bool pureRotation, bool goBackward)
{
    uint8_t errorStatus = 0;
    deltaCurve = max(0.3, deltaCurve); 

    // Set variables state
    posAim = posFinal;
    t = 0.0;
    t_e = 0.0;
    t_e_delayed = 0.0;
    t_delayed = 0.0;
    rotating = pureRotation;
    backward = goBackward;
    trajectoryFinished = false;

    posAim.normalizeTheta();
    posCurrent.normalizeTheta();

    float normRawMove = posCurrent.distanceWith(posAim);

    if (pureRotation) // Only update orientation
    {
        lengthTrajectory = normalizeAngle(posAim._theta - posCurrent._theta);

        // If the orientation is unchanged
        if (abs(lengthTrajectory) < MIN_ROTATION)
        {
            errorStatus = 1;
        }

        //posAim._theta += lengthTrajectory;
        posAim.normalizeTheta();

        durationTrajectory = speedProfileRotation.set(speedRamps, speedRamps, cruisingSpeed, abs(lengthTrajectory));
    }
    else
    {

        if (abs(normRawMove) < MIN_MOVEMENT)
        {
            errorStatus = 1;
        }

        // Define Trajectory
        float x0 = posCurrent._x;
        float y0 = posCurrent._y;
        float x3 = posAim._x;
        float y3 = posAim._y;
        float x2 = x3 - deltaCurve * normRawMove * cos(posAim._theta);
        float y2 = y3 - deltaCurve * normRawMove * sin(posAim._theta);
        float x1;
        float y1;
        if (backward)
        {
            x1 = x0 + deltaCurve * normRawMove * cos(normalizeAngle(posCurrent._theta + PI));
            y1 = y0 + deltaCurve * normRawMove * sin(normalizeAngle(posCurrent._theta + PI));
        }
        else
        {
            x1 = x0 + deltaCurve * normRawMove * cos(posCurrent._theta);
            y1 = y0 + deltaCurve * normRawMove * sin(posCurrent._theta);
        }

        trajectory_X.set(x0, 3.0 * (x1 - x0), 3.0 * (x0 - 2 * x1 + x2), 3.0 * x1 - x0 - 3.0 * x2 + x3);
        trajectory_Y.set(y0, 3.0 * (y1 - y0), 3.0 * (y0 - 2 * y1 + y2), 3.0 * y1 - y0 - 3.0 * y2 + y3);

        // Define lengthTrajectory
        Polynome SpeedX_e = Derivative_ptr(&trajectory_X);
        Polynome SpeedY_e = Derivative_ptr(&trajectory_Y);
        speedSquare_e = Sum(Square_ptr(&SpeedX_e), Square_ptr(&SpeedY_e));

        lengthTrajectory = 0.0; // [...] = m
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
        durationTrajectory = speedProfileLinear.set(speedRamps, speedRamps, cruisingSpeed, lengthTrajectory);
    }

    return errorStatus;
}

bool Ghost::IsLocked()
{
    return locked;
}

bool Ghost::IsRotating()
{
    return rotating;
}

bool Ghost::IsBackward()
{
    return backward;
}

bool Ghost::trajectoryIsFinished()
{
    return trajectoryFinished;
}

void Ghost::Lock(bool state)
{
    locked = state;
}

int Ghost::StateManager()
{
    uint8_t errorState = 0;

    if (speedLinearCurrent > MAX_SPEED)
    {
        locked = true;
        posCurrent = posPrevious;
        errorState = 1;
    }

    if (t_e_delayed > 1.0)
    {
        if (posDelayed.distanceWith(posAim) < MAX_DISTANCE) //If we're not to far from the goal
        {
            if (rotating)
            {
                posCurrent._theta = posAim._theta;
                posDelayed._theta = posAim._theta;
                posPrevious._theta = posAim._theta;
            }
            else
            {
                posCurrent = posAim;
                posDelayed = posAim;
                posPrevious = posAim;
            }

            //t_e_delayed = 1.01; //<-------------- Ca a fix le pb. ARTHUR CHECK PLS -> C'est degeu mais, if it works, it works
            trajectoryFinished = true;
        }
        else
        {
            errorState = 1;
        }
    }
    return errorState;
}

int Ghost::ActuatePosition(float dt)
{
    int errorStatus = 0;
    t += dt;
    t_delayed = ((t > DELAY_POSITION / 1e3) ? t - DELAY_POSITION / 1e3 : 0.0);
    if ((!locked) and (t_e_delayed <= 1.0))
    {
        if (rotating)
        {
            posPrevious = posCurrent;

            posCurrent._theta += speedProfileRotation.f(t) * dt * ((lengthTrajectory > 0) ? 1 : -1);
            posCurrent.normalizeTheta();
            posDelayed._theta += speedProfileRotation.f(t_delayed) * dt * ((lengthTrajectory > 0) ? 1 : -1);
            posDelayed.normalizeTheta();

            t_e = t / durationTrajectory;
            t_e_delayed = t_delayed / durationTrajectory;
        }
        else
        {
            // Determine T_e
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

            // Determine t_e_delayed
            float speed_e_delayed = sqrt(speedSquare_e.f(t_e_delayed)); // Virtual speed - associated to Bezier curves
            float speed_delayed = speedProfileLinear.f(t_delayed);      // Real (wanted) speed

            if (speed_e_delayed != 0.0)
            {
                t_e_delayed += (speed_delayed / speed_e_delayed) * dt;
            }
            else
            {
                errorStatus = 1;
            }

            // t_e_delayed = ((t_e_delayed > 1.0) ? 1.0 : t_e_delayed);

            // Compute positions
            posPrevious = posCurrent;

            posDelayed._x = trajectory_X.f(t_e_delayed);
            posDelayed._y = trajectory_Y.f(t_e_delayed);

            posCurrent._x = trajectory_X.f(t_e);
            posCurrent._y = trajectory_Y.f(t_e);

            if (backward)
            {
                posDelayed._theta = atan2(-trajectory_Y.df(t_e_delayed), -trajectory_X.df(t_e_delayed));
                posCurrent._theta = atan2(-trajectory_Y.df(t_e), -trajectory_X.df(t_e));
            }
            else
            {
                posDelayed._theta = atan2(trajectory_Y.df(t_e_delayed), trajectory_X.df(t_e_delayed));
                posCurrent._theta = atan2(trajectory_Y.df(t_e), trajectory_X.df(t_e));
            }
        }
    }
    else // Bot is locked in position, don't move
    {

        errorStatus = 1;
    }

    Update_Speeds(posCurrent, posPrevious, dt);

    cinetiqueController = Get_Controller_Cinetique();

    float errorFailSafe = StateManager(); //Avoid multiple call
    errorStatus = max(errorFailSafe, errorStatus);
    return errorStatus;
}

void Ghost::Update_Speeds(VectorE posNow, VectorE posLast, float dt)
{
    speedLinearCurrent = (backward ? -1.0 : 1.0) * posNow.distanceWith(posLast) / dt;

    speedRotationalCurrent = normalizeAngle(posNow._theta - posLast._theta) / dt;
}

Cinetique Ghost::Get_Controller_Cinetique()
{
    Cinetique out;

    out._x = posDelayed._x;
    out._y = posDelayed._y;
    out._theta = posDelayed._theta;
    out._v = speedLinearCurrent;
    out._w = speedRotationalCurrent;

    return out;
}

void Ghost::moveGhost(VectorE newPos)
{
    posCurrent = newPos;
    posDelayed = newPos;
    posPrevious = newPos;
}

void Ghost::moveGhost(Cinetique newPos)
{
    posCurrent = VectorE(newPos._x, newPos._y, newPos._theta);
    posDelayed = posCurrent;
    posPrevious = posCurrent;
}
