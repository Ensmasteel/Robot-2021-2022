#include "RobotSimu.h"

RobotSimu::RobotSimu(float xIni ,float yIni ,float thetaIni,TeamColor tc, Stream* commPortStream, Stream* actuPort) : Robot(xIni,yIni,thetaIni,tc,commPortStream,actuPort){
    simu = Simulator(0.30, 9.0, 6.5, 1.5, &cinetiqueCurrent, &motorLeft.order, &motorRight.order);
    Logger::infoln("SIMULATOR MODE");
}

void RobotSimu::Update_Cinetique(float dt){
    simu.updateCinetique(dt);
}