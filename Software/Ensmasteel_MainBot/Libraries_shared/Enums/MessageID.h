#ifndef MESSAGEID_H_
#define MESSAGEID_H_
#include <stdint.h>

#define DROIT_byteM 0
#define GAUCHE_byteM 1

// COMPLETER LES ENUMERATIONS PAR LA FIN //


// En-tete de message //
///////////////////////

enum MessageID : uint16_t
{
    Empty_M,        //Message vide
    Stop_M,         //[Teensy -> Aux]:      Arret des actionneurs
    Tirette_M,      //[Mega -> Teensy]:     Tirette actionnée
    PID_tweak_M,    //[Aux -> Teensy]:      Changer PID | DATA: 4 bytes [bool incr, bool translation, wichOne (P=0,I=1,D=2)]
    add_forward_M,  //[Aux -> Teensy]:      Rajoute un forward a la fin de la file principale
    add_backward_M, //[Aux -> Teensy]:      Rajoute un backward a la fin de la file principale
    add_spin_M,     //[Aux -> Teensy]:      Rajoute un spin a la fin de la file principale
    Em_Stop_M,      //[Aux -> Teensy]:      Arret d'urgence
    Evitemment_M,   //[Mega/Aux -> Teensy]: Un Evitemment peut être a faire |  DATA: Un vecteur (position de l'obstacle)
    North_M,        //[Camera -> Teensy]:   Il faudra finir au nord
    South_M,        //[Camera -> Teensy]:   Il faudra finir au sud
    Pavillon_M,     //[Teensy -> Mega]:     Il faut lever le pavillon | byte0 = 0 | byte1 = Actuator_Order |
    Bras_M          //[Teensy -> Mega]:     Rentrer ou sortir bras    | byte0 = Actuator_Position | byte1 = Actuator_Order |
};

// Complements d'ordres //
/////////////////////////

// Byte 0
enum Actuator_Position : uint8_t
{
    Gauche,
    Droit,
    Arriere,
    Null
};

// Byte 1
enum Actuator_Order : uint8_t
{
    Sortir,     // Bras
    Rentrer,    // Bras
    Monter,     // Pavillon, Railles
    Descendre,  // Pavillon, Railles
    Fermer,     // Pinces
    Ouvrir      // Pinces
};

#endif