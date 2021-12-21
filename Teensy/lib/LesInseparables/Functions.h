#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

/**
 * Defines number of functions for the robot.
 */


class Robot;
/**
 * Send a tweak to the PID
 * @param robot , pointer to the robot.
 */
void PID_tweak(Robot* robot);

/**
 * Add a Backward_Action to the sequence running.
 * @param robot, pointer to the robot.
 */
void add_backward(Robot* robot);

/**
 * Add a Spin_Action to the sequence running.
 * @param robot, pointer to the robot.
 */
void add_spin(Robot* robot);

/**
 * Pauses and locks the main sequence on the robot.
 * @param robot, pointer to the robot.
 */
void pauseNlockMainSequence(Robot* robot);

/**
 * Send a ping to the robot, to verify the connection.
 * @param robot, pointer to the robot.
 */
void ping(Robot* robot);

/**
 * Shutdown the robot.
 * @param robot
 */
void shutdown(Robot* robot);

void setTimeStart(Robot* robot);

/**
 * Add a backHomme sequence to the sequence running.
 * @param robot
 */
void startBackHomeSeq(Robot* robot);

/**
 * Set the nuorth to robot.
 * @param robot
 */
void setNorth(Robot* robot);

/**
 * Set South to robot.
 * @param robot
 */
void setSouth(Robot* robot);

/**
 * Recalibrating with the edges of the table.
 * @param robot
 */
void recallageBordure(Robot* robot);

/**
 * Forces main sequence to start.
 * @param robot
 */
void forceMainSeqNext(Robot* robot);

/**
 * Starts time sequence.
 * @param robot
 */
void startTimeSeq(Robot* robot);


#endif // !FUNCTIONS_H_