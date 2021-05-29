#include <Arduino.h>
#include <Servo.h>
#include <DRV8834.h>

Servo brasG;
Servo brasD;
Servo pinceD;
Servo pinceG;
Servo pinceArr;


int pos = 90;


const int dirPinD = 26;
const int stepPinD = 25;
const int dirPinG = 24;
const int stepPinG = 23;
const int dirPinArr = 31;
const int stepPinArr = 30;

const int sleepPinces = 27;
const int pinArrM0 = 33;
const int pinArrM1 = 32;
const int pinAvM0 = 29;
const int pinAvM1 = 28;

const int dirPav = 35;
const int stepPav = 34;
const int sleepPav = 22;
const int pavM0 = 37;
const int pavM1 = 36;

const float nb_toursRailStock = 2.1f;
const float nb_toursRailDestock = 2.02f;
const float nb_toursPav = 10.0f;
const int stepPerRevolution = 200;
int stepDelayRail = 2000;
int stepDelayRailArr = 4000;
int stepDelayPav = 500;

DRV8834* stepperMotor;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(dirPinD,OUTPUT);
  pinMode(stepPinD,OUTPUT);
  pinMode(dirPinG,OUTPUT);
  pinMode(stepPinG,OUTPUT);
  pinMode(dirPinArr,OUTPUT);
  pinMode(stepPinArr,OUTPUT);

  pinMode(sleepPinces,OUTPUT);
  pinMode(pinArrM0,OUTPUT);
  pinMode(pinArrM1,OUTPUT);
  pinMode(pinAvM0,OUTPUT);
  pinMode(pinAvM1,OUTPUT);

  pinMode(dirPav,OUTPUT);
  pinMode(stepPav,OUTPUT);
  pinMode(sleepPav,OUTPUT);
  pinMode(pavM0,OUTPUT);
  pinMode(pavM1,OUTPUT);

  brasD.attach(4);
  brasG.attach(3);
  pinceD.attach(7);
  pinceG.attach(6);
  pinceArr.attach(5);

  pinceArr.write(180);

}

