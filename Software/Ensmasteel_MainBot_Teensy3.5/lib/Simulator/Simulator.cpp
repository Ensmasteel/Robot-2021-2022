#include "Simulator.h"


Simulator::Simulator(float size, float mass, float maxAcceleration, float maxSpeed
            , Cinetique * cinetique, float * orderMotorLeft, float * orderMotorRight) {
    this->size=size;
    this->mass=mass;
    this->maxMotorForce=maxAcceleration*mass;
    this->J=mass*size*size/6.0;
    this->friction=maxMotorForce/maxSpeed;

    this->cinetique=cinetique;
    this->orderMotorLeft=orderMotorLeft;
    this->orderMotorRight=orderMotorRight;
}

void Simulator::updateCinetique( float dt) {
    float vLeft,vRight;
    vLeft=cinetique->_v - cinetique->_w*size/2;
    vRight=cinetique->_v + cinetique->_w*size/2;
    
    float forceLeft,forceRight;
    forceLeft=(*orderMotorLeft)*maxMotorForce - friction*vLeft;
    forceRight=(*orderMotorRight)*maxMotorForce - friction*vRight;

    float force = (forceLeft + forceRight);
    float moment = (forceRight-forceLeft)*size/2;

    cinetique->_v+=force*dt/mass;
    cinetique->_w+=moment*dt/J;

    cinetique->_theta+=cinetique->_w*dt;
    (*cinetique)+=directeur(cinetique->_theta)*cinetique->_v*dt;
}

#ifdef TESTONCOMPUTER
#include <iostream>
void print(Cinetique cin)
{
    std::cout<<"x= "<<cin._x<<"\ty= "<<cin._y<<"\tv= "<<cin._v<<std::endl;
}

void test()
{
    float leftOrder=1;
    float rightOrder=0.9;
    Cinetique cin=Cinetique();
    Simulator sim=Simulator(0.10,2.0,0.5,0.5,&cin,&leftOrder,&rightOrder);
    for (int i=0;i<500;i++)
    {
        sim.letThePhysicsDoItsJob(0.1);
        print(cin);
    }
}

#endif