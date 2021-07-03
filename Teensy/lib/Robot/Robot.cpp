#include "Robot.h"
#include "Arduino.h"
#include "Actions.h"
#include "Functions.h"
#include "Codeuse.h"

#define PIN_CODEUSE_GAUCHE_A 29
#define PIN_CODEUSE_GAUCHE_B 28
#define PIN_CODEUSE_DROITE_A 34
#define PIN_CODEUSE_DROITE_B 35

#define PIN_MOTEUR_GAUCHE_PWR 24
#define PIN_MOTEUR_GAUCHE_SENS 2
#define PIN_MOTEUR_GAUCHE_BRAKE 37

#define PIN_MOTEUR_DROITE_PWR 25
#define PIN_MOTEUR_DROITE_SENS 3
#define PIN_MOTEUR_DROITE_BRAKE 39

#define PIN_INTERRUPTEUR_ARR_DROITE 30
#define PIN_INTERRUPTEUR_ARR_GAUCHE 33

#define ELOIGNEMENT_CODEUSES 0.29430415
#define DIAMETRE_ROUE_CODEUSE_DROITE 0.05325315
#define DIAMETRE_ROUE_CODEUSE_GAUCHE 0.053570956
#define TICKS_PER_ROUND 16384

#define SKIP_TELEMETRY_LONG 10000
#define SKIP_TELEMETRY_FAST 400000

