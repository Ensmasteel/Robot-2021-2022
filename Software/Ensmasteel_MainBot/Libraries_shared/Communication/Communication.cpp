#include "Communication.h"

#include "Arduino.h"
#include "Logger.h"
#ifdef TEENSY35
#include <cstring>
#else
#include <string.h>
#endif

#define MESSAGE_BOX_SIZE 10 //Taille des boites d'envoie et reception

typedef struct
{
    uint8_t x;
    uint8_t y;
    int16_t theta;
}UIntVectorE;

union Decoder
{
    FourBytes fBytes;
    int32_t data;
    UIntVectorE uIntVectorE;
};

Decoder decoder;

Message newMessage(MessageID id)
{
    Message out;
    out._ID = id;
    out._data = 0;
    return out;
}


Message newMessage(MessageID id, int32_t data)
{
    Message out;
    out._ID = id;
    out._data = data;
    return out;
}

Message newMessage(MessageID id, uint8_t byte0,uint8_t byte1,uint8_t byte2,uint8_t byte3)
{
    FourBytes fBytes;
    fBytes.byte0=byte0;
    fBytes.byte1=byte1;
    fBytes.byte2=byte2;
    fBytes.byte3=byte3;
    decoder.fBytes=fBytes;
    return newMessage(id,decoder.data);
}

Message newMessage(MessageID id, VectorE vectorE)
{
    decoder.uIntVectorE.x=(uint8_t)round(vectorE._x*__UINT8_MAX__/3.0);
    decoder.uIntVectorE.y=(uint8_t)round(vectorE._y*__UINT8_MAX__/2.0);
    decoder.uIntVectorE.theta=(int16_t)round(vectorE._theta*__INT16_MAX__/PI);
    return newMessage(id,decoder.data);
}

MessageID extractID(Message message){
    return (MessageID)message._ID;
}

FourBytes extract4Bytes(Message message){
    decoder.data=message._data;
    return decoder.fBytes;
}

int32_t extractInt32(Message message){
    return message._data;
}

Vector extractVectorE(Message message){
    decoder.data=message._data;
    VectorE out;
    out._x=decoder.uIntVectorE.x*3.0/__UINT8_MAX__;
    out._y=decoder.uIntVectorE.y*2.0/__UINT8_MAX__;
    out._theta=decoder.uIntVectorE.theta*PI/__INT16_MAX__;
    return out;
}

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

Message MessageBox::pull()
{
    if (empty)
    {
        Logger::infoln("The mailbox is empty");
        //Dans ce cas on renvoie le message vide
        return newMessage(MessageID::Empty_M, 0);
    }
    else
    {
        //On recupere le message et on fait avancer le buffer
        Message out = box[iFirstEntry];
        iFirstEntry = (iFirstEntry + 1) % MESSAGE_BOX_SIZE;
        empty = (iFirstEntry == iNextEntry);
        return out;
    }
}

Message MessageBox::peek()
{
    if (empty)
    {
        Logger::infoln("The mailbox is empty");
        //Dans ce cas on renvoie le message vide
        return newMessage(MessageID::Empty_M, 0);
    }
    else
        return box[iFirstEntry];
}

void MessageBox::push(Message message)
{
    if ((iFirstEntry == iNextEntry) && !empty)
        Logger::infoln("The mailbox is full");
    //Dans ce cas on n'empile pas
    else
    {
        //Sinon on empile et on fait avancer l'indice de la prochaine entrée
        box[iNextEntry] = message;
        iNextEntry = (iNextEntry + 1) % MESSAGE_BOX_SIZE;
        empty = false;
    }
}

int MessageBox::size()
{
    if (empty)
        return 0;
    else if (iNextEntry == iFirstEntry)
        return MESSAGE_BOX_SIZE;
    else
        return (iNextEntry - iFirstEntry + MESSAGE_BOX_SIZE) % MESSAGE_BOX_SIZE; //Marche dans tous les cas
}

void Communication::update()
{
    //RECEPTION
    if (port->available()>0)
        Logger::debugln(String(port->available())+" bytes available");
    if (port->available() >= 6) //On attend de voir 6 octets dans le buffer pour lire le message entier d'un coup
    {
        uint8_t in[6];
        for (int i = 0; i < 6; i++)
            in[i] = port->read();
        Message out;
        memcpy(&out, in, sizeof(out)); //On convertit les octets en message
        receiveBox->push(out);
    }
    if (inWaitingRx()>0)
        Logger::debugln("received ID " + String(extractID(peekOldestMessage())));

    //EMISSION
    if (!sendingBox->empty && ((millis() - millisLastSend) > ANTISPAM_MS))
    {
        Message toSend = sendingBox->pull();
        uint8_t out[6];
        memcpy(out, &toSend, sizeof(out)); //On convertit le message en octet
        for (int i = 0; i < 6; i++)
            port->write(out[i]);
        millisLastSend = millis();
    }
}

void Communication::send(Message message)
{
    sendingBox->push(message);
}

void Communication::popOldestMessage()
{
    receiveBox->pull();
}

Message Communication::peekOldestMessage()
{
    return receiveBox->peek();
}

uint8_t Communication::inWaitingRx()
{
    return receiveBox->size();
}

uint8_t Communication::inWaitingTx()
{
    return sendingBox->size();
}

Communication::Communication(Stream* port)
{
    this->port=port;
    this->receiveBox = new MessageBox();
    this->sendingBox = new MessageBox();
    //On vide les caractères qui pourrait trainer
    while (port->available() > 0){
        port->read();
    }
    millisLastSend = millis();
}

void Communication::operator=(const Communication &other)
{
    this->port=other.port;
    this->receiveBox=new MessageBox();
    this->sendingBox=new MessageBox();
}



void Communication::toTelemetry()
{
    if (inWaitingRx()>0)
        Logger::toTelemetry("messId",String(extractID(peekOldestMessage())));
}
