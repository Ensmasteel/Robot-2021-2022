/**   Ensmasteel Library - LIDAR management
 * note : Exploitation of LIDAR's data : collision avoidance, trajectory optimisation
 * author : EnsmaSteel
 * date : November 2019
*/

//points to check :
//use


#ifndef LIDAR_H
#define LIDAR_H

#include "Arduino.h"
#include "Vector.h"
#include "RPLidar.h"


#define radius_tracking 10 //the radius in witch 2 objects are concidered the sames
#define max_sep 5 //the maximum separation between 2 points of the same object
#define max_dt 500//the maximum time between 2 detections of an object
#define max_dist 2000 //the maximum distance concidered for detecting an object

#define max_points_per_object 50
#define max_objects_detected 20


/*
Serial.write(val)
Serial.write(str)
Serial.write(buf, len)
*/



//basics:
//1 detect objects
//2 identify objects

/**
 * Struct building a point with angle and distance from robot.
 */
struct point
{
    float angle;
    float distance;
};

/**
 * Calculates the distance between two points.
 * @param p1 : Point, first point
 * @param p2 : Point, first point
 * @return float, distance between the two.
 */
float dist(point p1,point p2);

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

/**
 * Enable to save data about abjects.
 */
class object_data
{
public:
    //data about the objects
    BotIdentificater Bot_name = unknown;
    Cinetique Bot_cinetique;
    point points[max_objects_detected];///<previous points associated to this object
    int length = 0;
    uint32_t age; ///< age of the last detection of the object
    bool state; ///<if true the object is tracked, if false object inexistant
    void calculateCG();
};

float dist(object_data obj1,object_data obj2);

class LIDAR
{
public:

    // GOAL / Constructor
    // IN   / bool secondaryBot : An ally is on the table => true
    /**
     * Public constructor
     * @param secondaryBot : if true, an ally is on table.
     */
    LIDAR(bool secondaryBot = false);


    //Serial conection with Lidar
    /**
     * Serial connection with LIDAR
     * @param serialobj
     */
    void Begin(HardwareSerial &serialobj);

    uint8_t botDetected = 0; ///<Number of enemy bot detected by the LIDAR


    // GOAL / Give speed and position of a specific bot identify by its identificater
    // IN   / BotIdentificater id
    // OUT  / Cinetique : speed and position of _id_ bot
    /**
     * Gives speed and position of specific bot identified by its identifier.
     * @param id
     * @return
     */
    Cinetique get_Cinetique(BotIdentificater id);

    // GOAL / Warn against incoming collision
    // IN   / bool forwar : true => watch in front of the bot
    //                      false => watch behind
    // OUT  / bool : Trajectory blocked => true
    //               Free to move => false
    /**
     * Warn against incoming collision.
     * @param forward : Bool, indicates if the LIDAR watches in front of the bot or not.
     * @return : bool, if trajectory blocked => true, if not => false.
     */
    bool trajectoryBlocked(bool forward = true);

    void detect(); ///<detect all objects (cluster of points being in range of each others)

    void identify();///<identify detected object initially

    void track(); ///<identify detected object considering previous objects

private:

    bool enemy1 = false ,enemy2 = false;

    bool SECONDARY_BOT_ALLY = false;

    RPLidar lidar;

    //liste of all detected object
    object_data object_list [max_objects_detected];
    uint16_t nb_object = 0;//number of objects detected

    bool detectinging_an_object = false;//used to know if the lidar is curently detecting an object
};








#endif
