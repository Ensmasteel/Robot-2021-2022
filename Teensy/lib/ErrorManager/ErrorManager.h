/**
 * The error manager file. Defines the errorManager class to handle errors.
 */

#ifndef ERRORMANAGER_H
#define ERRORMANAGER_H
#include "Arduino.h"

class ErrorBox;

/**
 * Enum to detail some classic error codes. To be updated each time a new error is defined.
 */
enum Error
{
    NO_ERROR,      ///<No error detected.
    PID_FAIL_ERROR, ///<Error on PID
    __NBERROR__  ///<Counts the number of error types enumerated
};

/**
 * The errorManager class handles each error and log them.
 */
class ErrorManager
{
public:
    static void raise(Error error); ///<Raise an error.
    static void popOldestError(); ///<Delete the oldest error raised.
    static Error peekOldestError(); ///<Returns the oldest error raised( knowing that each iteration an error is archived).
    static uint8_t inWaiting(); ///<Returns the number of error messages waiting in the sender box
    /**
     * Set every parameters up for usage. To be primary used before any other function in this module.
     */
    static void setup();
    /**
     * Reset every parameter to ensure no one is forgotten. To be called at the end of usage to reset all parameters for
     * next usage.
     */
    static void reset();
//    void toTelemetry(); ///<used to send errors to telemetry [[not already in use, to be updated and maybe used]]

private:
    static ErrorBox* errorBox;
    static float* timeLastIn;

};
#endif
