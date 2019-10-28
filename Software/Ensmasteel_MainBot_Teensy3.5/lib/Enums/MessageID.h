#ifndef MESSAGEID_H_
#define MESSAGEID_H_
#include <stdint.h>


//En-tete d'un message
enum MessageID : uint16_t
{
    Empty,
    Stop,
    Tirette,
    PID_T_P_incr_M,
    PID_T_P_decr_M,
    PID_T_I_incr_M,
    PID_T_I_decr_M,
    PID_T_D_incr_M,
    PID_T_D_decr_M,
    PID_R_P_incr_M,
    PID_R_P_decr_M,
    PID_R_I_incr_M,
    PID_R_I_decr_M,
    PID_R_D_incr_M,
    PID_R_D_decr_M,
    add_forward_M,
    add_backward_M,
    add_spin_M
};

#endif