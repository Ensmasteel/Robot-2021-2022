// =============================
// ===       Libraries       ===
// =============================
#include "Logger.h"
#include "Arduino.h"
#include "Vector.h"
#include "Math_functions.h"
#include "Ghost.h"
#include "PID.h"
#include "Moteur.h"
#include "Codeuse.h"
#include "Simulator.h"
#include "Sequence.h"
#include "RobotSimu.h"
#include "ErrorManager.h"
#include "Lidar2022.h"

#define FREQUENCY 100

Robot *bender;
Lidar2022 *lidar;
uint32_t currentMillis = 0, lastMillis = 0;
#ifdef STM32BOTH
HardwareSerial Serial1(PA10, PA9);
#endif
uint32_t topWarn;
uint32_t moy;
uint32_t compteur=0;

void setup()
{
  Serial.begin(115200);   // USB
  Serial2.begin(115200);  // Arduino Mega
  Serial3.begin(115200);  // HC05
  Serial4.begin(115200);  // LIDAR
  
  delay(500);
  Logger::setup(&Serial, &Serial, &Serial, true, true, true  );
  ErrorManager::setup();
  delay(5000);
  Logger::infoln("REBOOT%"); //Le caractère % permet de faire sauter le parsing en cours sur la station sol
  Logger::infoln("Bender's booting up");
  bender = new Robot(0.110,1.370,0,BLEU,&Serial,&Serial2,&Serial4);
  //bender = new Robot(2.880,1.370,PI,JAUNE,&Serial,&Serial2,&Serial4);
  lidar = new Lidar2022(bender);
  lidar->Begin(Serial4);
  
  //bender=new RobotSimu(0.22,1.20,0,&Serial,&Serial2);
  //bender->setTeamColor(TeamColor::JAUNE); 
  Logger::infoln("Hello, I'm bender");
  topWarn=millis();
}

void loop()
{
  currentMillis = millis();

  if (currentMillis-topWarn>25000) //Affichage toutes les 25 secondes de la freq moyenne
  {
    Logger::infoln("Actual Frequency: "+String(1000.0/moy*compteur)+" Hz");
    topWarn=millis();
  }

  if ((currentMillis - lastMillis) / 1e3 >= 1.0 / FREQUENCY)
  {
    moy+=(currentMillis - lastMillis);
    compteur++;
    bender->Update(1.0 / FREQUENCY);
    lidar->detect();
    //bender->dem=false;
    if (lidar->getDetectClose()){
      bender->stopped=true;
    }
    if (bender->stopped==true && lidar->getRedemarrage()){
      Logger::debugln("ET MAINTENANT LAAAAAAA");
      bender->stopped=false;
      bender->dem=true;
    }
    lastMillis = currentMillis;
  }
}