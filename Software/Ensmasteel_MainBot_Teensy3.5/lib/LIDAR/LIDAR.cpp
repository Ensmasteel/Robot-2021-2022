#include "LIDAR.h"
#include "Ghost.h"
#include "Arduino.h"
#include "RPLidar.h"


#define delta_distance = 100;
#define delta_angle = 10;

#define dist_secondaryBot = 400;
#define angle_secondaryBot = 90;

#define dist_enemyBot = 1800;
#define angle_enemyBot = 0;


LIDAR::LIDAR(bool secondaryBot) // Constructor
{
    SECONDARY_BOT_ALLY = secondaryBot;
}


void Begin(HardwareSerial &serialobj = Serial)
{
    return lidar.begin(&serialobj);

    object_list[0] = enemyBot1;
    object_list[0] = enemyBot2;
    object_list[0] = allyBot2; 
}

Vector get_Vector(BotIdentificater id);

Cinetique LIDAR::get_Cinetique(BotIdentificater id)
{
    Cinetique out;

    return out;
}

bool LIDAR::trajectoryBlocked(bool forward)
{
    bool out;

    return out;
}


void detect()
{
    if (IS_OK(lidar.waitPoint())) { //take the data from rhe lidar
        float distance = lidar.getCurrentPoint().distance;
        float angle = lidar.getCurrentPoint().angle;
    }

    if (distance<max_dist){
        //if object not too far reference the object

        if (abs(distance-dist_secondaryBot)<delta_distance && abs(angle-angle_secondaryBot)<delta_angle)
        {
            //secondaryBot detected
        }
        else if(abs(distance-dist_secondaryBot)<delta_distance && abs(angle-angle_secondaryBot)<delta_angle)
        {
            //enemyBot detected, need to see if there are 2
        }
    }


}



void track()
{
    if (IS_OK(lidar.waitPoint())) { //take the data from rhe lidar
        float distance = lidar.getCurrentPoint().distance;
        float angle = lidar.getCurrentPoint().angle;
    }

    if (){
        //if object not too far search corespondance with all previous objects
    }


}