Robot::Robot(float xIni, float yIni, float thetaIni, Stream *commPort, Stream *actuPort)
{
    MoveProfiles::setup();
    cinetiqueCurrent = Cinetique(xIni, yIni, thetaIni);
    odometrie = Odometrie(TICKS_PER_ROUND, &cinetiqueCurrent, ELOIGNEMENT_CODEUSES, PIN_CODEUSE_GAUCHE_A, PIN_CODEUSE_GAUCHE_B, DIAMETRE_ROUE_CODEUSE_GAUCHE, PIN_CODEUSE_DROITE_A, PIN_CODEUSE_DROITE_B, DIAMETRE_ROUE_CODEUSE_DROITE, PIN_INTERRUPTEUR_ARR_DROITE, PIN_INTERRUPTEUR_ARR_GAUCHE);

    motorLeft = Motor(PIN_MOTEUR_GAUCHE_PWR, PIN_MOTEUR_GAUCHE_SENS, 10);
    pinMode(PIN_MOTEUR_DROITE_BRAKE, OUTPUT);
    digitalWrite(PIN_MOTEUR_DROITE_BRAKE, LOW); //Adaptation ancien driver
    motorRight = Motor(PIN_MOTEUR_DROITE_PWR, PIN_MOTEUR_DROITE_SENS, 10);
    pinMode(PIN_MOTEUR_GAUCHE_BRAKE, OUTPUT);
    digitalWrite(PIN_MOTEUR_GAUCHE_BRAKE, LOW); //Adaptation ancien driver

    ghost = Ghost(cinetiqueCurrent);
    controller = Asservissement(&translationOrderPID, &rotationOrderPID, &cinetiqueCurrent, &cinetiqueNext, filterFrequency);
    communication = Communication(commPort);
    commActionneurs = Communication(actuPort);

    sequences = new Sequence*[__NBSEQUENCES__]; //On définit un array de la bonne taille
    Action::setPointer(this);
    for (int i=0;i<__NBSEQUENCES__;i++)
        sequences[i] = new Sequence(i);

    //ATTENTION, LES ACTIONS DOIVENT ETRE DEFINIE EN TANT QUE ROBOT BLEU !
    // Might be define in main.cpp->setup

    TargetVector base = TargetVector(0.22,1.20,false);
    TargetVector northBase = TargetVector(0.22,1.65,false);
    TargetVector southBase = TargetVector(0.22,0.70,false);

    TargetVector gobeletR1 = TargetVector(0.300,1.600,false);
    TargetVector gobeletV1 = TargetVector(0.445,1.485,false);
    TargetVector gobeletR2 = TargetVector(0.445,0.915,false);
    TargetVector gobeletV2 = TargetVector(0.300,0.800,false);

    TargetVector gobeletR3 = TargetVector(0.670,1.900,false);
    TargetVector gobeletV3 = TargetVector(0.965,1.600,false);
    TargetVector gobeletR4 = TargetVector(1.100,1.200,false);
    TargetVector gobeletV4 = TargetVector(1.270,0.800,false);
    
    TargetVector gobeletR5 = TargetVector(1.605,0.045,false);
    TargetVector gobeletV5 = TargetVector(1.665,0.345,false);
    TargetVector gobeletR6 = TargetVector(1.935,0.345,false);
    TargetVector gobeletV6 = TargetVector(1.995,0.045,false);
    
    TargetVector rack1 = TargetVector(0.000,1.600,false);
    TargetVector rack2 = TargetVector(0.850,2.000,false);
    
    TargetVector manche1 = TargetVector(0.230,0.000,false);
    TargetVector manche2 = TargetVector(0.635,0.000,false);

    Sequence* mainSequence = getSequenceByName(mainSequenceName);
        Serial.println("entree dans main");
        //TODO config recalage etc

        //Attend le message Tirette
        //mainSequence->add(new Wait_Tirette_Action(30));
        //mainSequence->add(new Do_Action(startTimeSeq));
        //mainSequence->add(new Wait_Message_Action(Tirette_M,-1,&communication));
        //mainSequence->add(new Spin_Action(10,TargetVectorE(PI/4,false),standard));
        mainSequence->add(new Sleep_Action(3));
        /*mainSequence->add(new Send_Action(newMessage(Pavillon_M, Actuator_Order::Monter, 0, 0, 0),&commActionneurs));
        
        mainSequence->add(new Wait_Message_Action(Pavillon_M, 5, &commActionneurs));
        mainSequence->add(new Send_Action(newMessage(Pavillon_M, Actuator_Order::Descendre, 0, 0, 0),&commActionneurs));
        mainSequence->add(new Wait_Message_Action(Pavillon_M, 5, &commActionneurs));
        mainSequence->add(new Send_Action(newMessage(PinceAvD_M, Actuator_Order::Monter, 0, 0, 0),&commActionneurs));
        mainSequence->add(new Send_Action(newMessage(PinceAvG_M, Actuator_Order::Monter, 0, 0, 0),&commActionneurs));
        mainSequence->add(new Send_Action(newMessage(PinceAvD_M, Actuator_Order::Ouvrir, 0, 0, 0),&commActionneurs));
        mainSequence->add(new Send_Action(newMessage(PinceAvG_M, Actuator_Order::Ouvrir, 0, 0, 0),&commActionneurs));
        mainSequence->add(new Send_Action(newMessage(PinceAvD_M, Actuator_Order::Fermer, 0, 0, 0),&commActionneurs));
        mainSequence->add(new Send_Action(newMessage(PinceAvG_M, Actuator_Order::Fermer, 0, 0, 0),&commActionneurs));

        mainSequence->add(new Send_Action(newMessage(PinceArr_M, Actuator_Order::Monter, 0, 0, 0),&commActionneurs));
        mainSequence->add(new Send_Action(newMessage(PinceArr_M, Actuator_Order::Ouvrir, 0, 0, 0),&commActionneurs));
        mainSequence->add(new Send_Action(newMessage(PinceArr_M, Actuator_Order::Descendre, 0, 0, 0),&commActionneurs));*/
        
        mainSequence->add(new Send_Action(newMessage(BrasD_M, Actuator_Order::Sortir, 0, 0, 0),&commActionneurs));
        mainSequence->add(new Send_Action(newMessage(BrasD_M, Actuator_Order::Rentrer, 0, 0, 0),&commActionneurs));
        mainSequence->add(new Send_Action(newMessage(BrasG_M, Actuator_Order::Sortir, 0, 0, 0),&commActionneurs));
        mainSequence->add(new Send_Action(newMessage(BrasG_M, Actuator_Order::Rentrer, 0, 0, 0),&commActionneurs));

        /*mainSequence->add(new Forward_Action(5,0.2,standard));
        //mainSequence->add(new Send_Action(newMessage(BrasG_M, Actuator_Order::Sortir, 0, 0, 0), &commActionneurs));
        //mainSequence->add(new Spin_Action(10,TargetVectorE(PI/4,false),standard));
        mainSequence->add(new Backward_Action(5,0.2,standard));
        mainSequence->add(new Spin_Action(10,TargetVectorE(PI/2,false),standard));
        mainSequence->add(new Goto_Action(5,TargetVectorE(1.2,1.7,0,false),0.5,standard));
        mainSequence->add(new Spin_Action(10,TargetVectorE(PI,false),standard));
        mainSequence->add(new Goto_Action(5,TargetVectorE(2.5,0.3,PI,false),0.5,standard,true));*/

        /*
        * Lors des "5.0" prochaines secondes, si une erreur PID est levée
        *       le robot et le ghost sont recallés contre une bordure (et le ghost reste statique ensuite)
        *       cette action saute (forceMainSeqNext est appelé)
        * sinon
        *       le timeout est appelé et on passe a l'action suivante (au prochain move start, le ghost sera recallé sur le robot)
        */
        //mainSequence->add(new Recallage_Action(true,1.0,5.0));  
        //mainSequence->add(new Brake_Action(-1));

        // mainSequence->add(new Forward_Action(5,0.1,standard));
        // mainSequence->add(new Rotate_Action(5,PI/2,standard));
        // mainSequence->add(new Forward_Action(5,0.2,standard));

        // mainSequence->add(new Goto_Action(5,TargetVectorE(0.2,1.2,0,false),0.5,standard));
        // mainSequence->add(new Spin_Action(10,TargetVectorE(PI,false),standard));


        //ActionFinale
        //mainSequence->add(new End_Action(false,true,true));
/*        mainSequence->add(new Send_Order_Action(PinceAvD_M, Actuator_Order::Stock, (float)5.0, &commActionneurs, true));
        mainSequence->add(new Sleep_Action(1));
        mainSequence->add(new Send_Order_Action(PinceAvD_M, Actuator_Order::Destock, (float)5.0, &commActionneurs, true));
        mainSequence->add(new Sleep_Action(1));
        mainSequence->add(new Send_Order_Action(PinceArr_M, Actuator_Order::Stock, (float)5.0, &commActionneurs, true));
        mainSequence->add(new Sleep_Action(1));
        mainSequence->add(new Send_Order_Action(PinceArr_M, Actuator_Order::Destock, (float)5.0, &commActionneurs, true));
        mainSequence->add(new Sleep_Action(1000));*/
        mainSequence->add(new End_Action(true,false));
        Serial.println("avant StartSequence");
        mainSequence->startSelected();
    
    Serial.println("mainpass");
        //déclenchée par timeSequence
    Sequence* goNorth = getSequenceByName(goNorthName);
        goNorth->add(new StraightTo_Action(-1,northBase,standard));
        goNorth->add(new End_Action());
        goNorth->pause(false);//Cette action ne doit pas se lancer dès le début

        //déclenchée par timeSequence
    Sequence* goSouth = getSequenceByName(goSouthName);
        goSouth->add(new StraightTo_Action(-1,southBase,standard));
        goSouth->add(new End_Action());
        goSouth->pause(false); //Cette action ne doit pas se lancer dès le début

    Sequence* communicationSequence = getSequenceByName(communicationSequenceName);

        Switch_Message_Action* messageSwitch = new Switch_Message_Action(-1,&communication,NO_REQUIREMENT);
        messageSwitch->addPair(MessageID::Empty_M , ping);
        messageSwitch->addPair(MessageID::Em_Stop_M,shutdown);
        messageSwitch->addPair(MessageID::PID_tweak_M,PID_tweak);
        messageSwitch->addPair(MessageID::North_M,setNorth);
        messageSwitch->addPair(MessageID::South_M,setSouth);

        communicationSequence->add(messageSwitch);
        communicationSequence->add(new End_Action(true,false));
        communicationSequence->startSelected();

        //déclenchée par mainSequence juste après la tirette
    Sequence* timeSequence = getSequenceByName(timeSequenceName);
        timeSequence->add(new Do_Action(setTimeStart));
        timeSequence->add(new Sleep_Action(300));
        timeSequence->add(new Do_Action(startBackHomeSeq));
        timeSequence->add(new Sleep_Action(10));
        timeSequence->add(new Do_Action(shutdown));
        timeSequence->add(new End_Action());
        timeSequence->pause(false); //La time sequence ne doit s'écouler qu'a partir du tiré de la tirette !!

    Sequence* recallageListener = getSequenceByName(recallageListerName);
        recallageListener->add(new Wait_Error_Action(PID_FAIL_ERROR,14.0));
        recallageListener->add(new Do_Action(recallageBordure,-1));

        //Si cette action s'active, elle s'active au plus tard 14s après le démarrage du backward.
        //Sachant qu'on a un timeout de 15s sur le backward, on est sur que la seule action qu'on peut forcer c'est le backward
        recallageListener->add(new Do_Action(forceMainSeqNext,-1)); 
        recallageListener->add(new End_Action(true,true,false));
        recallageListener->pause(false);

    ghost.Lock(false);
}

