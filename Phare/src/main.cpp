#include <Arduino.h>


#define LED_1 2
#define LED_2 3
#define LED_3 4
#define LED_4 5
#define LED_5 6
#define LED_6 7
#define LED_7 8





const uint8_t pinOutBouton = 0;
const uint8_t pinInBouton = 1;
const uint8_t delayStepper = 1500;
const uint8_t delayLed = 100;

const uint8_t stepPin = 34;
const uint8_t dirPin = 35;
const uint8_t sleepPin = 22;
const uint8_t pinM0 = 37;
const uint8_t pinM1 = 36;

const uint8_t steps = 200;


void allumerPhare(){ //On allume successivement toutes les leds du phare

  digitalWrite(LED_6, LOW);
  digitalWrite(LED_1, HIGH); 
  delay(delayLed);

  digitalWrite(LED_7, LOW);
  digitalWrite(LED_2, HIGH);
  delay(delayLed);

  digitalWrite(LED_1, LOW);
  digitalWrite(LED_3, HIGH);
  delay(delayLed);

  digitalWrite(LED_2, LOW);
  digitalWrite(LED_4, HIGH);
  delay(delayLed);

  digitalWrite(LED_3, LOW);
  digitalWrite(LED_5, HIGH);
  delay(delayLed);

  digitalWrite(LED_4, LOW);
  digitalWrite(LED_6, HIGH);
  delay(delayLed);
  
  digitalWrite(LED_5, LOW);
  digitalWrite(LED_7, HIGH);
  delay(delayLed);
}


void monterPhare(bool up, bool holdPosition){  
//permet de monter le phare 
  digitalWrite(sleepPin, HIGH);
  digitalWrite(dirPin, up ? HIGH : LOW);
  for(int x = 0; x < steps; x++) {
      digitalWrite(stepPin,HIGH);
      delayMicroseconds(delayStepper);
      digitalWrite(stepPin,LOW);
      delayMicroseconds(delayStepper);
    }
  digitalWrite(sleepPin, holdPosition ? HIGH : LOW);
}

void setup() {

//Initialisation des pin 
pinMode(LED_1, OUTPUT); 
pinMode(LED_2, OUTPUT);
pinMode(LED_3, OUTPUT);
pinMode(LED_4, OUTPUT);
pinMode(LED_5, OUTPUT);
pinMode(LED_6, OUTPUT);
pinMode(LED_7, OUTPUT);

pinMode(pinOutBouton, OUTPUT);
pinMode(pinInBouton, INPUT);

pinMode(stepPin, OUTPUT);
pinMode(dirPin, OUTPUT);
pinMode(sleepPin, OUTPUT);
pinMode(pinM0, OUTPUT);
pinMode(pinM1, OUTPUT);

//On s'assure de l'état BAS des pins du moteur 
digitalWrite(pinM0, LOW);
digitalWrite(pinM1, LOW);
digitalWrite(pinOutBouton, HIGH);


Serial.begin(9600);
Serial.println("Liaison initialise");
}
void loop() 
{

int tourBoucle = 0;

if (digitalRead(pinInBouton) == HIGH) { //Si on presse le bouton 

  Serial.println("Le Phare Monte!");
  monterPhare(true, true);

  delay(tempsEteint);
  Serial.println("Le Phare s'allume!");
  while (1)
  {
    allumerPhare();
    tourBoucle++;
    if (tourBoucle == 100)
      break;
  }
  
  
  
}

}



