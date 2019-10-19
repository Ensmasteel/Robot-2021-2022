#include "AbsolutelyNotRobot.h"
#define PIN_CODEUSE_GAUCHE_A 29
#define PIN_CODEUSE_GAUCHE_B 28
#define PIN_CODEUSE_DROITE_A 34
#define PIN_CODEUSE_DROITE_B 35

#define PIN_MOTEUR_GAUCHE_PWR 22
#define PIN_MOTEUR_GAUCHE_SENS 36
#define PIN_MOTEUR_GAUCHE_BRAKE 37

#define PIN_MOTEUR_DROITE_PWR 23
#define PIN_MOTEUR_DROITE_SENS 38
#define PIN_MOTEUR_DROITE_BRAKE 39

#define ELOIGNEMENT_CODEUSES 0.29430415
#define DIAMETRE_ROUE_CODEUSE_DROITE 0.05325315
#define DIAMETRE_ROUE_CODEUSE_GAUCHE 0.053570956
#define TICKS_PER_ROUND 16384

void AbsolutelyNotRobot::update(float dt) {
    #ifndef SIMULATOR
    odometrie.updateCinetique(dt);
    #else
    simu.updateCinetique(dt);
    #endif
    ghost.ActuatePosition(dt);
    target=ghost.Get_Controller_Cinetique();
    asservissement.compute(dt);
    motorLeft.setOrder(outTranslation-outRotation);
    motorRight.setOrder(outTranslation+outRotation);
    sequence.update();
}

void AbsolutelyNotRobot::printCinetique()
{
    cin.print();
}

AbsolutelyNotRobot::AbsolutelyNotRobot(float x, float y, float theta) {
    cin=Cinetique(x,y,theta);
    #ifndef SIMULATOR
    odometrie=Odometrie(TICKS_PER_ROUND,&cin,ELOIGNEMENT_CODEUSES,PIN_CODEUSE_GAUCHE_A,PIN_CODEUSE_GAUCHE_B,DIAMETRE_ROUE_CODEUSE_GAUCHE,PIN_CODEUSE_DROITE_A,PIN_CODEUSE_DROITE_B,DIAMETRE_ROUE_CODEUSE_DROITE);
    #endif
    motorLeft=Motor(PIN_MOTEUR_GAUCHE_PWR,PIN_MOTEUR_GAUCHE_SENS,10);
    pinMode(PIN_MOTEUR_DROITE_BRAKE,OUTPUT); digitalWrite(PIN_MOTEUR_DROITE_BRAKE,LOW); //Adaptation ancien driver
    motorRight=Motor(PIN_MOTEUR_DROITE_PWR,PIN_MOTEUR_DROITE_SENS,10);
    pinMode(PIN_MOTEUR_GAUCHE_BRAKE,OUTPUT); digitalWrite(PIN_MOTEUR_GAUCHE_BRAKE,LOW); //Adaptation ancien driver
    ghost=Ghost(cin);
    asservissement=Asservissement(&outTranslation,&outRotation,&cin,&target,30);
    communication=Communication();
    sequence=Sequence();
    Action::setPointers(&cin,&ghost,&sequence,&communication,&asservissement);
    sequence.add(Goto_Action(10,2,1,0,0.2,fast));
    sequence.add(Spin_Action(5,3.14,fast));
    sequence.add(End_Action());
    sequence.reStart();
    #ifdef SIMULATOR
    simu=Simulator(0.30, 4.0, 1.5, 1.3, &cin, &motorLeft.order, &motorRight.order);
    #endif // DEBUG
}
