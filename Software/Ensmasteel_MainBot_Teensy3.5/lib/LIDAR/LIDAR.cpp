#include "LIDAR.h"
#include "Ghost.h"
#include "Arduino.h"
#include "RPLidar.h"

//points to check :
//math fonctions : cos, sin , pow , sqrt


#define delta_distance = 100;
#define delta_angle = 10;

#define dist_secondaryBot = 400;
#define angle_secondaryBot = 90;

#define dist_enemyBot = 1800;
#define angle_enemyBot = 0;


float dist(point p1,point p2)
{
    return sqrt(pow((p1.distance+p2.distance)/2*abs(p1.angle-p2.angle),2) + pow(p1.distance-p2.distance,2)); //big aproximation 
    //work for small angles
};


void object_data::calculateCG()
{
    float x = 0.0,y = 0.0;
    for(int i=0;i < length;i++){
        x+= points[i].distance*cos(points[i].angle);
        y+= points[i].distance*sin(points[i].angle);
    }
    Bot_cinetique._x = x/length;
    Bot_cinetique._y = y/length;
};



LIDAR::LIDAR(bool secondaryBot) // Constructor
{
    SECONDARY_BOT_ALLY = secondaryBot;
};


void LIDAR::Begin(HardwareSerial &serialobj = Serial)
{
    lidar.begin(serialobj);
};

Vector get_Vector(BotIdentificater id);

Cinetique LIDAR::get_Cinetique(BotIdentificater id)
{
    Cinetique out;

    return out;
};

bool LIDAR::trajectoryBlocked(bool forward)
{
    bool out;

    return out;
};


void LIDAR::detect()
{
    if (IS_OK(lidar.waitPoint())) { //take the data from the lidar
        float distance = lidar.getCurrentPoint().distance;
        float angle = lidar.getCurrentPoint().angle;
        point p = {angle,distance};

        if(detectinging_an_object){
            point previous_p = object_list[nb_object-1].points[object_list[nb_object].length-1];

            if (dist(p,previous_p)<max_sep){
                //if an object is being detected and the curent point is close enougth to the last point
                object_list[nb_object-1].points[object_list[nb_object-1].length] = p;
                object_list[nb_object-1].length++;
                object_list[nb_object-1].age = millis();
                //add a new point to the curent object            
            }

            else
            {
                detectinging_an_object = false;
                 object_list[nb_object-1].calculateCG();
                //else terminate the object
            }
        }

        if (distance<max_dist && !detectinging_an_object){
            object_list[nb_object].state = true;
            object_list[nb_object].points[0] = p;

            object_list[nb_object].length++;
            nb_object ++;
            detectinging_an_object = true;
            //if new object detected, creat new object   
        }


    }

};

void track()
{



};