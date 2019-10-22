// =============================
// ===       Libraries       ===
// =============================

#include <Arduino.h>
#include <Vector.h>
#include <Math_functions.h>
#include <Ghost.h>
#include <PID.h>
#include <Moteur.h>
#include <Codeuse.h>
#include <Simulator.h>
#include <Sequence.h>
#include <AbsolutelyNotRobot.h>

#define FREQUENCY 100

AbsolutelyNotRobot* bender;
uint16_t currentMillis=0,lastMillis=0;

void setup()
{
  
  Serial.begin(115200);
  delay(2000);
  Serial.println("Bender's booting up");
  bender=new AbsolutelyNotRobot(0,0,0,true);
  Serial.println("Hello, I'm bender");
}

void loop()
{
  currentMillis=millis();
  if ((currentMillis-lastMillis)/1e3 > 1.0/FREQUENCY)
  {
    bender->update(1.0/FREQUENCY);
    bender->debug(true,true,true,true);
    lastMillis=currentMillis;
  }
}