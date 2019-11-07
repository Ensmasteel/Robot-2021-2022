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

#define FREQUENCY 100.0

Robot *bender;
uint32_t currentMillis = 0, lastMillis = 0;
HardwareSerial Serial1(PA10, PA9);
uint32_t topWarn;
uint32_t moy;
uint32_t compteur=0;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  Logger::setup(&Serial1, &Serial1, &Serial, true, true, true);
  Target::setTeamColor(BLEU);
  delay(2000);
  Logger::infoln("REBOOT%"); //Le caractère % permet de faire sauter le parsing en cours sur la station sol
  Logger::infoln("Bender's booting up");
  //bender = new Robot(0, 0, 0, &Serial1);
  bender=new RobotSimu(0.2,1.2,0,&Serial1);
  Logger::infoln("Hello, I'm bender");
  topWarn=millis();
}

void loop()
{
  currentMillis = millis();

  if (currentMillis-topWarn>5000) //Affichage toute les 5 seconde de la freq moyenne
  {
    Logger::infoln("Actual Frequency: "+String(1000.0/moy*compteur)+" Hz");
    topWarn=millis();
  }

  if ((currentMillis - lastMillis) / 1e3 >= 1.0 / FREQUENCY)
  {
    moy+=(currentMillis - lastMillis);
    compteur++;
    bender->Update(1.0 / FREQUENCY);
    lastMillis = currentMillis;
  }
}