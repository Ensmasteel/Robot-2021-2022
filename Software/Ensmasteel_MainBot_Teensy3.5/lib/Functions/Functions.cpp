#include "Functions.h"
#include "Vector.h"
#include "Ghost.h"
#include "Sequence.h"
#include "Communication.h"
#include "Actions.h"
#include "PID.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter" //Retire le warning "unused parameter"

//Lorsque cette fonction est appellé, il y a un PID_tweak dans la mailbox
void PID_tweak(Cinetique * robotCinetique, Ghost * ghost, Sequence * mainSequence, Communication * communication, Asservissement * asser) {
    Decoder decoder;
    decoder.data=communication->peekOldestMessage().data;
    bool incr=decoder.raw.byte1==1;
    bool translation=decoder.raw.byte2==1;
    uint8_t whichOne=decoder.raw.byte3; //0 = P, 1 = I, 2 = D
    Logger::infoln(String(asser->tweak(incr,translation,whichOne)*4095.0));
}

#pragma GCC diagnostic pop