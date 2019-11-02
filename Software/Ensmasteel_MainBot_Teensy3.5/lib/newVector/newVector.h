/**   Ensmasteel Library - Vector modelisation
 * 
 * author : EnsmaSteel, github.com/ArthurFDLR
 * date : November 2019
*/

#ifndef VECTOR_H_
#define VECTOR_H_

#include "Arduino.h"
#include "Logger.h"

enum TeamColor
{
    BLEU,
    JAUNE,
};

class Vector
{
public:
    Vector(float x = 0.0, float y = 0.0, bool absolute = false);            //Absolute et Relatif, dans le cas relatif, on donnera la position dans le cas bleu !
    Vector(float xB = 0.0, float yB = 0.0, float xJ = 0.0, float yJ = 0.0); //Specific
    float getX();
    float getY();
    void setTeamColor(TeamColor color);

    Vector operator+(const Vector &other);
    void operator+=(const Vector &other);
    Vector operator-(const Vector &other);
    float operator%(const Vector &other);   //Produit scalaire
    Vector operator*(const float scalaire); //Produit par un scalaire (homotetie)
    bool operator==(Vector const &other);
    float Norm();
    float angle();
    float distanceWith(Vector &other);
    void print(const String &prefix = "", bool info = false);
    void toTelemetry(const String &prefix = "");

protected:
    enum Type
    {
        RELATIVE, // Si Jaune alors getX renvoie 3.0 - _x   et  getTheta renvoie PI - _theta
        ABSOLUTE, // Independant du coté
        SPECIFIC  // X Y Theta dépendent du coté
    };

    float _xB, _xJ;
    float _yB, _yJ;
    Type type;
    static TeamColor color;
    const static float LENGTH_TABLE = 3.0; // meters
    const static float WIDTH_TABLE = 2.0; //meters
};

float normalizeAngle(float angle);

class VectorE : public Vector
{
public:
    VectorE(float x = 0.0, float y = 0.0, float theta = 0.0, bool absolute = false);                                 //Absolute et Relatif
    VectorE(float xB = 0.0, float yB = 0.0, float thetaB = 0.0, float xJ = 0.0, float yJ = 0.0, float thetaJ = 0.0); //Specific
    float getTheta();

    void normalizeTheta();
    void print(const String &prefix = "", bool info = false);
    void toTelemetry(const String &prefix = "");
    bool operator==(VectorE const &other);

protected:
    float _thetaB, _thetaJ;
};

class Cinetique : public VectorE
{
public:
    float _v;
    float _w;
    Cinetique(float x = 0.0, float y = 0.0, float theta = 0.0, float v = 0.0, float w = 0.0, bool absolute = false);                                 //Absolute et Relatif
    Cinetique(float xB = 0.0, float yB = 0.0, float thetaB = 0.0, float xJ = 0.0, float yJ = 0.0, float thetaJ = 0.0, float v = 0.0, float w = 0.0); //Specific

    void print(const String &prefix = "", bool info = false);
    void toTelemetry(const String &prefix = "");
    bool operator==(Cinetique const &other);
};

#endif
