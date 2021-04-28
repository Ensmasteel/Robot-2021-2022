# <h1 align = "center"> EnsmaSteel bot 2020 - 2021 </h1> <!-- omit in toc -->

<img src="./.github/images/EnsmaSteel_Bot.png" width="100%" alt="Robot during 2019 competition preparation" class="center">

🚧 MarkdDown file in progress 🚧

Source code of Ensmasteel's bot. 

- [Robot configuration](#robot-configuration)
  - [Sequences](#sequences)
  - [Actions](#actions)
- [Navigation](#navigation)
  - [Trajectory computation](#trajectory-computation)
  - [Odometry](#odometry)
  - [Position control](#position-control)
- [Electronics](#electronics)
- [Mecanics](#mecanics)

## Robot configuration
### Sequences

Present Sequence type

### Actions

Present possible actions to stack in sequences

## Navigation
### Trajectory computation

<!---
Commençons par déterminer la trajectoire du robot lorsqu'il se déplace d'un point A à un point B. Pour se faire nous calculons deux polynômes définissant l'abscisse X et l'ordonnée Y en fonction d'une variable arbitraire notée t*. Ces fonctions sont des [courbes de Bézier d'ordre 3](https://www.desmos.com/calculator/cahqdxeshd "Bezier viewer"). On peut alors tracer la trajectoire en connaissant le point de départ __(X0,Y0)__, le point d'arrivé __(X3,Y3)__ et les points intermédiaires __(X1,Y1)__ et __(X2,Y2)__. Ces points intermédiraires sont déterminés pour obtenir la courbure et plus particulièrement, l'orientation initiale et finale du robot.
L'orientation du robot est essentielle pour effectuer les actions, nous devons donc pouvoir contrôler l'orientation d'arrivé en __(X3,Y3)__. On se contentera de partir dans le sens initial du robot en début de trajectoire, quitte à tourner avant.

Nous stockons toutes les informations relatives à la trajectoire dans un objet _Ghost_. Le robot physique se contentera de suivre cette trajectoire par un asservissement en position. Afin de permettre au robot de suivre la trajectoire, la position ne suffit pas, il faut aussi la vitesse de déplacement. La variable t* des courbes de Beziers n'a aucun sens temporellement. On souhaite imposer la vitesse progressivement. On définit alors une fonction _V(t)_ déterminant la vitesse d'avancer le long de l'abscisse curviligne décrivant la trajectoire. On a alors _dt*=(V(t)/V*(t*))x(dt)_ avec _dt_ le temps écoulé entre deux tour de boucle (de calcul), _V*(t*)_ la vitesse réelles _V*²(t*)=X’(t*)²+Y’(t*)²_ où _X_ et _Y_ sont les courbes de Beziers) et _dt*_ le pas d'incrémentation de _t*_ pour le calcul de la position suivante. _(cf Passation-CODE.docx pour plus de détails)_
-->
### Odometry
### Position control
<!---
L'ensembles des positions sont stocker sous forme de _VectorE(x, y, theta)_ indiquant la position en coordonées x,y en mètres et l'orientation du robot en radiants _(note : theta est toujours compris dans [-PI; PI])_. Le type _Cinetique_ est attendu en entrée d'asservissement. Il contient la position et l'orientation du Ghost en temps réelle et sa vitesse linéaire et en rotation dans le futur d'un délai de _delayPosition_ en milli-secondes. Ce délai permet à l'asservissement de "prédire" les accélérations/décélérations afin d'etre plus fluide.
-->
## Electronics

## Mecanics
