// =============================
// ===       Libraries       ===
// =============================


#include <Arduino.h>
#include "Communication.h"
#include "Actuators.h"
#include "Actuators_Manager.h"

#define DELAY 10
uint32_t lastMillis = 0;

Manager* manager;

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200);
  Serial.println("STARTED");
  manager = new Manager(&Serial2, &Serial);
  lastMillis = millis();
}

void loop()
{
  if (millis() - lastMillis > DELAY)
  {
    lastMillis = millis();

    manager->Update();
  }
}