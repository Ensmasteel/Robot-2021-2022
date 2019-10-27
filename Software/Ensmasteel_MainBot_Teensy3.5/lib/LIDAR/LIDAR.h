/**   Ensmasteel Library - LIDAR management
 * note : Exploitation of LIDAR's data : collision avoidance, trajectory optimisation
 * author : EnsmaSteel
 * date : November 2019
*/

#ifndef LIDAR_H
#define LIDAR_H

#include "Arduino.h"
#include "Vector.h"

enum BotIdentificater
{
    enemyBot1,
    enemyBot2,
    allyBot2
};

class LIDAR
{
public:
    // GOAL / Constructor
    // IN   / bool secondaryBot : An ally is on the table => true
    LIDAR(bool secondaryBot = false);

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

private:
    bool SECONDARY_BOT_ALLY = false;
};

#endif
