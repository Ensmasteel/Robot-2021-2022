/**   Ensmasteel Library - LIDAR management
 * note : Exploitation of LIDAR's data : collision avoidance, trajectory optimisation
 * author : EnsmaSteel
 * date : November 2019
*/

#ifndef LIDAR_H
#define LIDAR_H

#include "Arduino.h"
#include "Vector.h"
#include "RPLidar.h"


#define min_radius 10 //the min radius of an objected necessary to be traced by lidar
#define max_sep 5 //the maximum separation between 2 points of the same object
#define max_dt //the maximum time between 2 detections of an object
#define max_dist 2000 //the maximum distance concidered for detecting an object

#define max_points_per_object 50
#define max_objects_detected 20

//first phase:
//when robot are static --> detect them
//second phase:
//when robot are moving --> track them


struct point
{
    float angle;
    float distance;
}

enum BotIdentificater
{
    // How to diferenciate ?
    // folow bot ==> blind spot
    enemyBot1,
    enemyBot2,
    allyBot2,
    tower,
    unknown// if unknow object detected

};

//Lidar continously update object_data ==> will become a class soon
struct object_data
{
    //data about the objects
    BotIdentificater Bot_name = unknown;
    Cinetique Bot_cinetique;

    point old[max_objects_detected];//previous points associated to this object
    int old_length = 0;
    point now[max_objects_detected];//points being associated with this object
    int now_length = 0;

    float age; // age of the last detection of the object
    bool state; //if true the object is tracked
};


class LIDAR
{
public:

    // GOAL / Constructor
    // IN   / bool secondaryBot : An ally is on the table => true
    LIDAR(bool secondaryBot = false);


    //Serial conection with Lidar
    bool Begin(HardwareSerial &serialobj = Serial);

    uint8_t botDetected = 0; //Number of enemy bot detected by the LIDAR


    // GOAL / Give speed and position of a specific bot identify by its identificater
    // IN   / BotIdentificater id
    // OUT  / Cinetique : speed and position of _id_ bot
    Cinetique get_Cinetique(BotIdentificater id);

    // GOAL / Warn against incoming collision
    // IN   / bool forwar : true => watch in front of the bot
    //                      false => watch behind
    // OUT  / bool : Trajectory blocked => true
    //               Free to move => false
    bool trajectoryBlocked(bool forward = true);

    void detect(); //detect all non mooving objects

    void track(); //identify all points too all previously known objects

private:

    bool SECONDARY_BOT_ALLY = false;

    RPLidar lidar;

    //liste of all detected object
    object_data object_list [max_objects_detected];
};



#endif
