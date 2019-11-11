#ifndef MESSAGEID_H_
#define MESSAGEID_H_
#include <stdint.h>


//En-tete d'un message
//-----------------------Merci de rajouter des elements par la FIN-----------------------
enum MessageID : uint16_t
{
    Empty_M,  //Message vide
    Stop_M, //[Teensy -> Aux]: Arret des actionneurs 
    Tirette_M, //[Mega -> Teensy]: Tirette actionnée
    PID_tweak_M, //[Aux -> Teensy]: Changer PID | DATA: 4 bytes [bool incr, bool translation, wichOne (P=0,I=1,D=2)]
    add_forward_M, //[Aux -> Teensy]: Rajoute un forward a la fin de la file principale
    add_backward_M, //[Aux -> Teensy]: Rajoute un backward a la fin de la file principale
    add_spin_M, //[Aux -> Teensy]: Rajoute un spin a la fin de la file principale
    Em_Stop_M, //[Aux -> Teensy]: Arret d'urgence
    Evitemment_M, //[Mega/Aux -> Teensy]: Un Evitemment peut être a faire |  DATA: Un vecteur (position de l'obstacle)
    North_M, //[Camera -> Teensy]: Il faudra finir au nord
    South_M  //[Camera -> Teensy]: Il faudra finir au sud
};

#endif