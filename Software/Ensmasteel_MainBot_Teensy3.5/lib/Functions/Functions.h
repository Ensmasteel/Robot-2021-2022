#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
class Cinetique;
class Ghost;
class Sequence;
class Communication;
class Asservissement;

void PID_tweak(Cinetique * robotCinetique,Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser);
void add_backward(Cinetique * robotCinetique,Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser);
void add_spin(Cinetique * robotCinetique,Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser);
void pauseNlockMainSequence(Cinetique * robotCinetique, Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser);



#endif // !FUNCTIONS_H_