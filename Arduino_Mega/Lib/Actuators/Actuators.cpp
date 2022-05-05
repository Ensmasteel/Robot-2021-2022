#include "Actuators.h"

Actuator::Actuator(String name, MessageID messID)
{
    this->messID = messID;
    this->name = name;
    etat = Actuator_State::Attente;
}

Actuator_State Actuator::Update()
{
    return etat;
}

void Actuator::NewOrder(Actuator_Order order)
{
    currentOrder = order;
    etat = Actuator_State::NewMess;
}

/**Pavillon::Pavillon() : Actuator("Pav", MessageID::Pavillon_M)
{
    stepperMotor = new StepperMotorJ(pinStep, pinDir, pinSleep, pinM0, pinM1);
}

void Pavillon::Init(uint8_t pinDir, uint8_t pinStep, uint8_t pinSleep, uint8_t pinM0, uint8_t pinM1)
{
    this->pinDir = pinDir;
    this->pinStep = pinStep;
    this->pinSleep = pinSleep;
    this->pinM0 = pinM0;
    this->pinM1 = pinM1;
    etat = Actuator_State::Attente;
}

Actuator_State Pavillon::Update()
{   
    /*Serial.println("Pavillon");
    Serial.println(int(etat));
    Serial.println(String(currentOrder));
    Serial.print("etat :");
    Serial.println(int(etat));
    switch (etat)
    {
    case Actuator_State::NewMess:
        switch (currentOrder)
        {
        case Actuator_Order::Monter:
            stepperMotor->move(actionStep,500,true,false);
            break;

        case Actuator_Order::Descendre:
            stepperMotor->move(actionStep,500,false,false);
            break;

        default:
            break;
        }
        etat = Actuator_State::MouvFinished;
        break;
    
    case Actuator_State::MouvFinished:
        etat = Actuator_State::Attente;
        break;

    default:
        break;
    }

   return Actuator::Update();
}*/

Tourelle::Tourelle() : Actuator("Tourelle")
{
}

void Tourelle::Init(uint8_t pinDir, uint8_t pinStep, uint8_t pinM0, uint8_t pinM1, MessageID ID)
{
    messID = ID;
    switch (messID)
    {
    case MessageID::BrasD_M:
        name += "D";
        break;

    case MessageID::BrasG_M:
        name += "G";
        break;
    
    default:
        break;
    }
    this->pinDir = pinDir;
    this->pinStep = pinStep;
    this->pinM0 = pinM0;
    this->pinM1 = pinM1;
    etat = Actuator_State::Attente;
}

Actuator_State Tourelle::Update()
{
    switch (etat)
    {
    case Actuator_State::NewMess:
        switch (currentOrder)
        {
        case Actuator_Order::TournerHoraire:
            stepperMotor->move(actionStep,50,true,false);
            break;

        case Actuator_Order::TournerAntiHoraire:
            stepperMotor->move(actionStep,50,false,false);
            break;

        default:
            break;
        }
        etat = Actuator_State::MouvFinished;
        break;

    case Actuator_State::MouvFinished:
        etat = Actuator_State::Attente;
        break;

    default:
        break;
    }
    return Actuator::Update();
}

void Position::Init(int posServo1,int posServo2,int posServo3){
    this->posServo1=posServo1;
    this->posServo=posServo2;
    this->posServo3=posServo3;
}  


Bras::Bras() : Actuator("Bras")
{
}

Actuator_State Bras::Update()
{
    switch (etat)
    {
    case Actuator_State::NewMess:
        switch (currentOrder)
        {
        case Actuator_Order::Sortir:
            servo.write(posSortie);
            break;

        case Actuator_Order::Rentrer:
            servo.write(posRentre);
            break;

        default:
            break;
        }
        etat = Actuator_State::MouvFinished;
        break;
    
    case Actuator_State::MouvFinished:
        etat = Actuator_State::Attente;
        break;
        
    default:
        break;
    }

    return Actuator::Update();
}

void Bras::Init(uint8_t pinServo, MessageID ID, int posRentre, int posSortie)
{
    messID = ID;
    switch (messID)
    {
    case MessageID::TourelleD_M:
        name += "D";
        break;

    case MessageID::TourelleG_M:
        name += "G";
        break;
    
    default:
        break;
    }

    this->pinServo = pinServo; 
    this->posRentre = posRentre;
    this->posSortie = posSortie;

    servo.attach(pinServo);
    servo.write(posRentre);

    etat = Actuator_State::Attente;
}


