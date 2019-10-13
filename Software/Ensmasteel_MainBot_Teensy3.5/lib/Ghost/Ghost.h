#ifndef GHOST_H
#define GHOST_H

#include "Arduino.h"
#include "Vector.h"
#include "Math_functions.h"

class Ghost
{
public:
    Ghost(VectorE posEini);

    // VARIABLES //

    float t = 0.0, t_e = 0.0;                // t : time since new trajectory setup ; 0<t_e<1 virtual time of Bezier curves
    VectorE posCurrent, posPrevious, posAim; // VectorE : struct type containing X,Y,Orientation
    bool locked = true;                      // locked=true => no movement allowed
    bool moving = false;                     // moving=true => trajectory not ended
    bool rotating = false;                   // rotating=true => the robot is doing a pure rotation
    Polynome trajectory_X, trajectory_Y;     // Bezier curves, function of t*
    Polynome speedSquare_e;                  // (V*)^2 Evolution speed of the theorical trajectory
    Trapezoidal_Function speedProfileLinear; // wanted speed of the bot along the trajectory [...] = cm/s
    Trapezoidal_Function speedProfileRotation; // wanted speed in rotation [...] = rad/s
    float durationTrajectory;   // [...] = s

    // METHODES //

    // GOAL / Calculate next position of the robot along the trajectory in memory
    // IN   / float dt : Duration since last call of the function - Keep track if real time
    // OUT  / int error : 0 if calculation completed
    //                    1 if bot locked in position or posAim reached
    //      / VectorE posCurrent
    int ActuatePosition(float dt);

    void Set_NewTrajectory(Polynome newTrajectoryX, Polynome newTrajectoryY, Trapezoidal_Function newSpeed); // store new trajectories

    // GOAL / Compute the new trajectory of the bot to reach posFinal
    //      / To execute a pure rotation set pureRotation to True and set posFinal to posCurrent except for _theta
    // IN   / VectorE posFinal
    //      / float deltaCurve, speedRamps, cruisingSpeed : speed parameters of the trajectory [speedRamps] = cm/s^2 ; [cruisingSpeed] = cm/s
    // OUT  / int error : 0 if calculation completed
    //                    1 if no movement needed i.e. distance and orientation to the aimed position less than epsilon
    //      / Polynome trajectory_X, trajectory_Y, speedSquare_e
    int Compute_Trajectory(VectorE posFinal, float deltaCurve, float speedRamps, float cruisingSpeed, bool pureRotation);
    void lock(bool state);

private:
    const float epsilonPosition = 0.01;
    const float epsilonOrientation = 0.01;
    const float deltaPositionMax = 0.1;

    int failSafe_position(); // Cancel comming movement if teleportation (movement > deltaPositionMax)
};

#endif
