#ifndef Lidar2022_H
#define Lidar2022_H

#include "Arduino.h"
#include "Vector.h"
#include "RPLidar.h"
#include "Robot.h"

#define dist_arret 450
#define max_dist_regard 2000 //max distance a regarder

struct point{
    float angleLidar;
    float distanceLu;
};

Vector coordonneeRelativePointDetecte(point p);
Vector coordonneeAbsolueParRelative(Vector vRelatif, Robot* robot);
bool pointInTable(Vector pV);

class Lidar2022
{
public:
    Lidar2022();
    Lidar2022(Robot* r);
    void Begin(HardwareSerial &serialobj);
    void detect();
    bool getDetectClose(){return detecting_a_point_close;};
    bool getRedemarrage(){return redemarrage;};
private:
    RPLidar lidar;
    bool detecting_a_point_close = false;
    bool redemarrage=false;
    Robot* robot;
};

#endif