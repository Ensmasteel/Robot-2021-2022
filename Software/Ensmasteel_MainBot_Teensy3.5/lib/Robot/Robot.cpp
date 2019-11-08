#include "Robot.h"
#include "Arduino.h"
#include "Actions.h"
#include "Functions.h"

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

#define SKIP_TELEMETRY 5

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
    Action::setPointer(this);

    //ATTENTION, LES ACTIONS DOIVENT ETRE DEFINIE EN TANT QUE ROBOT BLEU !
    // Might be define in main.cpp->setup
    //Goto (timeout = 25s, x=2m, y=20cm, thetaFinal = -PI/2, courbure = 20%, allure = standard)
    mainSequence.add(new Goto_Action(25, TargetVectorE(2.0, 0.2, -1.57,false), 0.2, standard));
    //Spin (timeout = 20s, thetaFinal = 2*PI/3, allure = standard)
    mainSequence.add(new Spin_Action(20, TargetVectorE(2*PI/3.0,false),standard));
    //AvancerToutDroit (timeout = 20s, avancerDe = 1.0m, allure = standard)
    mainSequence.add(new Forward_Action(20,1,standard));
    //AllerDirectementVers (timeout = 30s, x=20cm, y=1m20, allure = standard)
    mainSequence.add(new StraightTo_Action(30,TargetVector(0.2,1.2,false),standard));
    //ActionFinale
    mainSequence.add(new End_Action());
    mainSequence.startSelected();

    communicationSequence = Sequence();

    Switch_Message_Action* messageSwitch = new Switch_Message_Action(-1,NO_REQUIREMENT);
    messageSwitch->addPair(MessageID::Empty_M , ping);
    messageSwitch->addPair(MessageID::Em_Stop_M,shutdown);
    messageSwitch->addPair(MessageID::PID_tweak_M,PID_tweak);

    communicationSequence.add(messageSwitch);
    communicationSequence.add(new End_Action(true));
    communicationSequence.startSelected();

    stopSequence = Sequence();
    stopSequence.add(new Sleep_Action(100));
    stopSequence.add(new Do_Action(pauseNlockMainSequence));
    stopSequence.add(new End_Action());
    stopSequence.startSelected();

    ghost.Lock(false);
}

void Robot::Update_Cinetique(float dt)
{
    odometrie.updateCinetique(dt);
}

void Robot::Update(float dt)
{
    communication.update();
    Update_Cinetique(dt);
    ghost.ActuatePosition(dt);
    cinetiqueNext = ghost.Get_Controller_Cinetique();
    controller.compute(dt);
    motorLeft.setOrder(translationOrderPID - rotationOrderPID);
    motorRight.setOrder(translationOrderPID + rotationOrderPID);
    motorLeft.actuate();
    motorRight.actuate();
    mainSequence.update();
    stopSequence.update();
    communicationSequence.update();
    if (compteur==SKIP_TELEMETRY)
    {
        telemetry();
        compteur=0;
    }
    if (communication.inWaitingRx() > 0)
        communication.popOldestMessage(); //Tout le monde a eu l'occasion de le peek, on le vire.
    compteur++;
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