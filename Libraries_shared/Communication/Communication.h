#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#define ANTISPAM_MS 300 //Nombre de milliseconde entre deux envoi de message

#include "Arduino.h"
#include "MessageID.h"
#include <Stream.h>
#include "Vector.h"

class MessageBox;

/**
 * Struct to ensure that the message passed is on 4 bytes.
 */
struct FourBytes
{
    uint8_t byte0,byte1,byte2,byte3;
};

/**
 * Struct to ensure the message is encoded on 6 bytes.
 */
struct Message
{
    int32_t _data;
    uint16_t _ID;
}; //        Taille de 6 octets !

//Je fais cette implémentation "non POO" car je veux etre certains qu'un mesasge fasse 6 octets....
/**
 * Constructor of a new message passing an ID and the data as parameters.
 * @param id : the message ID of the message
 * @param data : The data contained in the message
 * @return a Message ready to send.
 */
Message newMessage(MessageID id, int32_t data);

/**
 * Another public constructor, getting only the ID.
 * @param id The message ID of the message.
 * @return a Message ready to send, with no Data. Can be fill up later.
 */
Message newMessage(MessageID id);

/**
 * Another public constructor, getting four bytes as parameters, and the ID.
 * Ensure you to construct a 6 bytes Message.
 * @param id The message ID.
 * @param byte0 The first byte of you data.
 * @param byte1 The second one.
 * @param byte2 The third one.
 * @param byte3 The fourth one.
 * @return A message ready to send, only on 6 bytes.
 */
Message newMessage(MessageID id, uint8_t byte0,uint8_t byte1, uint8_t byte2, uint8_t byte3);

/**
 * A constructor to create a message containing a vector of data. Ensure that the vector is encoded on 4 bytes.
 * @param id The message ID
 * @param vectorE The vector containing the data you want to send.
 * @return A message ready to send, containing the data of the vector.
 */
Message newMessage(MessageID id, VectorE vectorE);

/**
 * Returns the ID of a message. Useful to read the information got from the sender.
 * @param message A message to read.
 * @return The message ID.
 */
MessageID extractID(Message message);

/**
 * Extract a 4 bytes data from the message.
 * @param message The message to read
 * @return FourBytes, the data to use, encoded on 4 bytes.
 */
FourBytes extract4Bytes(Message message);

/**
 * Extract an int32_t data (aka a 4 bytes data but encoded all in once).
 * @param message The message to read
 * @return int32_t, the data to use in one unique variable.
 */
int32_t extractInt32(Message message);

/**
 * Does the same as above, but useful when the message is send with a Vector.
 * @param message The message to read
 * @return The vector contained in the message.
 */
Vector extractVectorE(Message message);

/**
 * Extract the order contained in the message. Uses all the functions above.
 * @param message The message to read containing an order.
 * @return Actuator_Order the order to send to actuators.
 */
Actuator_Order extractOrder(Message message);


/**
 * Class defining the communications ways used in the robot.
 */
class Communication
{
public:
    /**
     * Send a message on the communication port.
     * @warning this function is asynchronous.
     * @param message : the message to send.
     */
    void send(Message message);

    /**
     * Pop the oldest message in the reception box.
     */
    void popOldestMessage();

    /**
     * Pop the oldest message in the reception box and returns it.
     * @return Message, the oldest message in the reception box.
     */
    Message peekOldestMessage();

    /**
     * Gives the number of messages contained in the reception box and waiting for treatment.
     * @return uint8_t, the number of messages.
     */
    uint8_t inWaitingRx();

    /**
     * Same as above but with the sender box.
     * @return uint8_t, the number of messages waiting.
     */
    uint8_t inWaitingTx();

    /**
     * To be called regularly. Updates the reception and emission box.
     */
    void update();

    /**
     * Constructor of the communication port.
     * @param port the port to use for communication.
     */
    Communication(Stream* port=&Serial);

    /**
     * Redefinition of the operator '=', to ensure there is no pointer on local variables.
     * @param other Any other communication port.
     */
    void operator=(const Communication &other);

    /**
     * Method to send information to the telemetry.
     */
    void toTelemetry();

private:
    Stream* port;
    uint32_t millisLastSend;
    MessageBox* sendingBox;
    MessageBox* receiveBox;
};
#endif
