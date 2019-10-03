# Robot principal ENSMASTEEL
## Trajectoire

Commencons par determiner la trajectoire du robot lorsqu'il se deplace d'un point A à un point B. Pour se faire nous calculons deux polynomes definissant l'abscisse X et l'ordonnée Y en fonction d'une variable arbitraire notée t*. Ces fonctions sont des [courbes de Bézier d'ordre 3](https://www.desmos.com/calculator/cahqdxeshd "Bezier viewer"). On peut alors tracer la trajectoire en connaissant le point de départ __(X0,Y0)__, le point d'arrivé __(X3,Y3)__ et les points intérmediaires __(X1,Y1)__ et __(X2,Y2)__. Ces points intermediraires sont déterminé pour obtenir la courbure et plus particulierement, l'orientation initiale et finale du robot.
L'orientation du robot est essentielle pour effectuer les actions, nous devons donc pouvoir controler l'orientation d'arrivé en __(X3,Y3)__. On se contentera de partir dans le sens initiale du robot en début de trajectoir quite à tourner avant.

Nous stockons toutes les informations relative à la trajectoire dans un objet _Ghost_. Le robot physique se contentera de suivre cette trajectoire par un asservissement en position. Afin de permettre au robot de suivre la trajectoire, la position ne suffit pas, il faut aussi la vitesse de déplacement. La variable t* des courbes de Beziers n'a aucun sens temporellement. On souhaite imposer la vitesse progressivement. On défini alors une fonction _V(t)_ détérminant la vitesse d'avancer le long de l'abscice curviligne décrivant la trajectoire. On a alors _dt*=(V(t)/V*(t*))x(dt)_ avec _dt_ le temps écoulé entre deux tour de boucle (de calcule), _V*(t*)_ la vitesse reelles _V*²(t*)=X’(t*)²+Y’(t*)²_ où _X_ et _Y_ sont les courbes de Beziers) et _dt*_ le pas d'incrémentation de _t*_ pour le calcule de la position suivante. _(cf Passation-CODE.docx pour plus de détails)_


```c++
class Ghost {
public:
    VectorE posCurrent;  // VectorE : struct type containing X,Y,Orientation
    bool locked=true;    // Ghost moving => True
    Polynome trajectory; // Bezier curve


    Ghost();
    Ghost(VectorE posEini);

    int rotate(float theta); //rotatation of theta rad
    int actuate(float dt);   // Update position with trajectory
    void Set_Trajectory(Polynome newTrajectory);
    void unlock();
};
```
