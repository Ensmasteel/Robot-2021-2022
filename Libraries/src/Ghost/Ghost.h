#ifndef Ghost
#define Ghost

#include "Math_functions.h"
#include "Vector.h"

class Ghost {
public:
    VectorE posCurrent;  // VectorE : struct type containing X,Y,Orientation
    bool locked=true, moving=false;    // locked=true => no movement allowed ; moving=true => trajectory not ended
    Polynome trajectory_X, trajectory_Y, ; // Bezier curves, en fonction de t*

    float t=0.0, t_e=0.0; // t : time since new trajectory setup ; 0<t_e<1 virtual time of Bezier curves 

    //Ghost();
    Ghost(VectorE posEini);

    int rotate(float theta); // Rotation of _theta_rad
    int actuate(float dt);   // Update position with trajectory
    void Set_Trajectory(Polynome newTrajectoryX, Polynome newTrajectoryY); // store new trajectories
    void Compute_Trajectory(VectorE posFinal, float deltaCurve);
    void lock();
};

#endif