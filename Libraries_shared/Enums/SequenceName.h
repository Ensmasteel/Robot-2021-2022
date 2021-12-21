#ifndef SEQUENCENAME_H_
#define SEQUENCENAME_H_

/**
 * This enum details the sequences' names.
 */
enum SequenceName
{
    mainSequenceName,          ///< Defines the main sequence that will be performed on robot
    communicationSequenceName, ///< Defines the communication sequence
    timeSequenceName,          ///< Defines the sequence organizing time
    goNorthName,               ///< Defines the sequence for going to North [[deprecated in 2022 edition ]]
    goSouthName,               ///< Defines the sequence for going to South [[Deprecated in 2022 edition]]
    recallageListerName,       ///< Defines the sequence for recalibrating the PID and coder
    __NBSEQUENCES__            ///< Counts the number of sequences defined
};

#endif