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

    VectorE posCurrent, posPrevious, posAim; // VectorE : struct type containing X,Y,Orientation
    bool locked = true;                      // locked=true => no movement allowed
    bool moving = false;                     // moving=true => trajectory not ended
    bool rotating = false;                   // rotating=true => the robot is doing a pure rotation
    Polynome trajectory_X, trajectory_Y;     // Bezier curves, function of t*
    Polynome speedSquare_e;                  // (V*)^2 Evolution speed of the theorical trajectory
    Trapezoidal_Function speedProfileLinear, speedProfileRotation;
    float durationTrajectory;

    // METHODES //

    int ActuatePosition(float dt); // Update position with trajectory
    int Set_NewTrajectory(Polynome newTrajectoryX,
                          Polynome newTrajectoryY,
                          Trapezoidal_Function newSpeed); // store new trajectories
    void Compute_Trajectory(VectorE posFinal, float deltaCurve);
    void lock();

private:
    float t = 0.0, t_e = 0.0; // t : time since new trajectory setup ; 0<t_e<1 virtual time of Bezier curves

    static const float epsilonPosition = 0.001;
    static const float epsilonOrientation = 0.001;
    static const float deltaPositionMax = 1.0;

    int failSafe_position(); // Cancel comming movement if teleportation (movement > deltaPositionMax)
};

#endif
