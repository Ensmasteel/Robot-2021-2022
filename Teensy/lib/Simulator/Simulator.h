#ifndef SIMULATOR_H_
#define SIMULATOR_H_
#include "Vector.h"

/**
 * Class enabling to create a simulator to test the robot's functions.
 */
class Simulator
{
    float mass; ///<Mass
    float J;    ///<Rotational Inertia
    float maxMotorForce;    ///<Newton
    float size; ///<Size of one side of the robot, considering its template is a square. We also consider that the motors are at the extremity
    float friction;  ///<friction  frictionForce=friction*(-V)
    float health;
    Cinetique * cinetique;
    float * orderMotorLeft;
    float * orderMotorRight;

    void computeCollision();

public:
    Simulator(float size, float mass, float maxAcc, float maxSpeed
            , Cinetique * cinetique, float * orderMotorLeft, float * orderMotorRight, float health=1);
    Simulator(){}
    void updateCinetique(float dt);
};

#endif // !SIMULATOR_H_