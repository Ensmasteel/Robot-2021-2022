#include "Simulator.h"
#define MAX_PERCENT_DEFECT 50

Simulator::Simulator(float size, float mass, float maxAcceleration, float maxSpeed
            , Cinetique * cinetique, float * orderMotorLeft, float * orderMotorRight, float health) {
    this->size=size;
    this->mass=mass;
    this->maxMotorForce=maxAcceleration*mass;
    this->J=mass*size*size/6.0;
    this->friction=maxMotorForce/maxSpeed;
    this->health=health;
    this->cinetique=cinetique;
    this->orderMotorLeft=orderMotorLeft;
    this->orderMotorRight=orderMotorRight;
}

void Simulator::updateCinetique( float dt) {
    float vLeft,vRight;
    vLeft=cinetique->_v - cinetique->_w*size/2;
    vRight=cinetique->_v + cinetique->_w*size/2;
    
    float forceLeft,forceRight;
    forceLeft=(*orderMotorLeft)*maxMotorForce - friction*(1 + (1-health)*random(-MAX_PERCENT_DEFECT,MAX_PERCENT_DEFECT)/100)*vLeft;
    forceRight=(*orderMotorRight)*maxMotorForce - friction*(1 + (1-health)*random(-MAX_PERCENT_DEFECT,MAX_PERCENT_DEFECT)/100)*vRight;

    float force = (forceLeft + forceRight);
    float moment = (forceRight-forceLeft)*size/2;

    cinetique->_v+=force*dt/mass;
    cinetique->_w+=moment*dt/J;

    cinetique->_theta+=cinetique->_w*dt;
    (*cinetique)+=directeur(cinetique->_theta)*cinetique->_v*dt;
}