Pince::Pince() : Actuator("Pince")
{
}

void Pince::Init(uint8_t pinServo, uint8_t pinDir, uint8_t pinStep, uint8_t pinSleep, uint8_t pinM0, uint8_t pinM1, MessageID ID, int ferme, int ouvert, int actionStep)
{
    messID = ID;
    switch (messID)
    {
    case MessageID::PinceArr_M:
        name += "Arr";
        break;

    case MessageID::PinceAvD_M:
        name += "AvD";
        break;

    case MessageID::PinceAvG_M:
        name += "AvG";
        break;
    
    default:
        break;
    }

    this->pinServo = pinServo; 
    this->pinDir = pinDir;
    this->pinStep = pinStep;
    this->pinSleep = pinSleep;
    this->pinM0 = pinM0;
    this->pinM1 = pinM1;
    posFermee = ferme;
    posOuverte = ouvert;
    this-> actionStep = actionStep;

    servo.attach(pinServo);
    servo.write(posFermee);

    stepperMotor = new StepperMotorJ(pinStep, pinDir, pinSleep, pinM0, pinM1);

    etat = Actuator_State::Attente;
}

Actuator_State Pince::Update()
{
    switch (etat)
    {
    case Actuator_State::NewMess:
        switch (currentOrder)
        {
        case Actuator_Order::Monter:
            stepperMotor->move(actionStep,standardDelay,true,true);
            break;

        case Actuator_Order::Descendre:
            stepperMotor->move(actionStep,standardDelay,false,true);
            break;
        
        case Actuator_Order::Ouvrir:
            servo.write(posOuverte);
            break;
        
        case Actuator_Order::Fermer:
            servo.write(posFermee);
            break;

        default:
            break;
        }
        etat = Actuator_State::MouvFinished;
        break;
    
    case Actuator_State::MouvFinished:
        etat = Actuator_State::Attente;
        break;

    default:
        break;
    } 

    return Actuator::Update();
}

Actuator_State PinceAvant::Update()
{
    switch (etat)
    {
    case Actuator_State::NewMess:
        switch (currentOrder)
        {
        
        case Actuator_Order::Stock:
            servo.write(posOuverte);
            stepperMotor->move(miniStep, standardDelay, true, true);//rapide
            servo.write(posFermee);
            delay(200);
            stepperMotor->move(miniStep+actionStep, standardDelay, false, true); //rapide
            stepperMotor->move(miniStep, standardDelay*2, false, true); //lent
            delay(200);
            servo.write(posTresOuverte);
            stepperMotor->move(miniStep+actionStep, standardDelay, true, true);//rapide
            break;

        case Actuator_Order::Destock:
            stepperMotor->move(actionStep, standardDelay, false, true);//rapide
            stepperMotor->move(miniStep, standardDelay*2, false, true);//lent
            delay(200);
            servo.write(posFermee);
            delay(200);
            stepperMotor->move(miniStep*2, standardDelay*3, true, true);//tres lent
            stepperMotor->move(actionStep-miniStep, standardDelay, true, true);//rapide
            servo.write(posTresOuverte);
            break;

        default:
            break;
        }

    default:
        break;
    } 

    return Pince::Update();
}

Actuator_State PinceArriere::Update()
{
    switch (etat)
    {
    case Actuator_State::NewMess:
        switch (currentOrder)
        {
            case Actuator_Order::Stock:
            //depuis la position haute, ouvre les pince puis descend legerment, ferme les pinces et remonte
            servo.write(posOuverte);
            stepperMotor->move(actionStep, standardDelay, false, true);//rapide descente
            servo.write(posFermee);
            stepperMotor->move(actionStep, standardDelay*3, true, true); //tres lente montee
            break;

        case Actuator_Order::Destock:
            stepperMotor->move(actionStep, standardDelay, false, true);//rapide descente
            servo.write(posOuverte);
            stepperMotor->move(actionStep, standardDelay*2, true, true); //lente montee
            break;

        default:
            break;
        }

    default:
        break;
    } 

    return Pince::Update();
}