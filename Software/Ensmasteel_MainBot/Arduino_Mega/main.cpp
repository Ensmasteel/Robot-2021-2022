// =============================
// ===       Libraries       ===
// =============================

#include <Arduino.h>
#include "Communication.h"

#define DELAY 10
uint32_t currentMillis = 0, lastMillis = 0;
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