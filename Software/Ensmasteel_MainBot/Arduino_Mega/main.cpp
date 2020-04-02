// =============================
// ===       Libraries       ===
// =============================

#include <Arduino.h>
#include "Communication.h"

#define DELAY 500
uint32_t currentMillis = 0;
uint64_t counter = 0;

void setup()
{
  Serial2.begin(115200);
}

void loop()
{
  if(millis()-currentMillis > DELAY)
  {
    currentMillis = millis();
    counter++;
    Serial2.print("Ping ");
    Serial2.println((int)counter);
  }
}

/*
int incomingByte = 0; // for incoming serial data

Communication communicationTeensy;
Message lastMessage;

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200);
  Serial.println("Begin logging");
  communicationTeensy = Communication(&Serial2);
}

void loop()
{
  communicationTeensy.update();
  if (communicationTeensy.inWaitingRx() > 0)
  {
    Serial.print("New message : ");
    lastMessage = communicationTeensy.peekOldestMessage();
    Serial.print(extractID(lastMessage));
    Serial.print(" ");
    Serial.println(extractInt32(lastMessage));
    communicationTeensy.popOldestMessage();
  }
}
*/