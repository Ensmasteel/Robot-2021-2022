#ifndef ROBOTSIMU_H_
#define ROBOTSIMU_H_
#include "Simulator.h"
#include "Robot.h"

/**
 * Creates a class that simulates the robot behavior to start the simulator.
 */
class RobotSimu : public Robot
{
private:
    Simulator simu;
    void Update_Cinetique(float dt) override;

public:
    RobotSimu(float xIni=0.0,
              float yIni=0.0,
              float thetaIni=0.0,
              Stream* commPortStream =&Serial,
              Stream *actuPort = &Serial);
};


#endif /* ROBOTSIMU_H_ */
