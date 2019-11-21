/**   Ensmasteel Library - movement management
 * 
 * author : Arthur FINDELAIR - EnsmaSteel, github.com/ArthurFDLR
 * date : October 2019
*/

#ifndef GHOST_H
#define GHOST_H

#include "Arduino.h"
#include "Vector.h"
#include "Math_functions.h"

class Ghost
{
public:
    Ghost(VectorE posEini = VectorE(0.0, 0.0, 0.0));
    //Ghost() {}

    Cinetique cinetiqueController; // updated at each call of ActuatePosition, stores delayed position and current speed

    // ===    METHODES    ===
    // ======================

    // GOAL / IsVar() gives Var status
    // OUT  / bool Status
    bool IsLocked();
    bool IsRotating();
    bool IsBackward();
    bool trajectoryIsFinished();
    
    // GOAL / Setter function of locked variable, true => robot won't move
    // IN   / bool state
    // PREREQUIRE / Please brake the robot to avoid hicups
    void Lock(bool state);

    // GOAL / Calculate next position of the robot along the trajectory in memory
    // IN   / float dt : Duration since last call of the function - Keep track if real time
    // OUT  / int error : 0 if calculation completed
    //                    1 if bot locked in position or posAim reached
    //      / VectorE posCurrent
    int ActuatePosition(float dt);

    // GOAL / Compute the new trajectory of the bot to reach posFinal
    //      / To execute a pure rotation set pureRotation to True and set posFinal to posCurrent except for _theta
    // IN   / VectorE posFinal
    //      / float deltaCurve, speedRamps, cruisingSpeed : speed parameters of the trajectory [speedRamps] = cm/s^2 ; [cruisingSpeed] = cm/s
    //      / bool pureRotation : true if a simple rotation is wanted
    //      / bool backward     : true if the robot is going backaward
    // OUT  / int error : 0 if calculation completed
    //                    1 if no movement needed i.e. distance and orientation to the aimed position less than epsilon
    //      / Polynome trajectory_X, trajectory_Y, speedSquare_e
    int Compute_Trajectory(VectorE posFinal, float deltaCurve, float speedRamps, float cruisingSpeed, bool pureRotation = false, bool backward = false);

    // GOAL / Teleport the ghost to _newPos_
    // IN   / VectorE newPos OR Cinetique newPos : only consider position
    // OUT  / The position of the ghost is set to VectorE. Speed are not affected by this action
    void moveGhost(VectorE newPos);
    void moveGhost(Cinetique newPos);

    Cinetique Get_Controller_Cinetique();

private:
    // ===    PARAMETERS    ===
    // ========================
    float MIN_MOVEMENT = 0.005; // Minimal distance to allow trajectory computation
    float MIN_ROTATION = 0.1*DEG_TO_RAD; // Minimal angle to allow trajectory computation
    float DELAY_POSITION = 40.0; // [...] = ms, Delay between posCurrent and posDelayed
    float MAX_SPEED = 50.0; // ... if linear speed above the limit, block movement
    float MAX_DISTANCE = 5.0; // ... if distance between to consecutive position, block movement 

    // ===    VARIABLES    ===
    // =======================
    VectorE posCurrent, posPrevious, posAim;   // VectorE : struct type containing X,Y,Orientation
    VectorE posDelayed;                        // Position _delayPosition_ ms ago. Used as input for the position controller
    Polynome trajectory_X, trajectory_Y;       // Bezier curves, function of t*
    Polynome speedSquare_e;                    // (V*)^2 Evolution speed of the theorical trajectory
    Trapezoidal_Function speedProfileLinear;   // wanted speed of the bot along the trajectory [...] = cm/s
    Trapezoidal_Function speedProfileRotation; // wanted speed in rotation [...] = rad/s

    float t = 0.0, t_delayed = 0.0;                             // t : time since new trajectory setup
    float t_e = 0.0, t_e_delayed = 0.0;          // 0<t_e<1 virtual time of Bezier curves
    float durationTrajectory = 0.0, lengthTrajectory = 0.0;       // [...] = s ; [...] = (rotating ? rad : cm)
    float speedLinearCurrent = 0.0, speedRotationalCurrent = 0.0; // Current speeds

    // ===    STATE    ===
    // ===================
    bool locked = true;              // locked=true => no movement allowed
    bool rotating = false;           // rotating=true => the robot is doing a pure rotation
    bool backward = false;           // backward=true => the robot is going backward
    bool trajectoryFinished = false; // trajectoryFinished=true => the robot has reached posAim

    // ===    PRIVATE METHODES    ===
    // ==============================

    // GOAL / Determine linear and rotational speed between to positions
    // IN   / VectorE posNow, posLast
    //      / float dt : delay between posNow and posLast
    // OUT  / float : speedLinearCurrent, speedRotationalCurrent
    void Update_Speeds(VectorE posNow, VectorE posLast, float dt);
    void Set_NewTrajectory(Polynome newTrajectoryX, Polynome newTrajectoryY, Trapezoidal_Function newSpeed); // store new trajectories
    int StateManager(); // Cancel coming movement if teleportation (movement > deltaPositionMax)
};

#endif
