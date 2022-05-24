#include "LIDAR2022.h"
#include "Arduino.h"
#include "RPLidar.h"
#include "Vector.h"
#include "Robot.h"


Vector coordonneeRelativePointDetecte(point p){
    float x= p.distanceLu*cos(p.angleLidar);
    float y= p.distanceLu*sin(p.angleLidar);
    return Vector(x,y);
};

Vector coordonneeAbsolueParRelative(Vector vRelatif, Robot* robot){
    float robot_abs_x=robot->cinetiqueCurrent._x;
    float robot_abs_y=robot->cinetiqueCurrent._y;
    float robot_abs_angle=robot->cinetiqueCurrent.angle();
    //VectorE vector_robot_abs = VectorE(robot_abs_x,robot_abs_y,robot_abs_angle);
    float point_x_abs = robot_abs_x + vRelatif._x * sin(robot_abs_angle) + vRelatif._y * cos(robot_abs_angle);
    float point_y_abs = robot_abs_y + vRelatif._x * cos(robot_abs_angle) + vRelatif._y * sin(robot_abs_angle);
    return Vector(point_x_abs,point_y_abs);
};

bool pointInTable(Vector pV){
    if (pV._x<100 || pV._x> 2900 || pV._y<100 || pV._y>1900){
        return false;
    }
    else{
        return true;
    }
};

Lidar2022::Lidar2022(){

};

Lidar2022::Lidar2022(Robot* r) : Lidar2022(){
    this->robot=r;

};

void Lidar2022::Begin(HardwareSerial &serialobj)
{
    lidar.begin(serialobj);
};

void Lidar2022::detect()
{   
    while(IS_OK(lidar.waitPoint()) && detecting_a_point_close == false){
        float distance = lidar.getCurrentPoint().distance;
        float angle = lidar.getCurrentPoint().angle;
        point p = {angle,distance};
        Vector pVectRel = coordonneeRelativePointDetecte(p);
        Vector pVectAbs = coordonneeAbsolueParRelative(pVectRel,robot);
        if (distance<dist_arret && pointInTable(pVectAbs)==true){
            detecting_a_point_close=true;
        }
        else{
            detecting_a_point_close=false;
        }
    }
};





