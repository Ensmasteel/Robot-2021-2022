#include "Ghost.h"
#include "Arduino.h"

void Ghost::Ghost(VectorE posEIni){
    posCurrent = posEIni;
    t=0.0;
    t_e=0.0;
}

void Ghost::Set_NewTrajectory(Polynome newTrajectoryX, Polynome newTrajectoryY, Trapezoidal_Function newSpeed){
    trajectoryX = newTrajectoryX;
    trajectoryY = newTrajectoryY;
    speedRamp = newSpeed;
    t = 0.0;
    t_e = 0.0;
}

void Compute_Trajectory(VectorE posFinal, float deltaCurve, float speedRamps, float cruisingSpeed){
    posAim = posFinal;
    float x0 = posCurrent.vec.x;
    float y0 = posCurrent.vec.y;
    float x3 = posAim.vec.x;
    float y3 = posAim.vec.y;
    float x1 = x0 + deltaCurve * cos(posCurrent.theta);
    float y1 = y0 + deltaCurve * sin(posCurrent.theta);
    float x2 = x3 - deltaCurve * cos(posAim.theta);
    float y2 = y3 - deltaCurve * sin(posAim.theta);

    trajectoryX.set(x0, 3.0*(x1-x0), 3.0*(x0-2*x1+x2), 3.0*x1-x0-3.0*x2+x3, 0.0, 0.0, 0.0);
    trajectoryY.set(y0, 3.0*(y1-y0), 3.0*(y0-2*y1+y2), 3.0*y1-y0-3.0*y2+y3, 0.0, 0.0, 0.0);

    
    // calculate and store V*
    // Integrate V* to find travel length
    // Manually inegrate V to find the formula that gives totale duration
    speedRampLinear.set(speedRamps,cruisingSpeed,speedRamps, !!duration!! ); 
}


void Ghost::lock(bool state){
    locked = state;
}