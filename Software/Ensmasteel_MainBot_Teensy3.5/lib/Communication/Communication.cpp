#include "Communication.h"

#include "Arduino.h"
#include <cstring>

Message newMessage(MessageID id, int32_t data)
{
    Message out;
    out.ID = id;
    out.data = data;
    return out;
}

Message MessageBox::pull()
{
    if (empty)
    {
        Serial.println("The mailbox is empty");
        //Dans ce cas on renvoie le message vide
        return newMessage(MessageID::Empty, 0);
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

void MessageBox::push(Message message)
{
    if ((iFirstEntry == iNextEntry) && !empty)
        Serial.print("The mailbox is full");
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

void Communication::actuate()
{
    //RECEPTION
    if (Serial1.available() >= 6) //On attend de voir 6 octets dans le buffer pour lire le message entier d'un coup
    {
        uint8_t in[6];
        for (int i = 0; i < 6; i++)
            in[i] = Serial.read();
        Message out;
        memcpy(&out, in, sizeof(out)); //On convertit les octets en message
        receiveBox.push(out);
    }

    //EMISSION
    if (!sendingBox.empty && ((millis() - millisLastSend) > ANTISPAM_MS))
    {
        Message toSend = sendingBox.pull();
        uint8_t out[6];
        memcpy(out, &toSend, sizeof(out)); //On convertit le message en octet
        for (int i = 0; i < 6; i++)
            Serial.write(out[i]);
        millisLastSend = millis();
    }
}

void Communication::send(Message message)
{
    sendingBox.push(message);
}

Message Communication::pullOldestMessage()
{
    return receiveBox.pull();
}

uint8_t Communication::inWaiting()
{
    return receiveBox.size();
}

Communication::Communication()
{
    //On vide les caractères qui pourrait trainer
    while (Serial.available() > 0)
    {
        Serial.read();
    }
    millisLastSend = millis();
}
