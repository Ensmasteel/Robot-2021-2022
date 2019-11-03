/**   Ensmasteel Library - Vector modelisation
 * 
 * author : Arthur FINDELAIR - EnsmaSteel, github.com/ArthurFDLR
 * date : October 2019
*/

#ifndef VECTOR_H_
#define VECTOR_H_

#include "Arduino.h"
#include "Logger.h"

float normalizeAngle(float angle);

class Vector
{
public:
    float _x, _y;
    Vector operator+(const Vector &other);
    void operator+=(const Vector &other);
    Vector operator-(const Vector &other);
    float operator%(const Vector &other); //Produit scalaire
    Vector operator*(const float scalaire); //Produit par un scalaire (homotetie)
    bool operator==(Vector const &other);
    float norm();
    float angle();
    float distanceWith(Vector &other);
    void print(const String& prefix="",bool info=false);
    void toTelemetry(const String& prefix="");
    Vector(float x = 0.0, float y = 0.0);
};

Vector directeur(float theta);

class VectorE : public Vector
{
public:
    float _theta;
    void normalizeTheta();
    void print(const String& prefix="",bool info=false);
    void toTelemetry(const String& prefix="");
    bool operator==(VectorE const &other);
    VectorE(float x = 0.0, float y = 0.0, float theta = 0.0);
};



class Cinetique : public VectorE
{
public:
    float _v;
    float _w;
    void print(const String& prefix="",bool info=false);
    void toTelemetry(const String& prefix="");
    bool operator==(Cinetique const &other);
    Cinetique(float x = 0.0, float y = 0.0, float theta = 0.0, float v = 0.0, float w = 0.0);
};

// =======     TARGETS    =======
// ==============================
enum TeamColor
{
    BLEU,
    JAUNE,
};

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
    Target();
};

class TargetVector : public Target
{
private:
    float _xB,_yB;
    float _xJ,_yJ;
public:
    Vector getVector();
    TargetVector(float x, float y, bool absolute);
    TargetVector(float xB, float yB, float xJ, float yJ);
    TargetVector(){}
};

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
