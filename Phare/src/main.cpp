#include <Arduino.h>

const uint8_t L1 = 53; // broche 2 du micro-contrôleur se nomme maintenant : L1nvoid setup() //fonction d'initialisation de la carte



void setup() {
  // put your setup code here, to run once:

//contenu de l'initialisation
pinMode(L1, OUTPUT); //L1 est une broche de sortie
Serial.begin(9600);
Serial.println("Liaison initialise");
}
void loop() //fonction principale, elle se répète (s’exécute) à l'infini
{
//contenu du programmen
digitalWrite(L1, HIGH); //allumer
Serial.println("Led Allume");
delay(1000); // attendre 1 seconde
digitalWrite(L1, (uint8_t)LOW); // Eteindre L1n
delay(2000); // attendre 2 seconde
Serial.println("Led Eteinte");

}