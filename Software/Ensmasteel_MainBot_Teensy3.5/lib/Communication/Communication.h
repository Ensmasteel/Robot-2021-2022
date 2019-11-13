#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#define ANTISPAM_MS 300 //Nombre de milliseconde entre deux envoi de message

#include "Arduino.h"
#include "MessageID.h"
#include <Stream.h>
#include "Vector.h"

class MessageBox;

struct FourBytes
{
    uint8_t byte0,byte1,byte2,byte3;
};

struct Message
{
    int32_t _data;
    uint16_t _ID;
}; //        Taille de 6 octets !

//Je fais cette implémentation "non POO" car je veux etre certains qu'un mesasge fasse 6 octets....
Message newMessage(MessageID id, int32_t data);
Message newMessage(MessageID id);
Message newMessage(MessageID id, uint8_t byte0,uint8_t byte1,uint8_t byte2,uint8_t byte3);
Message newMessage(MessageID id, VectorE vectorE);
MessageID extractID(Message message);
FourBytes extract4Bytes(Message message);
int32_t extractInt32(Message message);
Vector extractVectorE(Message message);

class Communication
{
public:
    void send(Message message); //Envoie un message sur le port de communication (Attention: Asynchrone)
    void popOldestMessage(); //Supprime le dernier message de la boite de reception
    Message peekOldestMessage(); //Renvoie le message le plus ancien (sachant qu'a chaque tour, un message est supprimé de la boite au lettre)
    uint8_t inWaitingRx(); //Renvoie le nombre de message en attente dans la boite de reception
    uint8_t inWaitingTx(); //Renvoie le nombre de message en attente dans la boite d'envoie
    void update(); //Doit etre appelé régulièrement. Mets a jours les boites d'envoie et de reception
    Communication(Stream* port=&Serial); //port : Le port a utiliser pour les communications.
    void operator=(const Communication &other); //Pour eviter d'avoir des pointeurs vers des variables locales...
    void toTelemetry();

private:
    Stream* port;
    uint32_t millisLastSend;
    MessageBox* sendingBox;
    MessageBox* receiveBox;
};
#endif
