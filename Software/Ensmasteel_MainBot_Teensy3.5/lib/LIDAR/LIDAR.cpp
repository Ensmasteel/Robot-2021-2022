#include "LIDAR.h"
#include "Ghost.h"
#include "Arduino.h"

LIDAR::LIDAR(bool secondaryBot) // Constructor
{
    SECONDARY_BOT_ALLY = secondaryBot;
}

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