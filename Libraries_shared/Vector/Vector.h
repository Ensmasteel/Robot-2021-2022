/**   Ensmasteel Library - Vector modelisation
 *
 * author : Arthur FINDELAIR - EnsmaSteel, github.com/ArthurFDLR
 * date : October 2019
*/

#ifndef VECTOR_H_
#define VECTOR_H_

#include "Arduino.h"
#include "Logger.h"

/**
 * Normalize an angle, from degree to rad.
 * @param angle : float, angle value in degree.
 * @return : float, angle value in rad.
 */
float normalizeAngle(float angle);

/**
 * Class defining the vector model used in this code.
 */
class Vector
{
public:

    float _x, _y; ///< Coordinates of the vector
    /**
     * Public constructor of the vector.
     * @param x : float, X-axis coordinate.
     * @param y : float, Y-axis coordinate.
     *
     */
    Vector(float x = 0.0, float y = 0.0);

    Vector operator+(const Vector &other); ///<Redefines the + operator to work with vectors.
    void operator+=(const Vector &other); ///<Redefines the += operator to work with vectors.
    Vector operator-(const Vector &other); ///<Redefines the - operator to work with vectors.
    float operator%(const Vector &other); ///< Scalar product.
    Vector operator*(const float scalaire); ///< Homothety.
    bool operator==(Vector const &other); ///<Redefines the == operator to work with vectors.

    float norm(); ///<Returns the vector's norm.
    float angle(); ///<Returns the vector's angle with base.
    float distanceWith(Vector &other); ///<Returns the distance between two vectors.
    void print(const String& prefix="",bool info=false);///<Prints the values of vectors.
    void toTelemetry(const String& prefix="");///<Send the vectors' info to telemetry.
    Vector rotate(float theta);///<Adds an angle to Vector.
};

Vector directeur(float theta);///<Defines the principal vector.

/**
 * Class extending the vector base class.
 */
class VectorE : public Vector
{
public:
    float _theta; //<Angle of vector
    /**
     * Public constructor of the VectorE
     * @param x : float, X-axis coordinate
     * @param y : float, Y-axis coordinate
     * @param theta : float, angle.
     */
    VectorE(float x = 0.0, float y = 0.0, float theta = 0.0);

    void normalizeTheta();///<Normalizes the vector's angle.
    void print(const String& prefix="",bool info=false);///<Prints vector values
    void toTelemetry(const String& prefix="");///<Send vector's values to telemetry.
    bool operator==(VectorE const &other);///<Redefines the == operator to work with VectorE
};


/**
 * Defines a kinetic vector class.
 */
class Cinetique : public VectorE
{
public:
    float _v; ///<Speed value
    float _w; ///<Rotation speed value
    /**
     * Public constructor of the kinetic vector class
     * @param x : float, X-axis coordinate
     * @param y : float, Y-axis coordinate
     * @param theta : float, Vector's angle
     * @param v : Speed value
     * @param w : Rotation speed value
     */
    Cinetique(float x = 0.0, float y = 0.0, float theta = 0.0, float v = 0.0, float w = 0.0);
    void print(const String& prefix="",bool info=false);///<Prints kinetic vector values
    void toTelemetry(const String& prefix="");///<Send kinetic vector values to telemetry.
    bool operator==(Cinetique const &other);///<Redefines the == operator to work with kinetic vectors.
};

// =======     TARGETS    =======
// ==============================
/**
 * Enum defining the color team [[deprecated in 2022 edition]]
 */
enum TeamColor
{
    BLEU,
    JAUNE,
};

/**
 * Class defining target for robot. To be updated for 2022 edition because its relative to 2021 teams.
 */
class Target
{
protected:
    enum Type
    {
        RELATIVE, // Si Jaune alors getX renvoie 3.0 - _x   et  getTheta renvoie PI - _theta
        ABSOLUTE, // Independant du coté
        SPECIFIC  // X Y Theta dépendent du coté
    };
    static TeamColor teamColor;
    Type type;
    constexpr static float LENGTH_TABLE = 3.0; // meters
    constexpr static float WIDTH_TABLE = 2.0; //meters
public:
    static void setTeamColor(TeamColor color);
    Target();//<Public constructor for target.
};

/**
 * Class extending the target class, defining a vector to reach target, depending on team color. [[deprecated for 2022 edition]]
 */
class TargetVector : public Target
{
private:
    float _xB,_yB;
    float _xJ,_yJ;
public:
    Vector getVector();
    TargetVector(float x, float y, bool absolute);
    TargetVector(float xB, float yB, float xJ, float yJ);
    TargetVector::TargetVector(Vector v,bool absolute);
    TargetVector(){}
};

/**
 * Class extending the target class, defining a vectorE for target, depending on team color. [[deprecated for 2022 edition]]
 */
class TargetVectorE: public Target
{
private:
    float _xB,_yB,_thetaB;
    float _xJ,_yJ,_thetaJ;
public:
    VectorE getVectorE();
    TargetVectorE(float x, float y, float theta, bool absolute);
    TargetVectorE(float xB, float yB, float thetaB, float xJ, float yJ, float thetaJ);
    TargetVectorE(float theta, bool absolute);
    TargetVectorE(float thetaB, float thetaJ);
    TargetVectorE(){}
};

/**
 * Extends the target class, defining a target kinetic vector for reaching target. To be updated for 2022 edition
 */
class TargetCinetique: public Target
{
private:
    float _xB,_yB,_thetaB;
    float _xJ,_yJ,_thetaJ;
    float _v,_w;
public:
    Cinetique getCinetique();
    TargetCinetique(float x, float y, float theta, float v, float w, bool absolute);
    TargetCinetique(float xB, float yB, float thetaB, float xJ, float yJ, float thetaJ, float v, float w);
    TargetCinetique(){}
};


#endif
