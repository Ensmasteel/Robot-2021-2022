#include "RobotSimu.h"

RobotSimu::RobotSimu(float xIni ,float yIni ,float thetaIni ,Stream* commPort ) : Robot(xIni,yIni,thetaIni,commPort){
    simu = Simulator(0.30, 10.0, 6.5, 1.5, &cinetiqueCurrent, &motorLeft.order, &motorRight.order);
    Logger::infoln("SIMULATOR MODE");
}

void RobotSimu::Update_Cinetique(float dt){
    simu.updateCinetique(dt);
}