void Robot::Update_Cinetique(float dt)
{
    odometrie.updateCinetique(dt);
    Serial.println("odometrie : ");
    Serial.println(odometrie.codeuseDroite.ticks);
    Serial.println(odometrie.codeuseGauche.ticks);
}

void Robot::Update(float dt)
{   
    communication.update();
    commActionneurs.update();

    Update_Cinetique(dt);
    
    ghost.ActuatePosition(dt);
    
    cinetiqueNext = ghost.Get_Controller_Cinetique();

    controller.compute(dt);

    if (odometrie.getInterGaucheContact()) {
        motorLeft.setOrder(translationOrderPID - rotationOrderPID);
        motorLeft.actuate();
    }
    if (odometrie.getInterDroiteContact()) {
        motorRight.setOrder(translationOrderPID + rotationOrderPID);
        motorRight.actuate();
    }

    for (int i=0;i<__NBSEQUENCES__;i++)
        sequences[i]->update();

    
    if (compteur==SKIP_TELEMETRY_LONG){
        telemetry(false,true);
        compteur=0;
    } else if (compteur%SKIP_TELEMETRY_FAST==0){
        telemetry(true,false);
    }

    if (communication.inWaitingRx() > 0)
        communication.popOldestMessage(); //Tout le monde a eu l'occasion de le peek, on le vire.
    if (commActionneurs.inWaitingRx() > 0)
        commActionneurs.popOldestMessage(); //Tout le monde a eu l'occasion de le peek, on le vire.
    if (ErrorManager::inWaiting() > 0)
        ErrorManager::popOldestError(); //Les séquences ont eu l'occasion de le lire, on le vire
    compteur++;
}

