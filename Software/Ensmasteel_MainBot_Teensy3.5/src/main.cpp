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

#define FREQUENCY 100

AbsolutelyNotRobot* bender;
uint16_t currentMillis=0,lastMillis=0;
HardwareSerial Serial1(PA10, PA9);

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  Logger::setup(&Serial,&Serial,&Serial,true,true,false);
  delay(2000);
  Logger::infoln("Bender's booting up");
  bender=new AbsolutelyNotRobot(0,0,0,true,&Serial1);
  Logger::infoln("Hello, I'm bender");
}

void loop()
{
  currentMillis=millis();
  if ((currentMillis-lastMillis)/1e3 > 1.0/FREQUENCY)
  {
    bender->update(1.0/FREQUENCY);
    lastMillis=currentMillis;
  }
}