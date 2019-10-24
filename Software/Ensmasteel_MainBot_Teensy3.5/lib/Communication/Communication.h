#ifndef COMMUNICATION_H
#define COMMUNICATION_H

//Taille des boites d'envoie et reception
#define MESSAGE_BOX_SIZE 10

//Nombre de milliseconde entre deux envoi de message
#define ANTISPAM_MS 300

#include "Arduino.h"

#include <Stream.h>

//En-tete d'un message
enum MessageID : uint16_t
{
    Empty,
    Stop,
    Tirette
};

struct Message
{
    int32_t data; //La plus part du temps, ce champ sera vide....
    uint16_t ID;
}; //        Taille de 6 octets !

Message newMessage(MessageID id, int32_t data);

class MessageBox
{
public:
    Message pull();
    Message peek();
    void push(Message message);
    int size();
    bool empty = true;

private:
    Message box[MESSAGE_BOX_SIZE];
    uint8_t iFirstEntry = 0;
    uint8_t iNextEntry = 0;
};

class Communication
{
public:
    void send(Message message);
    Message pullOldestMessage();
    Message peekOldestMessage();
    uint8_t inWaiting();
    void update();
    Communication(Stream* port=&Serial);

private:
    Stream* port;
    uint32_t millisLastSend;
    MessageBox sendingBox;
    MessageBox receiveBox;
};
#endif
