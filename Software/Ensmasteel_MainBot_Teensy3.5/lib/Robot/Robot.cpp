#include "Robot.h"
#include "Arduino.h"

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

Robot::Robot(float xIni, float yIni, float thetaIni, Stream *commPort)
{
    cinetiqueCurrent = Cinetique(xIni, yIni, thetaIni);
    odometrie = Odometrie(TICKS_PER_ROUND, &cinetiqueCurrent, ELOIGNEMENT_CODEUSES, PIN_CODEUSE_GAUCHE_A, PIN_CODEUSE_GAUCHE_B, DIAMETRE_ROUE_CODEUSE_GAUCHE, PIN_CODEUSE_DROITE_A, PIN_CODEUSE_DROITE_B, DIAMETRE_ROUE_CODEUSE_DROITE);

    motorLeft = Motor(PIN_MOTEUR_GAUCHE_PWR, PIN_MOTEUR_GAUCHE_SENS, 10);
    pinMode(PIN_MOTEUR_DROITE_BRAKE, OUTPUT);
    digitalWrite(PIN_MOTEUR_DROITE_BRAKE, LOW); //Adaptation ancien driver
    motorRight = Motor(PIN_MOTEUR_DROITE_PWR, PIN_MOTEUR_DROITE_SENS, 10);
    pinMode(PIN_MOTEUR_GAUCHE_BRAKE, OUTPUT);
    digitalWrite(PIN_MOTEUR_GAUCHE_BRAKE, LOW); //Adaptation ancien driver

    ghost = Ghost(cinetiqueCurrent);
    controller = Asservissement(&translationOrderPID, &rotationOrderPID, &cinetiqueCurrent, &cinetiqueNext, controllerFrequency);
    communication = Communication(commPort);

    mainSequence = Sequence();
    Action::setPointers(&cinetiqueCurrent, &ghost, &mainSequence, &communication, &controller);

    // Might be define in main.cpp->setup
    mainSequence.add(new StraightTo_Action(20, TargetVector(1, 1,false), standard));
    mainSequence.add(new Goto_Action(20, TargetVectorE(2.0, 0.0, 0, false), 0.2, standard));
    mainSequence.add(new Goto_Action(20, TargetVectorE(0.0, 0.0, -1.57, false), 0.2, standard));
    mainSequence.add(new End_Action());
    mainSequence.startSelected();

    communicationSequence = Sequence();
    communicationSequence.add(new Wait_Message_Action(PID_tweak_M, -1, NO_REQUIREMENT));
    communicationSequence.add(new Do_Action(PID_tweak, -1));
    communicationSequence.add(new End_Action(true));
    communicationSequence.startSelected();

    ghost.Lock(false);
}

void Robot::Update(float dt)
{
    communication.update();
    odometrie.updateCinetique(dt);
    ghost.ActuatePosition(dt);
    cinetiqueNext = ghost.Get_Controller_Cinetique();
    controller.compute(dt);
    motorLeft.setOrder(translationOrderPID - rotationOrderPID);
    motorRight.setOrder(translationOrderPID + rotationOrderPID);
    motorLeft.actuate();
    motorRight.actuate();
    mainSequence.update();
    communicationSequence.update();
    telemetry();
    if (communication.inWaiting() > 0)
        communication.pullOldestMessage(); //Tout le monde a eu l'occasion de le peek, on le vire.
}

void Robot::telemetry()
{
    cinetiqueCurrent.toTelemetry("R");
    ghost.Get_Controller_Cinetique().toTelemetry("G");
    Logger::toTelemetry("pid", String(controller.close));
    Logger::toTelemetry("ghost", String(ghost.trajectoryIsFinished()));
    mainSequence.toTelemetry();
    communication.toTelemetry();
}