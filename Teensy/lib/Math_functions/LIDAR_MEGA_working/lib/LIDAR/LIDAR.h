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
#include "RPLidar.h"

const uint8_t nb_points_max = 40;
const uint8_t nb_objects_max = 100;
const uint8_t nb_objects_max_filtered = 10;

struct point
{
    float x = 0;
    float y = 0;
};

point operator+(point pt1,point pt2);
point operator-(point pt1,point pt2);

struct obj
{
    point CG;
    point speed;
    uint64_t time = 0;
    bool state = false;
    uint8_t nbpoint = 0;
};

const obj empty_object;


float dist(point p1,point p2);
float dist(point p1);
float angle(point p1);

float dist(obj o1,obj o2);
float dist(obj o1);
float angle(obj o1);
float angle_deg(obj o1);

class object_data
{
public:
    point points[nb_points_max];//stock all the points constituing this object
    uint16_t nbpoint = 0;//number of points
    uint64_t time = 0; // age of the last detection of the object
    point CG;
    bool state = false; //if true the object is tracked, if false object inexistant
    point calculateCG();
    bool addpoint(point pt,float dist_max = 200);
    void show(HardwareSerial &serialobj);
};


class LIDAR : public RPLidar
{
public:
    uint32_t baudRate = 115200;
    uint32_t  baudRate_com = 115200;
    uint32_t  motor_pin = 3;


    void init_com(HardwareSerial &serialobj);
    bool init(HardwareSerial &serialobj);
    void show_data();
    void scan();
    void show_objects();
    void filter_objects(int dmin = 250, int dmax = 2000,int nb_points_min = 2);
    void calcul_speed();
    void show_objects_filtered();
    void plot();
    void plot_biggest();
    void plot_closest();
    void order_by(char16_t methode = 'd');
    
private:
    object_data current_obj;
    obj list_object[nb_objects_max];
    obj list_object_filtered_prev[nb_objects_max_filtered];
    obj list_object_filtered[nb_objects_max_filtered];
    obj list_object_ordered[nb_objects_max_filtered];
    uint16_t index = 0;
    uint16_t index_filtered = 0;
    uint16_t index_ordered = 0;
    int distance[nb_objects_max_filtered];

protected:
    bool com = false;
    HardwareSerial * serial_com;  
};



#endif
