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

#define AVG 0
#define AVD 1
#define ARG 2
#define ARD 3
bool outOfBound(Vector v, Vector* out){ //Dit si le vector est out of bound et donne la correction
    if (v._x<0){
        out->_x=-v._x; out->_y=0;
        return true;
    }
    if (v._x>3){
        out->_x=3.0-v._x;out->_y=0;
        return true;
    }
    if (v._y<0){
        out->_x=0;out->_y=-v._y;
        return true;
    }
    if (v._y>2){
        out->_x=0;out->_y=2.0 - v._y;
        return true;
    }
}

void Simulator::computeCollision(){
    Vector coins[4];
    coins[AVG] = (Vector)*cinetique + Vector(-size/2.0,size/2.0).rotate(cinetique->_theta);
    coins[AVD] = (Vector)*cinetique + Vector(size/2.0,size/2.0).rotate(cinetique->_theta);
    coins[ARG] = (Vector)*cinetique + Vector(-size/2.0,-size/2.0).rotate(cinetique->_theta);
    coins[ARD] = (Vector)*cinetique + Vector(size/2.0,-size/2.0).rotate(cinetique->_theta);
    Vector out;
    for (int i=0;i<4;i++)
    {
        if (outOfBound(coins[i],&out))
        {   
            cinetique->_x = ((Vector)(*cinetique) + out)._x;
            cinetique->_y = ((Vector)(*cinetique) + out)._y;
            return;
        }
    }
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
    //computeCollision();
}