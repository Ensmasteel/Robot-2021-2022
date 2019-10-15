#include "Communication.h"

#include "Arduino.h"
#include <cstring>

Message newMessage(MessageID id, int32_t data)
{
    Message out;
    out.ID=id;
    out.data=data;
    return out;
}

Message MessageBox::pull()
{
    if (empty)
    {
        Serial.println("The mailbox is empty");
        return newMessage(MessageID::Empty,0);
    }
    else
    {
        Message out =   box[iFirstEntry];
        iFirstEntry =   (iFirstEntry + 1)%MESSAGE_BOX_SIZE;
        empty = (iFirstEntry==iNextEntry);
        return out;
    }

}

void MessageBox::push(Message message)
{
    if ((iFirstEntry==iNextEntry) && !empty)
        Serial.print("The mailbox is full");
    else
    {
        box[iNextEntry]=message;
        iNextEntry = (iNextEntry + 1)%MESSAGE_BOX_SIZE;
        empty = false;
    }
}

int MessageBox::size()
{
    if (empty)
        return 0;
    else if (iNextEntry==iFirstEntry)
        return MESSAGE_BOX_SIZE;
    else
        return (iNextEntry-iFirstEntry+MESSAGE_BOX_SIZE)%MESSAGE_BOX_SIZE;
}


void Communication::actuate()
{
    if (Serial1.available()>=6)
    {
        uint8_t in[6];
        for (int i=0;i<6;i++)
            in[i]=Serial.read();
        Message out;
        memcpy(&out,in,sizeof(out));
        receiveBox.push(out);
    }
    if (!sendingBox.empty && ((millis() - millisLastSend)>ANTISPAM_MS))
    {
        Message toSend = sendingBox.pull();
        uint8_t out[6];
        memcpy(out,&toSend,sizeof(out));
        for (int i=0;i<6;i++)
            Serial.write(out[i]);
        millisLastSend=millis();
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
  while(Serial.available()>0){Serial.read();}
  millisLastSend=millis();
}