void loop() {

  pinceD.write(60);
  delay(3000);
  pinceD.write(80);
  delay(1000);
  pinceD.write(110);
  delay(1000);
 /*
  delay(5000);
  
  //////////Systeme arriere//////////

  digitalWrite(sleepPinces,HIGH);
  digitalWrite(dirPinArr,HIGH);

  //montée rail arriere
  for(int x = 0; x < stepPerRevolution * nb_toursRailDestock; x++) {
    digitalWrite(stepPinArr,HIGH);
    delayMicroseconds(stepDelayRailArr);
    digitalWrite(stepPinArr,LOW);
    delayMicroseconds(stepDelayRailArr);
  }

  //ouverture puis fermeture pince arriere
  delay(1000);
  pinceArr.write(120);
  delay(1000);
  pinceArr.write(175);
  delay(1000);

  //descente rail arrière
  digitalWrite(dirPinArr,LOW);
  
  for(int x = 0; x < stepPerRevolution * nb_toursRailDestock; x++) {
    digitalWrite(stepPinArr,HIGH);
    delayMicroseconds(stepDelayRailArr);
    digitalWrite(stepPinArr,LOW);
    delayMicroseconds(stepDelayRailArr);
  }
 */
  Serial.println("avant delay");
  delay(2000);
  //////////Pavillon//////////

  /*stepperMotor = new DRV8834(stepPerRevolution, dirPav, stepPav, sleepPav, pavM0, pavM1);
  stepperMotor->begin(300,(short)1);
  stepperMotor->disable();
  stepperMotor->enable();
  stepperMotor->move(2000);
  stepperMotor->disable();
  Serial.println("Apres delay");
  /*digitalWrite(sleepPav,HIGH);
  digitalWrite(dirPav,HIGH);

  //montée pavillon
  for(int x = 0; x < stepPerRevolution * nb_toursPav; x++) {
    digitalWrite(stepPav,HIGH);
    delayMicroseconds(stepDelayPav);
    digitalWrite(stepPav,LOW);
    delayMicroseconds(stepDelayPav);
  }
  
  delay(1000);
  //descente pavillon
  digitalWrite(dirPav,LOW);
  
  for(int x = 0; x < stepPerRevolution * nb_toursPav; x++) {
    digitalWrite(stepPav,HIGH);
    delayMicroseconds(stepDelayPav);
    digitalWrite(stepPav,LOW);
    delayMicroseconds(stepDelayPav);
  }*/

  //digitalWrite(sleepPav,LOW);

  //delay(2000);
  //////////Pince avant gauche//////////

  /*digitalWrite(sleepPinces,HIGH);
  digitalWrite(dirPinG,HIGH);
  pinceG.write(100);

  //montée rail
  for(int x = 0; x < stepPerRevolution * (nb_toursRail); x++) {
    digitalWrite(stepPinG,HIGH);
    delayMicroseconds(stepDelayRail);
    digitalWrite(stepPinG,LOW);
    delayMicroseconds(stepDelayRail);
  }
  /*pinceG.write(110);
  for(int x = 0; x < stepPerRevolution * (2); x++) {
    digitalWrite(stepPinG,HIGH);
    delayMicroseconds(stepDelayRail);
    digitalWrite(stepPinG,LOW);
    delayMicroseconds(stepDelayRail);
  }

  //fermeture pince
  delay(1000);
  pinceG.write(100);
  delay(1000);
  pinceG.write(70);
  delay(1000);

  //descente rail
  digitalWrite(dirPinG,LOW);
  
  for(int x = 0; x < stepPerRevolution * nb_toursRail; x++) {
    digitalWrite(stepPinG,HIGH);
    delayMicroseconds(stepDelayRail);
    digitalWrite(stepPinG,LOW);
    delayMicroseconds(stepDelayRail);
  }

  //ouverture pince
  delay(1000);
  pinceG.write(100);


  delay(2000);*/
  //////////Pince avant droite//////////

 /* digitalWrite(sleepPinces,HIGH);

  for(int j = 0; j < 5; j++) {

  pinceD.write(80);
  digitalWrite(dirPinD,HIGH);
  for(int x = 0; x < stepPerRevolution * (0.2); x++) {
    digitalWrite(stepPinD,HIGH);
    delayMicroseconds(stepDelayRail);
    digitalWrite(stepPinD,LOW);
    delayMicroseconds(stepDelayRail);
  }

  digitalWrite(dirPinD,LOW);
  delay(200);
  pinceD.write(70);

  //montée rail
  for(int x = 0; x < stepPerRevolution * (nb_toursRailStock+0.2); x++) {
    digitalWrite(stepPinD,HIGH);
    delayMicroseconds(stepDelayRail);
    digitalWrite(stepPinD,LOW);
    delayMicroseconds(stepDelayRail);
  }

  /*for(int x = 0; x < stepPerRevolution * (0.2); x++) {
    digitalWrite(stepPinD,HIGH);
    delayMicroseconds(stepDelayRail*2);
    digitalWrite(stepPinD,LOW);
    delayMicroseconds(stepDelayRail*2);
  }*/

  //fermeture pince
 /* delay(200);
  pinceD.write(100);
  
  //descente rail
  digitalWrite(dirPinD,HIGH);
  
  for(int x = 0; x < stepPerRevolution * (nb_toursRailStock); x++) {
    digitalWrite(stepPinD,HIGH);
    delayMicroseconds(stepDelayRail);
    digitalWrite(stepPinD,LOW);
    delayMicroseconds(stepDelayRail);
  }

  }



  for(int j = 0; j < 5; j++) {

  digitalWrite(dirPinD,LOW);

  //montée rail
  for(int x = 0; x < stepPerRevolution * (nb_toursRailDestock); x++) {
    digitalWrite(stepPinD,HIGH);
    delayMicroseconds(stepDelayRail);
    digitalWrite(stepPinD,LOW);
    delayMicroseconds(stepDelayRail);
  }

  //fermeture pince
  delay(200);
  pinceD.write(70);
  delay(200);
  
  //descente rail
  digitalWrite(dirPinD,HIGH);

  for(int x = 0; x < stepPerRevolution * (0.5); x++) {
    digitalWrite(stepPinD,HIGH);
    delayMicroseconds(stepDelayRail*3);
    digitalWrite(stepPinD,LOW);
    delayMicroseconds(stepDelayRail*3);
  }
  
  for(int x = 0; x < stepPerRevolution * (nb_toursRailDestock-0.5); x++) {
    digitalWrite(stepPinD,HIGH);
    delayMicroseconds(stepDelayRail);
    digitalWrite(stepPinD,LOW);
    delayMicroseconds(stepDelayRail);
  }
  pinceD.write(100);
  }*/

  /*//ouverture pince
  delay(1000);
  pinceD.write(100);

  delay(2000);*/
  //////////Bras gauche//////////

  //deployement puis retour bras
  delay(1000);
  brasG.write(0);
  delay(1000);
  brasG.write(90);
  delay(1000);


  delay(2000);
  //////////Bras droit//////////

  //deployement puis retour bras
  delay(1000);
  brasD.write(0);
  delay(1000);
  brasD.write(90);

  delay(100000);
}