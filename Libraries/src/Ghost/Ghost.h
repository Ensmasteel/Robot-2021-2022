#ifndef GHOST_H
#define GHOST_H

#include "Math_functions.h"
#include "Vector.h"

class Ghost
{
public:
    Ghost(VectorE posEini);

    // VARIABLES //

    VectorE posCurrent, posAim;          // VectorE : struct type containing X,Y,Orientation
    bool locked = true;                  // locked=true => no movement allowed
    bool moving = false;                 // moving=true => trajectory not ended
    Polynome trajectory_X, trajectory_Y; // Bezier curves, function of t*
    Polynome speedSquare_e;              // (V*)^2 Evolution speed of the theorical trajectory
    Trapezoidal_Function speedProfileLinear, speedProfileRotation;
    float durationTrajectory;
    float t = 0.0, t_e = 0.0; // t : time since new trajectory setup ; 0<t_e<1 virtual time of Bezier curves

    // METHODES //

    int rotate(float rotationTheta);                                                                         // Rotation of _theta_rad
    int actuate(float dt);                                                                                   // Update position with trajectory
    void Set_NewTrajectory(Polynome newTrajectoryX, Polynome newTrajectoryY, Trapezoidal_Function newSpeed); // store new trajectories
    void Compute_Trajectory(VectorE posFinal, float deltaCurve);
    void lock();
};

#endif