void Robot::telemetry(bool odometrie, bool other)
{
    if (odometrie)
    {
        cinetiqueCurrent.toTelemetry("R");
        ghost.Get_Controller_Cinetique().toTelemetry("G");
    }
    if (other)
    {
        Logger::toTelemetry("pid", String(controller.close));
        Logger::toTelemetry("ghost", String(ghost.trajectoryIsFinished()));
        getSequenceByName(mainSequenceName)->toTelemetry();
        communication.toTelemetry();
    }
}

Sequence* Robot::getSequenceByName(SequenceName name){
    return sequences[(int)name];
}

float Robot::getTime(){
    return millis()/1e3 - timeStarted;
}

void Robot::setTeamColor(TeamColor teamColor){
    this->teamColor=teamColor;
    Target::setTeamColor(teamColor);
}

TeamColor Robot::getTeamColor(){
    return teamColor;
}

void Robot::move(VectorE where)
{
    cinetiqueCurrent = Cinetique(where._x,where._y,where._theta,0,0);
    cinetiqueNext = cinetiqueCurrent;
    ghost.moveGhost(where);
}

void Robot::move(TargetVectorE whereTarget){
    move(whereTarget.getVectorE());
}

void Robot::recalibrateGhost()
{
    ghost.moveGhost(cinetiqueCurrent);
}