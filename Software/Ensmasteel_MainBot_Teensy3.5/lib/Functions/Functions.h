#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
class Cinetique;
class Ghost;
class Sequence;
class Communication;
class Asservissement;

void PID_T_P_incr(Cinetique * robotCinetique,Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser);
void PID_T_P_decr(Cinetique * robotCinetique,Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser);
void PID_T_I_incr(Cinetique * robotCinetique,Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser);
void PID_T_I_decr(Cinetique * robotCinetique,Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser);
void PID_T_D_incr(Cinetique * robotCinetique,Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser);
void PID_T_D_decr(Cinetique * robotCinetique,Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser);
void PID_R_P_incr(Cinetique * robotCinetique,Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser);
void PID_R_P_decr(Cinetique * robotCinetique,Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser);
void PID_R_I_incr(Cinetique * robotCinetique,Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser);
void PID_R_I_decr(Cinetique * robotCinetique,Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser);
void PID_R_D_incr(Cinetique * robotCinetique,Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser);
void PID_R_D_decr(Cinetique * robotCinetique,Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser);
void add_forward(Cinetique * robotCinetique,Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser);
void add_backward(Cinetique * robotCinetique,Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser);
void add_spin(Cinetique * robotCinetique,Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser);



#endif // !FUNCTIONS_H_