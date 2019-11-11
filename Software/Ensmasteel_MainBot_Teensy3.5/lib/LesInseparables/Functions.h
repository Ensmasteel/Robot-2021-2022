#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
class Robot;

void PID_tweak(Robot* robot);
void add_backward(Robot* robot);
void add_spin(Robot* robot);
void pauseNlockMainSequence(Robot* robot);
void ping(Robot* robot);
void shutdown(Robot* robot);
void setTimeStart(Robot* robot);
void startBackHomeSeq(Robot* robot);
void setNorth(Robot* robot);
void setSouth(Robot* robot);


#endif // !FUNCTIONS_H_