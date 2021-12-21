#ifndef MOVEPROFILE_H_
#define MOVEPROFILE_H_
#include "Arduino.h"

/**
 * File describing all of the movement profiles with enums and class.
 */

/**
 * Enum containing precisions on movement profile and states. This enum may not be updated.
 */
enum MoveProfileName
{
    off,      ///< Robot off.
    brake,    ///< brake order
    accurate, ///< Precision state good.
    standard, ///< Standard state, everything good.
    fast,     ///< Fast profile, concerning when the robot is accelerating and moving faster
    recalibrating, ///< When the PID and the coder are recalibrating.
    __NBPROFILES__ ///< counts the number of moving profiles .
};

/**
 * Class containing the moving profile and defining the actions lead by each.
 */
class MoveProfile
{
public:
    float KP, KI, KD, KA;    ///< PID factors : Proportional, Integrator, Diverter, Anticipator
    float epsilon, dEpsilon; ///< Factors below which we consider the robot is good for position and speed.
    float maxErr;            ///< Max error before considering the robot went to far.
    float speedRamps;   ///< Acceleration (positive or negative) in m.s-2 or rad.s-2.
    float cruisingSpeed; ///< Max speed (or rotating speed) in m/s (or rad/s)

private:
    void set(float KP,float KI,float KD,float KA,float epsilon,float dEpsilon,float maxErr,float speedRamps,float cruisingSpeed);
    friend class MoveProfiles;
};

/**
 * Setting move profiles for all robot.
 */
class MoveProfiles
{
private:
    static MoveProfile profiles[2 * __NBPROFILES__];
public:
    /**
     * Public initializer. To call at initialization.
     */
    static void setup();

    /**
     * Public constructor for move profiles.
     * @param name : Profile name
     * @param translation : Bool, determined if param will consider translation or rotation.
     * @return An instance of MoveProfiles
     */
    static MoveProfile* get(MoveProfileName name, bool translation);

    /**
     * Used to correct the PID factors.
     * @param profile : the profile impacted
     * @param incr : to increment or decrement the factors
     * @param whichOne : to chose which factor will be impacted (KP, KI, KD)
     * @return Float, the correct value of the new factor.
     */
    static float tweak(MoveProfile* profile,bool incr,int whichOne);
};


#endif // !MOVEPROFILE_H_
