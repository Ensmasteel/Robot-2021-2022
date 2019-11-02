class newVector
{
public:
    newVector(float x=0.0, float y=0.0, bool absolute=false); //Absolute et Relatif
    newVector(float xB=0.0, float yB=0.0, float xJ=0.0, float yJ=0.0); //Specific
    float getX();
    float getY();

protected:
    enum Type
    {
        RELATIVE,   // Si Jaune alors getX renvoie 3.0 - _x   et  getTheta renvoie PI - _theta
        ABSOLUTE,   // Independant du coté
        SPECIFIC    // X Y Theta dépendent du coté
    };
    float _x;
    float _y;
    Type type;
};

class newVectorE : public newVector
{
protected:
    float _theta;
public:
    float getTheta();
    newVectorE(float x=0.0, float y=0.0, float theta=0.0, bool absolute=false); //Absolute et Relatif
    newVectorE(float xB=0.0, float yB=0.0, float thetaB=0.0, float xJ=0.0, float yJ=0.0, float thetaJ=0.0); //Specific

};

class newCinetique : public newVectorE
{
public:
    float _v;
    float _w;
    newCinetique(float x=0.0, float y=0.0, float theta=0.0, float v=0.0, float w=0.0, bool absolute=false); //Absolute et Relatif
    newCinetique(float xB=0.0, float yB=0.0, float thetaB=0.0, float xJ=0.0, float yJ=0.0, float thetaJ=0.0, float v=0.0, float w=0.0); //Specific

};