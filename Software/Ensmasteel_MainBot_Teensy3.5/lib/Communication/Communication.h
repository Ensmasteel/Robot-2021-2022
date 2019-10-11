#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#define MESSAGE_BOX_SIZE 10
#define ANTISPAM_MS 300

#include "Arduino.h"

struct Message
{
    int32_t data;
    uint16_t ID;
};//        Taille de 5 octets !

class MessageBox
{
public:
    Message pull();
    void push(Message message);
    int size();
    bool empty = true;

private:
    Message box[MESSAGE_BOX_SIZE];
    uint8_t iFirstEntry =   0;
    uint8_t iNextEntry  =   0;

};

class Communication
{
public:
    void send(Message message);
    Message pullOldestMessage();
    uint8_t inWaiting();
    void actuate();
    Communication();

private:
    uint32_t millisLastSend;
    MessageBox sendingBox;
    MessageBox receiveBox;
};
#endif
