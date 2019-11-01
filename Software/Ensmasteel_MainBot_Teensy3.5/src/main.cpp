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
#include "AbsolutelyNotRobot.h"
#include "Robot.h"

#define FREQUENCY 100

//Robot *bender;
AbsolutelyNotRobot* bender;
uint32_t currentMillis = 0, lastMillis = 0;
HardwareSerial Serial1(PA10, PA9);

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  Logger::setup(&Serial1, &Serial1, &Serial1, true, true, false);
  delay(2000);
  Logger::infoln("REBOOT%"); //Le caractère % permet de faire sauter le parsing en cours sur la station sol
  Logger::infoln("Bender's booting up");
  //bender = new Robot(0, 0, 0, &Serial1);
  bender=new AbsolutelyNotRobot(0.2,1.2,0,true,&Serial1);
  Logger::infoln("Hello, I'm bender");
}

void loop()
{
  currentMillis = millis();
  if ((currentMillis - lastMillis) / 1e3 > 1.0 / FREQUENCY)
  {
    //bender->Update(1.0 / FREQUENCY);
    bender->update(1.0 / FREQUENCY);
    lastMillis = currentMillis;
  }
}