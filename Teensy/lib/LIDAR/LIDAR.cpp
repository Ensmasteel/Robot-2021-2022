#include "LIDAR.h"
#include "Arduino.h"
#include "RPLidar.h"
#include "Vector.h"

//points to check :
//math fonctions : cos, sin , pow , sqrt
//all distances/speed are relative to the main bot ==> need to kwno postion/ orientation of the bot
//update cinematique LIDAR::Track
//improve reorganisation of object_list
//&serialobj != serialobj
//dist would work better with vetor and norm

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

float dist(object_data obj1, object_data obj2)
{
    //return sqrt(pow(obj1.Bot_cinetique._x-obj2.Bot_cinetique._x,2)+pow(obj1.Bot_cinetique._y-obj2.Bot_cinetique._y,2));
    return obj1.Bot_cinetique.distanceWith(obj2.Bot_cinetique);
};

void object_data::calculateCG()
{
    float x = 0.0,y = 0.0;
    for(int i=0;i < length;i++)
    {
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


void LIDAR::Begin(HardwareSerial &serialobj)
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
        Logger::debugln("distance" + String(distance));
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


#define dist_error 20.0
Vector enemyBot(1500,0); // position x,y of bots
Vector allyBot(0,300); // position x,y of bots
Vector TOWER1(100,100); // position of tower 1

void LIDAR::identify()
{
    for (int i = 0;i<nb_object-1;i++)
    {
        Vector a = enemyBot-object_list[i].Bot_cinetique;
        if(a.norm() < dist_error)
        {   //if the bot detected is in the location we expect an enemy to be
            if(!enemy1)
            {   //if no enemy bot detected yet --> assign bot to bot1
                object_list[i].Bot_name = enemyBot1;
            }
            else if(!enemy2)
            {   //if only one bot detected yet --> assign bot to bot2
                object_list[i].Bot_name = enemyBot2;
            }
            else
            {   //if more than 2 bot detected yet --> assign bot to unknown
                object_list[i].Bot_name = enemyBot2;
            }
        }
        if(SECONDARY_BOT_ALLY)
        {
            a = allyBot-object_list[i].Bot_cinetique;
            if(a.norm() < dist_error)
            {
                object_list[i].Bot_name = allyBot2;
            }
        }
        a = TOWER1-object_list[i].Bot_cinetique;
        if(a.norm() < dist_error)
        {
            object_list[i].Bot_name = allyBot2;
        }
    }
};


void LIDAR::track()
{
    int new_nb_object = nb_object;
    for (int i = 0;i<nb_object-1;i++)
    {
        for (int j = i+1;j<nb_object;j++)
        {
            //if 2 objects are similar, kill the oldest one and update the speed of the new one
            float d = dist(object_list[i],object_list [j]);            {
                uint32_t age1 = object_list[i].age,age2 = object_list[j].age;
                if (age1<age2)
                {
                    object_list[i].state = false; //stop tracking the old version of the bot
                    object_list[j].Bot_name = object_list[i].Bot_name; //the new object take the name of the old one
                    new_nb_object -= 1;
                }
                else
                {
                    object_list[j].state = false; //stop tracking the old version of the bot
                    object_list[i].Bot_name = object_list[j].Bot_name; //the new object take the name of the old one
                    new_nb_object -= 1;
                }
            }
        }
    }
    uint32_t t = millis();
    for (int i = 0;i<nb_object-1;i++)
    {
        //clear all objects too old
        if(t-object_list[i].age>max_dt)
        {
            object_list[i].state = false;
            new_nb_object -= 1;
        }
    }

    for (int i = 0;i<new_nb_object;i++)
    {
        //reorganize object_list
        //if one of the first object isn't tracked, replace it with an other one
        if(object_list[i].state == false){
            for (int j = i+1;j<nb_object;j++)
            {
                if(object_list[j].state == true){
                    object_list[i] = object_list[j];
                    object_list[j].state = false;
                }
            }
        }
    }

    nb_object = new_nb_object;

};