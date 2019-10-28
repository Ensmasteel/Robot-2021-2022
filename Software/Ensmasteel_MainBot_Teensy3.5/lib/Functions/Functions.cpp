#include "Functions.h"
#include "Vector.h"
#include "Ghost.h"
#include "Sequence.h"
#include "Communication.h"
#include "Actions.h"
#include "PID.h"

void PID_T_P_incr(Cinetique * robotCinetique, Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser) {
    Logger::infoln(String(asser->tweak(true,true,true,false,false)));
}

void PID_T_P_decr(Cinetique * robotCinetique, Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser) {
    Logger::infoln(String(asser->tweak(false,true,true,false,false)));
}

void PID_T_I_incr(Cinetique * robotCinetique, Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser) {
    Logger::infoln(String(asser->tweak(true,true,false,true,false)));
}

void PID_T_I_decr(Cinetique * robotCinetique, Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser) {
    Logger::infoln(String(asser->tweak(false,true,false,true,false)));
}

void PID_T_D_incr(Cinetique * robotCinetique, Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser) {
    Logger::infoln(String(asser->tweak(true,true,false,false,true)));
}

void PID_T_D_decr(Cinetique * robotCinetique, Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser) {
    Logger::infoln(String(asser->tweak(false,true,false,false,true)));
}



void PID_R_P_incr(Cinetique * robotCinetique, Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser) {
    Logger::infoln(String(asser->tweak(true,false,true,false,false)));
}

void PID_R_P_decr(Cinetique * robotCinetique, Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser) {
    Logger::infoln(String(asser->tweak(false,false,true,false,false)));
}

void PID_R_I_incr(Cinetique * robotCinetique, Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser) {
    Logger::infoln(String(asser->tweak(true,false,false,true,false)));
}

void PID_R_I_decr(Cinetique * robotCinetique, Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser) {
    Logger::infoln(String(asser->tweak(false,false,false,true,false)));
}

void PID_R_D_incr(Cinetique * robotCinetique, Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser) {
    Logger::infoln(String(asser->tweak(true,false,false,false,true)));
}

void PID_R_D_decr(Cinetique * robotCinetique, Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser) {
    Logger::infoln(String(asser->tweak(false,false,false,false,true)));
}


#define PACE standard

void add_forward(Cinetique * robotCinetique, Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser) {
    mainSequence->add(new Forward_Action(10,0.30,PACE,NO_REQUIREMENT));
}

void add_backward(Cinetique * robotCinetique, Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser) {
    mainSequence->add(new Backward_Action(10,0.30,PACE,NO_REQUIREMENT));
}

void add_spin(Cinetique * robotCinetique, Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser) {
    mainSequence->add(new Rotate_Action(10,PI/2.0,PACE,NO_REQUIREMENT));
}
