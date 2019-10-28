#ifndef MESSAGEID_H_
#define MESSAGEID_H_
#include <stdint.h>


//En-tete d'un message
enum MessageID : uint16_t
{
    Empty,
    Stop,
    Tirette,
    PID_tweak_M,
    add_forward_M,
    add_backward_M,
    add_spin_M
};

#endif