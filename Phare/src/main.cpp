#include <Arduino.h>


#define LED_1 2
#define LED_2 3
#define LED_3 4
#define LED_4 5
#define LED_5 6
#define LED_6 7
#define LED_7 8





const uint8_t L1 = 2; // broche 2 du micro-contrôleur se nomme maintenant : L1nvoid setup() //fonction d'initialisation de la carte



void setup() {
  // put your setup code here, to run once:

//contenu de l'initialisation
pinMode(LED_1, OUTPUT); //L1 est une broche de sortie
pinMode(LED_2, OUTPUT);
pinMode(LED_3, OUTPUT);
pinMode(LED_4, OUTPUT);
pinMode(LED_5, OUTPUT);
pinMode(LED_6, OUTPUT);
pinMode(LED_7, OUTPUT);

Serial.begin(9600);
Serial.println("Liaison initialise");
}
void loop() //fonction principale, elle se répète (s’exécute) à l'infini
{
//contenu du programmen
digitalWrite(LED_1, HIGH); //allumer
delay(2000);
digitalWrite(LED_1, LOW); //eteindre

digitalWrite(LED_2, HIGH);
delay(2000);

digitalWrite(LED_2, LOW);
digitalWrite(LED_3, HIGH);

delay(2000);
digitalWrite(LED_3, LOW);
digitalWrite(LED_4, HIGH);

delay(2000);

digitalWrite(LED_4, LOW);
digitalWrite(LED_5, HIGH);
delay(2000);

digitalWrite(LED_5, LOW);
digitalWrite(LED_6, HIGH);

delay(2000);
digitalWrite(LED_6, LOW);
digitalWrite(LED_7, HIGH);

delay(2000);
digitalWrite(LED_7, LOW);

}